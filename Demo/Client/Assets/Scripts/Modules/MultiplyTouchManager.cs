using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.EventSystems;
using XEngine;
using XGame;
using XUtils;

public interface IMultiplyToucher
{
    void SetUpdate(Action<IMultiplyToucher> action);
    void SetFixedUpdate(Action<IMultiplyToucher> action);
    void SetLateUpdate(Action<IMultiplyToucher> action);
    Vector2 GetTouchPosition();

    int TouchID { get; }
}
public interface IMultiplyTouchManager : IModule
{
    public enum eEventID
    {
        NewToucher = 0,
        ReleaseToucher = 1
    }

    void RegistEvent(eEventID ev, Action<IMultiplyToucher> action);
}

class MultiplyToucher : IMultiplyToucher
{
    public enum eTouchType
    {
        Touch,
        LeftMouse,
        RightMouse,
    }

    public readonly eTouchType _Type;
    public readonly int _TouchID;
    private Action<IMultiplyToucher> _Update, _FixedUpdate, _LateUpdate;

    public MultiplyToucher(in eTouchType type, in int touchId)
    {
        _Type = type;
        _TouchID = touchId;
    }

    private static bool GetTouchByTouchID(in int touchId, out Touch touch)
    {
        for (int i = 0; i < Input.touches.Length; i++)
        {
            if (Input.touches[i].fingerId == touchId)
            {
                touch = Input.touches[i];
                return true;
            }
        }
        touch = default(Touch);
        return false;
    }

    public Vector2 GetTouchPosition()
    {
        switch (_Type)
        {
            case eTouchType.Touch:
                {
                    Touch touch;
                    if (GetTouchByTouchID(_TouchID, out touch))
                    {
                        return touch.position;
                    }

                    return Vector2.zero;
                }
            case eTouchType.LeftMouse:
            case eTouchType.RightMouse:
                return Input.mousePosition;
            default:
                throw new NotImplementedException();
        }
    }

    public int TouchID => _TouchID;

    public void Update() => _Update?.Invoke(this);
    public void FixedUpdate() => _FixedUpdate?.Invoke(this);
    public void LateUpdate() => _LateUpdate?.Invoke(this);
    public void SetUpdate(Action<IMultiplyToucher> update) => _Update = update;
    public void SetFixedUpdate(Action<IMultiplyToucher> update) => _FixedUpdate = update;
    public void SetLateUpdate(Action<IMultiplyToucher> update) => _LateUpdate = update;
}
class MultiplyTouchManager : IMultiplyTouchManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IMultiplyTouchManager, MultiplyTouchManager>();
    }
    XEventPool<IMultiplyTouchManager.eEventID> _EventPool = new XEventPool<IMultiplyTouchManager.eEventID>();
#if UNITY_ANDROID || UNITY_IOS
        Dictionary<int, MultiplyToucher> _MultiplyTouchers = new Dictionary<int, MultiplyToucher>(10);
        static List<int> s_ReleasedTouchers = new List<int>(10);
        static List<int> s_TotalSystemTouchs = new List<int>(20);
#endif //UNITY_ANDROID || UNITY_IOS

#if UNITY_EDITOR || UNITY_STANDALONE
    MultiplyToucher _LeftMouseButton = null;
    MultiplyToucher _RightMouseButton = null;
