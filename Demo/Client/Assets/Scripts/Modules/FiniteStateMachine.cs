using AutoConfig;
using System;
using System.Collections.Generic;
using TMPro;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using XGame;


public interface IFiniteStateMachine : IModule
{
    AutoConfig.AutoEnum.eState GetState(in UInt64 guid);
    void ReleaseFsm(in UInt64 guid);

    public delegate bool fOnEnterJudge(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos);
    void RegistEnterJudge(in AutoConfig.AutoEnum.eState state, in fOnEnterJudge judge);

    public delegate bool fOnExitJudge(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos);
    void RegistExitJudge(in AutoConfig.AutoEnum.eState state, in fOnExitJudge judge);

    public delegate void fOnEnter(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos);
    void RegistStateEnter(in AutoConfig.AutoEnum.eState state, in fOnEnter fun);

    public delegate void fOnExit(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos);
    void RegistStateExit(in AutoConfig.AutoEnum.eState state, in fOnExit fun);

    bool EnterState(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos, in bool force = false);

    public delegate bool fOnActionJudge(in UInt64 guid, in AutoConfig.AutoEnum.eAction action, in Int64 skillConfigId, in float angle, in Vector2 pos);
    void RegistActionJudge(in AutoConfig.AutoEnum.eAction action, in fOnActionJudge fun);
    public delegate void fOnAction(in UInt64 guid, in AutoConfig.AutoEnum.eAction action, in Int64 skillConfigId, in float angle, in Vector2 pos);
    void RegistAction(in AutoConfig.AutoEnum.eAction action, in fOnAction fun);
    bool RunAction(in UInt64 guid, in AutoConfig.AutoEnum.eAction action, in Int64 skillConfigId, in float angle, in Vector2 pos, in bool force = false);
}

