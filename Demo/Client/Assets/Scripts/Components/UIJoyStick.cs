using System;
using UnityEngine;
using UnityEngine.EventSystems;
using XEngine;
using XUtils;
using static XEngine.XApi.ITimer;
using SkillVisual;
using static GameUnitApi;
using GameUnit;


public class UIJoyStick : MonoBehaviour, IDragHandler, IBeginDragHandler, IEndDragHandler
{
    private const float Epsilon = 1e-6f;
    public enum eJoyStickEvent
    {
        StickDown = 0,
        StickSliding = 1,
        StickUp = 2
    }

    public const int INVALID_STICK_ANGLE = -1;

    static XEventPool<eJoyStickEvent> s_Events = new XEventPool<eJoyStickEvent>();
    public static void RegistJoyStickEvent(in eJoyStickEvent ev, Action<float> callback)
    {
        s_Events.Regist(ev, callback);
    }

    public GameObject _JoyStickBG;
    public GameObject _JoyStick;
    public float _MaxRadius = 200f;   // 最大滑动半径
    public float _DeadZoneRadius = 190f;//死区半径

    Vector2 _StartPosition;
    Vector2 _DragOrigin;

    bool _IsDragging = false;
    float _LastMoveAngle = -1;
    Vector2 _LastPosition;
    ICharacterManager _characterManager;



    public void Start()
    {
        _StartPosition = _JoyStick.transform.localPosition;
        _characterManager = Launcher.GetModule<ICharacterManager>();
    }

    public void OnBeginDrag(PointerEventData eventData)
    {
        _LastPosition = eventData.position;
        _JoyStick.gameObject.GetComponent<RectTransform>().anchoredPosition = Vector2.zero;
        _IsDragging = false;
        // if (enableSkillVisual && _skillVisual == null)//创建技能可视化
        // {
        //     CreateSkillVisual();
        // }
    }

    void OnTime(in eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)
    {
        if (eState.BEAT == state)
        {
            s_Events.Trigger(eJoyStickEvent.StickSliding, _LastMoveAngle);
        }
    }

    public void OnDrag(PointerEventData eventData)
    {
        Vector2 localPoint = eventData.position - _LastPosition;
        if (localPoint.magnitude < _DeadZoneRadius)
        {
            localPoint = Vector2.zero;
            return;
        }
        localPoint = Vector2.ClampMagnitude(localPoint, _MaxRadius);
        _JoyStick.gameObject.GetComponent<RectTransform>().anchoredPosition = localPoint;
        Vector2 dir = localPoint - Vector2.zero;//方向

        if (dir != Vector2.zero)
        {
            float angle = (Mathf.Atan2(dir.y, dir.x) * Mathf.Rad2Deg + 360f) % 360f; // 即 +90°
            if (Math.Abs(angle - _LastMoveAngle) < Epsilon)
                return;

            _LastMoveAngle = angle;
            //Debug.Log("Joystick angle: " + _LastMoveAngle);

            if (false == _IsDragging)
            {
                s_Events.Trigger(eJoyStickEvent.StickDown, _LastMoveAngle);
                _IsDragging = true;
                s_Events.Trigger(eJoyStickEvent.StickSliding, _LastMoveAngle);
            }
            else
            {
                if (!XApi.TimerApi.IsTimerExists(OnTime, this))
                {
                    XApi.TimerApi.StartTimer(OnTime, this, 0, 100, 1, 100);
                }
            }

        }
    }

    public void OnEndDrag(PointerEventData eventData)
    {
        XApi.TimerApi.StopTimer(OnTime, this);

        _JoyStick.gameObject.GetComponent<RectTransform>().anchoredPosition = Vector2.zero;
        if (_IsDragging)
            s_Events.Trigger(eJoyStickEvent.StickUp, _LastMoveAngle);
        _LastMoveAngle = INVALID_STICK_ANGLE;
        _IsDragging = false;
    }

#if UNITY_EDITOR
    bool _KeyboardDragging = false;
    float _LastKeyboardAngle = INVALID_STICK_ANGLE;
    void Update()
    {
        Vector2 dir = Vector2.zero;

        if (Input.GetKey(KeyCode.W)) dir.y += 1f;
        if (Input.GetKey(KeyCode.S)) dir.y -= 1f;
        if (Input.GetKey(KeyCode.A)) dir.x -= 1f;
        if (Input.GetKey(KeyCode.D)) dir.x += 1f;

        if (dir.sqrMagnitude > 0f)
        {
            dir.Normalize();

            float angle = (Mathf.Atan2(dir.y, dir.x) * Mathf.Rad2Deg + 360f) % 360f;

            // 首次按下
            if (!_KeyboardDragging)
            {
                _KeyboardDragging = true;
                _LastKeyboardAngle = angle;

                s_Events.Trigger(eJoyStickEvent.StickDown, angle);
                s_Events.Trigger(eJoyStickEvent.StickSliding, angle);
            }
            else
            {
                // 角度变化才触发
                if (Math.Abs(angle - _LastKeyboardAngle) > Epsilon)
                {
                    _LastKeyboardAngle = angle;
                    s_Events.Trigger(eJoyStickEvent.StickSliding, angle);
                }
            }
        }
        else
        {
            // 松开所有 WASD
            if (_KeyboardDragging)
            {
                s_Events.Trigger(eJoyStickEvent.StickUp, _LastKeyboardAngle);
                _KeyboardDragging = false;
                _LastKeyboardAngle = INVALID_STICK_ANGLE;
            }
        }
    }
#endif//UNITY_EDITOR
}
