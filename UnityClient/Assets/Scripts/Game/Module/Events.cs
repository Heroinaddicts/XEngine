using System.Collections.Generic;
using TCore.Api;
using UnityEngine;

public abstract class IEvents : iModule
{
    protected IEvents(in string name) : base(name) { }
}

class Events : IEvents
{

    public delegate void event_delegate<T>(in T ev);

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void ModuleRegister()
    {
        Module.Registe(typeof(Events), new Events());
    }

    Events() : base("Events") { }
    public override bool Initialize()
    {
        return true;
    }

    public override bool Launch()
    {
        return true;
    }

    public override void Release()
    {

    }

    public abstract class iHandle
    {
        public bool _is_live = true;
        public abstract void on(in object e);
    }

    class Handle<T> : iHandle
    {
        public readonly event_delegate<T> _caller;

        public Handle(in event_delegate<T> caller) { _caller = caller; }

        public override void on(in object e)
        {
            _caller((T)e);
        }
    }

    IDictionary<Ev.eID, IList<iHandle>> _events = new Dictionary<Ev.eID, IList<iHandle>>();
    public void Register<T>(in Ev.eID id, in event_delegate<T> e)
    {
        IList<iHandle> handles;
        if (!_events.TryGetValue(id, out handles))
        {
            handles = new List<iHandle>();
            _events.Add(id, handles);
        }

        foreach (iHandle h in handles)
        {
            if (e == ((Handle<T>)h)._caller)
            {
                if (h._is_live)
                {
                    Debug.LogError("double register event : " + e.ToString());
                }
                else
                {
                    h._is_live = true;
                }
                return;
            }
        }

        handles.Add(new Handle<T>(e));
    }

    public void Cancellation<T>(in Ev.eID id, in event_delegate<T> e)
    {
        IList<iHandle> handles;
        if (_events.TryGetValue(id, out handles))
        {
            foreach (iHandle h in handles)
            {
                if (e == ((Handle<T>)h)._caller)
                {
                    h._is_live = false;
                    return;
                }
            }
        }
    }

    public void Trigger<T>(in Ev.eID id, in T body)
    {
        IList<iHandle> handles;
        if (_events.TryGetValue(id, out handles))
        {
            foreach (iHandle h in handles)
            {
                if (h._is_live)
                {
                    h.on(body);
                }
            }
        }
    }
}