#endif //UNITY_EDITOR || UNITY_STANDALONE
    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        XApi.Trace($"设备支持的最大触摸点数: {Input.touchCount}  ： {Input.multiTouchEnabled}");
        Input.multiTouchEnabled = true;
        return true;
    }

    public void Release()
    {

    }

    bool IsFingerIdExists(int fingerId)
    {
        for (int i = 0; i < Input.touches.Length; i++)
        {
            if (Input.touches[i].fingerId == fingerId)
            {
                return true;
            }
        }
        return false;
    }
    bool IsPointerOverUIObject(Vector2 position)
    {
        PointerEventData eventData = new PointerEventData(EventSystem.current);
        eventData.position = position;
        var results = new List<RaycastResult>();
        EventSystem.current.RaycastAll(eventData, results);
        return results.Count > 0;
    }

    public void Update()
    {
#if UNITY_EDITOR || UNITY_STANDALONE
        if (Input.GetMouseButtonDown(0))
        {
            if (IsPointerOverUIObject(Input.mousePosition))
            {
                return;
            }
            _LeftMouseButton = new MultiplyToucher(MultiplyToucher.eTouchType.LeftMouse, -1);
            _EventPool.Trigger(IMultiplyTouchManager.eEventID.NewToucher, _LeftMouseButton);
            XApi.Debug($"Input.GetMouseButtonDown(0)");
        }

        _LeftMouseButton?.Update();
        if (Input.GetMouseButtonUp(0))
        {
            if (null != _LeftMouseButton)
                _EventPool.Trigger(IMultiplyTouchManager.eEventID.ReleaseToucher, _LeftMouseButton);
            _LeftMouseButton = null;
            //XApi.Debug($"Input.GetMouseButtonUp(0)");
        }

        if (Input.GetMouseButtonDown(1))
        {
            if (IsPointerOverUIObject(Input.mousePosition))
            {
                return;
            }
            _RightMouseButton = new MultiplyToucher(MultiplyToucher.eTouchType.RightMouse, -1);
            _EventPool.Trigger(IMultiplyTouchManager.eEventID.NewToucher, _RightMouseButton);
            XApi.Debug($"Input.GetMouseButtonDown(1)");
        }
        _RightMouseButton?.Update();
        if (Input.GetMouseButtonUp(1))
        {
            if (null != _RightMouseButton)
                _EventPool.Trigger(IMultiplyTouchManager.eEventID.ReleaseToucher, _RightMouseButton);
            _RightMouseButton = null;
            XApi.Debug($"Input.GetMouseButtonUp(1)");
        }
#endif //UNITY_EDITOR || UNITY_STANDALONE

#if UNITY_ANDROID || UNITY_IOS
            s_ReleasedTouchers.Clear();
            foreach (var mt in _MultiplyTouchers)
            {
                if (IsFingerIdExists(mt.Key))
                {
                    mt.Value.Update();
                }
                else
                {
                    s_ReleasedTouchers.Add(mt.Key);
                }
            }

            for (int i = 0; i < s_ReleasedTouchers.Count; i++)
            {
                _EventPool.Trigger(IMultiplyTouchManager.eEventID.ReleaseToucher, _MultiplyTouchers[s_ReleasedTouchers[i]]);
                _MultiplyTouchers.Remove(s_ReleasedTouchers[i]);
            }
            s_ReleasedTouchers.Clear();

            for (int i = 0; i < Input.touches.Length; i++)
            {
                if (!_MultiplyTouchers.ContainsKey(Input.touches[i].fingerId))
                {
                    if (IsPointerOverUIObject(Input.touches[i].position))
                    {
                        continue;
                    }
                    XApi.Trace($"New touch {Input.touches[i].fingerId} does not exist");
                    MultiplyToucher toucher = new MultiplyToucher(MultiplyToucher.eTouchType.Touch, Input.touches[i].fingerId);
                    _EventPool.Trigger(IMultiplyTouchManager.eEventID.NewToucher, toucher);
                    _MultiplyTouchers[Input.touches[i].fingerId] = toucher;
                }
            }
#endif //UNITY_ANDROID || UNITY_IOS
    }

    public void FixedUpdate()
    {
#if UNITY_EDITOR || UNITY_STANDALONE
        _LeftMouseButton?.FixedUpdate();
        _RightMouseButton?.FixedUpdate();
#endif //UNITY_EDITOR || UNITY_STANDALONE

#if UNITY_ANDROID || UNITY_IOS
            foreach (var kv in _MultiplyTouchers)
            {
                kv.Value.FixedUpdate();
            }
#endif //UNITY_ANDROID || UNITY_IOS
    }

    public void LateUpdate()
    {
#if UNITY_EDITOR || UNITY_STANDALONE
        _LeftMouseButton?.LateUpdate();
        _RightMouseButton?.LateUpdate();
#endif //UNITY_EDITOR || UNITY_STANDALONE

#if UNITY_ANDROID || UNITY_IOS
            foreach (var kv in _MultiplyTouchers)
            {
                kv.Value.LateUpdate();
            }
#endif //UNITY_ANDROID || UNITY_IOS
    }

    public void RegistEvent(IMultiplyTouchManager.eEventID ev, Action<IMultiplyToucher> action)
    {
        _EventPool.Regist(ev, action);
    }
}
