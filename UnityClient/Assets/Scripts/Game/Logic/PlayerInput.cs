using Google.Protobuf;
using InControl;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum TouchStatus
{
    TouchDown = 0,
    TouchUp = 1,
}
public class PlayerInput : Singleton<PlayerInput>
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    public static void OnInit()
    {
        PlayerInput.Instance.Registe();
    }
    public void Registe() 
    {
        ConnectionManager.RegisterConnectionEvent((in eConnectionEventType type, in Connection connection) =>
        {
            if (connection._type == eConnectionType.GameGate)
            {
                if (type == eConnectionEventType.Connected)
                {
                    con = connection;
                }
                else
                {
                    con = null;
                }
            }
        });
    }

    public bool enable = false;
    public float touchThreadhold = 5;
    public float sendThreadHold = 1;

    protected Connection con;
    protected Quaternion stickRotation;

    [NaughtyAttributes.ReadOnly] public float lastSendSec = 0;
    [NaughtyAttributes.ReadOnly] public float lastSendAngle = 0;
    [NaughtyAttributes.ReadOnly] public float curAngle = 0;
    [NaughtyAttributes.ReadOnly] public TouchStatus touchState = TouchStatus.TouchUp;
    [NaughtyAttributes.ReadOnly] public TouchStatus preTouchState = TouchStatus.TouchUp;
    private void Awake()
    {
        lastSendSec = Time.unscaledTime;
    }

    public void SendProto<T>(in eCPID id, in T body) where T : IMessage
    {
        if (null != con)
        {
            con.SendProto<T>(id, body);
        } else
        {
            Debug.LogError("lost connect gate");
        }
    }

    public void SetEnable(bool _enable)
    {
        if(_enable != enable)
        {
            enable = _enable;
            ResetInput();
        }
    }

    private void Update()
    {
        if (enable)
        {
            //ProcessInput();
            TouchStatus _keyState = TouchStatus.TouchDown;
            if (InputManager.ActiveDevice.LeftStick.IsPressed)
            {
                Vector2 dir = InputManager.ActiveDevice.LeftStick.Vector;
                if(dir.y ==0)
                    curAngle = dir.y > 0 ? 0 : 180;
                else
                {
                    stickRotation.SetLookRotation(new Vector3(dir.x, 0, dir.y), Vector3.up);
                    curAngle = stickRotation.eulerAngles.y;
                }
            }
            else
            {
                float w = Input.GetKey(KeyCode.W) ? 1 : 0;
                float s = Input.GetKey(KeyCode.S) ? -1 : 0;
                float a = Input.GetKey(KeyCode.A) ? -1 : 0;
                float d = Input.GetKey(KeyCode.D) ? 1 : 0;
                if((w + s) != 0 || (a + d) != 0)
                {
                    stickRotation.SetLookRotation(new Vector3(a + d, 0, w + s), Vector3.up);
                    curAngle = stickRotation.eulerAngles.y;
                }
                else
                {
                    _keyState = TouchStatus.TouchUp;
                }
            }

            ProcessInput(_keyState);
        }
    }

    private void ResetInput()
    {
        lastSendAngle = -999;
        lastSendSec = -999;
        touchState = TouchStatus.TouchUp;
    }

    void Move()
    {
        oPLAYER_MOVE info = new oPLAYER_MOVE();
        info.Angle = curAngle * Mathf.Deg2Rad;
        Vector3 pos = CharactorManager.Instance.LocalPlayer.transform.position;
        info.Pos = new oVector3() { X = pos.x, Y = pos.y, Z = pos.z };
        SendProto<oPLAYER_MOVE>(eCPID.PlayerMove, info);

        //Debug.LogError("Send angle = " + curAngle);

        lastSendSec = Time.unscaledTime;
        lastSendAngle = curAngle;
#if UNITY_EDITOR
#endif
    }

    void Stop()
    {
        oPLAYER_STOP info = new oPLAYER_STOP();
        info.Angle = curAngle * Mathf.Deg2Rad;
        Vector3 pos = CharactorManager.Instance.LocalPlayer.transform.position;
        info.Pos = new oVector3() { X = pos.x, Y = pos.y, Z = pos.z };
        SendProto<oPLAYER_STOP>(eCPID.PlayerStop, info);
        //Debug.LogError("Move Stop " + Time.unscaledTime);

        ResetInput();
    }

    void ProcessInput(TouchStatus _keyState)
    {
        bool touchChanged = SetTouchStatus(_keyState);

        if(touchState == TouchStatus.TouchDown)
        {
            CheckSend(lastSendAngle, touchChanged);
        }
    }

    bool CheckSend(float preAngle, in bool send)
    {
        float delta = curAngle - preAngle;
        if (delta < 0)
        {
            delta += 360;
        }
        if (delta > touchThreadhold)
        {
            Move();
            return true;
        }
        return false;
    }

    bool SetTouchStatus(TouchStatus state)
    {
        if(state == touchState)
        {
            return false;
        }
        preTouchState = touchState;
        touchState = state;
        if (preTouchState == TouchStatus.TouchDown && touchState == TouchStatus.TouchUp)
        {
            Stop();
        }
        return true;
    }
}