public class FiniteStateMachine : IFiniteStateMachine
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IFiniteStateMachine, FiniteStateMachine>();
    }

    public bool Initialize()
    {
        return true;
    }

    IClient _Client;

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();

        _Client.RegistProtobuf<oStateChangePush>(eServerID.StateChangePush, OnStateChangePush);
        return true;
    }

    public void Release()
    {

    }

    class SA
    {
        public AutoConfig.AutoEnum.eState _State;
        public AutoConfig.AutoEnum.eAction _Action;

        public SA()
        {
            _State = AutoEnum.eState.Idle;
            _Action = AutoEnum.eAction.None;
        }
    }

    Dictionary<AutoConfig.AutoEnum.eState, HashSet<IFiniteStateMachine.fOnEnterJudge>> _EnterJudges = new Dictionary<AutoEnum.eState, HashSet<IFiniteStateMachine.fOnEnterJudge>>();
    Dictionary<AutoConfig.AutoEnum.eState, HashSet<IFiniteStateMachine.fOnExitJudge>> _ExitJudges = new Dictionary<AutoEnum.eState, HashSet<IFiniteStateMachine.fOnExitJudge>>();
    Dictionary<AutoConfig.AutoEnum.eState, HashSet<IFiniteStateMachine.fOnEnter>> _Enters = new Dictionary<AutoEnum.eState, HashSet<IFiniteStateMachine.fOnEnter>>();
    Dictionary<AutoConfig.AutoEnum.eState, HashSet<IFiniteStateMachine.fOnExit>> _Exists = new Dictionary<AutoEnum.eState, HashSet<IFiniteStateMachine.fOnExit>>();

    Dictionary<AutoConfig.AutoEnum.eAction, HashSet<IFiniteStateMachine.fOnActionJudge>> _ActionJudges = new Dictionary<AutoEnum.eAction, HashSet<IFiniteStateMachine.fOnActionJudge>>();
    Dictionary<AutoConfig.AutoEnum.eAction, HashSet<IFiniteStateMachine.fOnAction>> _Actions = new Dictionary<AutoEnum.eAction, HashSet<IFiniteStateMachine.fOnAction>>();
    Dictionary<UInt64, SA> _States = new Dictionary<UInt64, SA>();

    void OnStateChangePush(oStateChangePush push)
    {
        if (null != GameUnitApi.QueryUnit(push.Guid))
        {
            EnterState(push.Guid, (AutoConfig.AutoEnum.eState)push.Change.State, push.Change.Angle, new Vector2(push.Change.Pos.X, push.Change.Pos.Y), true);
        }
    }

    public AutoConfig.AutoEnum.eState GetState(in UInt64 guid)
    {
        SA sa;
        if (_States.TryGetValue(guid, out sa))
        {
            return sa._State;
        }
        return AutoEnum.eState.Idle;
    }
    public void ReleaseFsm(in UInt64 guid)
    {
        _States.Remove(guid);
    }

    public void RegistEnterJudge(in AutoEnum.eState state, in IFiniteStateMachine.fOnEnterJudge judge)
    {
        HashSet<IFiniteStateMachine.fOnEnterJudge> funs;
        if (!_EnterJudges.TryGetValue(state, out funs))
        {
            funs = new HashSet<IFiniteStateMachine.fOnEnterJudge>();
            _EnterJudges.Add(state, funs);
        }

        funs.Add(judge);
    }

    public void RegistExitJudge(in AutoEnum.eState state, in IFiniteStateMachine.fOnExitJudge judge)
    {
        HashSet<IFiniteStateMachine.fOnExitJudge> funs;
        if (!_ExitJudges.TryGetValue(state, out funs))
        {
            funs = new HashSet<IFiniteStateMachine.fOnExitJudge>();
            _ExitJudges.Add(state, funs);
        }

        funs.Add(judge);
    }

    public void RegistStateEnter(in AutoEnum.eState state, in IFiniteStateMachine.fOnEnter fun)
    {
        HashSet<IFiniteStateMachine.fOnEnter> funs;
        if (!_Enters.TryGetValue(state, out funs))
        {
            funs = new HashSet<IFiniteStateMachine.fOnEnter>();
            _Enters.Add(state, funs);
        }

        funs.Add(fun);
    }

    public void RegistStateExit(in AutoEnum.eState state, in IFiniteStateMachine.fOnExit fun)
    {
        HashSet<IFiniteStateMachine.fOnExit> funs;
        if (!_Exists.TryGetValue(state, out funs))
        {
            funs = new HashSet<IFiniteStateMachine.fOnExit>();
            _Exists.Add(state, funs);
        }

        funs.Add(fun);
    }

    public bool EnterState(in UInt64 guid, in AutoEnum.eState state, in float angle, in Vector2 pos, in bool force)
    {
        SA sa;
        if (!_States.TryGetValue(guid, out sa))
        {
            sa = new SA();
            _States.Add(guid, sa);
        }

        if (false == force)
        {
            if (sa._State != state)
            {
                HashSet<IFiniteStateMachine.fOnExitJudge> exitJudges;
                if (_ExitJudges.TryGetValue(sa._State, out exitJudges))
                {
                    foreach (var f in exitJudges)
                    {
                        if (false == f(guid, sa._State, angle, pos))
                        {
                            return false;
                        }
                    }
                }
            }


            HashSet<IFiniteStateMachine.fOnEnterJudge> enterJudges;
            if (_EnterJudges.TryGetValue(state, out enterJudges))
            {
                foreach (var f in enterJudges)
                {
                    if (false == f(guid, state, angle, pos))
                    {
                        return false;
                    }
                }
            }
        }

        if (sa._State != state)
        {
            HashSet<IFiniteStateMachine.fOnExit> exitFuns;
            if (_Exists.TryGetValue(sa._State, out exitFuns))
            {
                foreach (var f in exitFuns)
                {
                    f?.Invoke(guid, sa._State, angle, pos);
                }
            }
        }

        sa._State = state;

        HashSet<IFiniteStateMachine.fOnEnter> enterFuns;
        if (_Enters.TryGetValue(state, out enterFuns))
        {
            foreach (var f in enterFuns)
            {
                f?.Invoke(guid, sa._State, angle, pos);
            }
        }

        return true;
    }

    public void RegistActionJudge(in AutoEnum.eAction action, in IFiniteStateMachine.fOnActionJudge fun)
    {
        HashSet<IFiniteStateMachine.fOnActionJudge> funs;
        if (!_ActionJudges.TryGetValue(action, out funs))
        {
            funs = new HashSet<IFiniteStateMachine.fOnActionJudge>();
            _ActionJudges.Add(action, funs);
        }

        funs.Add(fun);
    }

    public void RegistAction(in AutoEnum.eAction action, in IFiniteStateMachine.fOnAction fun)
    {
        HashSet<IFiniteStateMachine.fOnAction> funs;
        if (!_Actions.TryGetValue(action, out funs))
        {
            funs = new HashSet<IFiniteStateMachine.fOnAction>();
            _Actions.Add(action, funs);
        }

        funs.Add(fun);
    }

    public bool RunAction(in ulong guid, in AutoEnum.eAction action, in long skillConfigId, in float angle, in Vector2 pos, in bool force = false)
    {
        SA sa;
        if (!_States.TryGetValue(guid, out sa))
        {
            sa = new SA();
            _States.Add(guid, sa);
        }

        if (force == false)
        {
            HashSet<IFiniteStateMachine.fOnActionJudge> funs;
            if (_ActionJudges.TryGetValue(action, out funs))
            {
                foreach (var f in funs)
                {
                    if (false == f?.Invoke(guid, action, skillConfigId, angle, pos))
                    {
                        return false;
                    }
                }
            }
        }

        sa._Action = action;
        HashSet<IFiniteStateMachine.fOnAction> onActions;
        if (_Actions.TryGetValue(action, out onActions))
        {
            foreach (var f in onActions)
            {
                f?.Invoke(guid, sa._Action, skillConfigId, angle, pos);
            }
        }

        return true;
    }
}
