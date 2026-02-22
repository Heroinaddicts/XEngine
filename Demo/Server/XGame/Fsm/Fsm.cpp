#include "Fsm.h"

Api::iEngine* g_Engine = nullptr;
static Fsm* s_Self = nullptr;

static XPool<Fsm::StateMachine> s_StateMachinePool;

bool Fsm::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    s_Self = this;
    return true;
}

bool Fsm::Launch(Api::iEngine* const engine) {
    return true;
}

bool Fsm::Destroy(Api::iEngine* const engine) {
    return true;
}

void Fsm::RegistActionJudge(const UInt64 guid, const AutoConfig::Enum::eAction action, const fActionJudge& callback, const char* file, const int line) {
    auto itor = _StateMachineMap.find(guid);
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Can not find State Machine for %llu", guid);
        XASSERT(false, "wtf");
        return;
    }

    auto it = itor->second->_ActionJugdes.find(action);
    if (itor->second->_ActionJugdes.end() == it) {
        it = itor->second->_ActionJugdes.insert({ action, std::unordered_set<IFsm::fActionJudge>() }).first;
    }

    it->second.insert(callback);
}

void Fsm::RegistRunAction(const UInt64 guid, const AutoConfig::Enum::eAction action, const fRunAction& callback, const char* file, const int line) {
    auto itor = _StateMachineMap.find(guid);
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Can not find State Machine for %llu", guid);
        XASSERT(false, "wtf");
        return;
    }

    auto it = itor->second->_Actions.find(action);
    if (itor->second->_Actions.end() == it) {
        it = itor->second->_Actions.insert({ action, std::unordered_set<IFsm::fRunAction>() }).first;
    }

    it->second.insert(callback);
}

bool Fsm::RunAction(const UInt64 guid, const AutoConfig::Enum::eAction action, const Int64 actionConfigID, const Vector2& position, const float angle, const bool force) {
    auto itor = _StateMachineMap.find(guid);
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Can not find State Machine for %llu", guid);
        XASSERT(false, "wtf");
        return false;
    }

    if (false == force) {
        auto it = itor->second->_ActionJugdes.find(action);
        if (it != itor->second->_ActionJugdes.end()) {
            for (auto i = it->second.begin(); i != it->second.end(); i++) {
                if (false == (*i)(guid, action, actionConfigID, position, angle)) {
                    return false;
                }
            }
        }
    }

    {
        auto it = itor->second->_Actions.find(action);
        if (it != itor->second->_Actions.end()) {
            for (auto i = it->second.begin(); i != it->second.end(); i++) {
                (*i)(guid, action, actionConfigID, position, angle);
            }
        }
    }

    return true;
}

void Fsm::RegistFsmEvent(const fFsmEvent fun) {
    _FsmEvents.insert(fun);
}

AutoConfig::Enum::eState Fsm::GetState(const UInt64 guid) {
    auto itor = _StateMachineMap.find(guid);
    if (_StateMachineMap.end() == itor) {
        return AutoConfig::Enum::eState::Idle;
    }

    return itor->second->_State;
}

void Fsm::CreateFsm(const UInt64 guid, const AutoConfig::Enum::eState state) {
    if (_StateMachineMap.find(guid) != _StateMachineMap.end()) {
        ErrorLog(g_Engine, "Double Create State Machine for %llu", guid);
        XASSERT(false, "wtf");
        return;
    }

    StateMachine* const machine = XPOOL_CREATE(s_StateMachinePool, guid);
    machine->_State = state;
    _StateMachineMap.insert({ guid, machine });
    for (auto it = _FsmEvents.begin(); it != _FsmEvents.end(); it++) {
        (*it)(eFsmEvent::FsmCreated, guid, state);
    }
}

void Fsm::ReleaseFsm(const UInt64 guid) {
    auto itor = _StateMachineMap.find(guid);
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Can not find State Machine for %llu", guid);
        XASSERT(false, "wtf");
        return;
    }

    //     auto it = itor->second->_Existes.find(itor->second->_State);
    //     if (itor->second->_Existes.end() != it) {
    //         for (auto i = it->second.begin(); i != it->second.end(); i++) {
    //             (*i)(guid, itor->second->_State, Vector2(0, 0), 0);
    //         }
    //     }

    for (auto it = _FsmEvents.begin(); it != _FsmEvents.end(); it++) {
        (*it)(eFsmEvent::FsmReleased, guid, itor->second->_State);
    }

    XPOOL_RELEASE(s_StateMachinePool, itor->second);
    _StateMachineMap.erase(itor);
}

bool Fsm::EnterState(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& position, const float angle, const bool trigger) {
    auto itor = _StateMachineMap.find(guid);
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Can not find State Machine for %llu", guid);
        XASSERT(false, "wtf");
        return false;
    }

    StateMachine* const machine = itor->second;
    if (state == machine->_State) {
        for (auto it = machine->_Changed.begin(); it != machine->_Changed.end(); it++) {
            (*it)(guid, state, position, angle);
        }

        auto it = machine->_Enters.find(state);
        if (machine->_Enters.end() != it) {
            for (auto i = it->second.begin(); i != it->second.end(); i++) {
                (*i)(guid, state, position, angle);
            }
        }

        return true;
    }

    {
        auto it = machine->_ExitJugdes.find(machine->_State);
        if (machine->_ExitJugdes.end() != it) {
            for (auto i = it->second.begin(); i != it->second.end(); i++) {
                if (!(*i)(guid, state, position, angle)) {
                    return false;
                }
            }
        }
    }

    {
        auto it = machine->_EnterJugdes.find(state);
        if (machine->_EnterJugdes.end() != it) {
            for (auto i = it->second.begin(); i != it->second.end(); i++) {
                bool ret = (*i)(guid, state, position, angle);
                if (!ret) {
                    return false;
                }
            }
        }
    }

    {
        auto it = machine->_Existes.find(machine->_State);
        if (machine->_Existes.end() != it) {
            for (auto i = it->second.begin(); i != it->second.end(); i++) {
                (*i)(guid, state, position, angle);
            }
        }
    }

    machine->_State = state;

    {
        if (trigger)
            for (auto it = machine->_Changed.begin(); it != machine->_Changed.end(); it++) {
                (*it)(guid, state, position, angle);
            }

        auto it = machine->_Enters.find(state);
        if (machine->_Enters.end() != it) {
            for (auto i = it->second.begin(); i != it->second.end(); i++) {
                (*i)(guid, state, position, angle);
            }
        }
    }
    return true;
}

void Fsm::ForceEnterState(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& position, const float angle) {
    auto itor = _StateMachineMap.find(guid);
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Can not find State Machine for %llu", guid);
        XASSERT(false, "wtf");
        return;
    }

    StateMachine* const machine = itor->second;
    if (state != machine->_State) {
        {
            auto it = machine->_Existes.find(machine->_State);
            if (machine->_Existes.end() != it) {
                for (auto i = it->second.begin(); i != it->second.end(); i++) {
                    (*i)(guid, machine->_State, position, angle);
                }
            }
        }
    }
    machine->_State = state;
    for (auto it = machine->_Changed.begin(); it != machine->_Changed.end(); it++) {
        (*it)(guid, state, position, angle);
    }

    {
        auto it = machine->_Enters.find(state);
        if (machine->_Enters.end() != it) {
            for (auto i = it->second.begin(); i != it->second.end(); i++) {
                (*i)(guid, state, position, angle);
            }
        }
    }
}

void Fsm::RegistEnterJudge(const UInt64 guid, const AutoConfig::Enum::eState state, const IFsm::fEnterJudge& callback, const char* file, const int line) {
    auto itor = _StateMachineMap.find(guid);
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Where is %lld 's state machine, please create for this guid first", guid);
        return;
    }

    StateMachine* const machine = itor->second;

    auto it = machine->_EnterJugdes.find(state);
    if (machine->_EnterJugdes.end() == it) {
        it = machine->_EnterJugdes.insert({ state, std::unordered_set<IFsm::fEnterJudge>() }).first;
    }

    it->second.insert(callback);
}

void Fsm::RegistExitJudge(const UInt64 guid, const AutoConfig::Enum::eState state, const IFsm::fExitJudge& callback, const char* file, const int line) {
    auto itor = _StateMachineMap.find(guid);
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Where is %lld 's state machine, please create for this guid first", guid);
        return;
    }

    StateMachine* const machine = itor->second;
    auto it = machine->_ExitJugdes.find(state);
    if (machine->_ExitJugdes.end() == it) {
        it = machine->_ExitJugdes.insert({ state, std::unordered_set<IFsm::fExitJudge>() }).first;
    }

    it->second.insert(callback);
}

void Fsm::RegistStateChanged(const UInt64 guid, const fState& changed) {
    auto itor = _StateMachineMap.find(guid);
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Where is %lld 's state machine, please create for this guid first", guid);
        return;
    }

    StateMachine* const machine = itor->second;
    machine->_Changed.insert(changed);
}

void Fsm::RegistStateEnter(const UInt64 guid, const AutoConfig::Enum::eState state, const IFsm::fState& enter, const char* file, const int line) {
    auto itor = _StateMachineMap.find(guid);
    XASSERT(_StateMachineMap.end() != itor, "wtf");
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Can not find state machine of %llu", guid);
        return;
    }

    auto it = itor->second->_Enters.find(state);
    if (itor->second->_Enters.end() == it)
        it = itor->second->_Enters.insert({ state, std::unordered_set<IFsm::fState>() }).first;

    it->second.insert(enter);
}

void Fsm::RegistStateExit(const UInt64 guid, const AutoConfig::Enum::eState state, const IFsm::fState& exit, const char* file, const int line) {
    auto itor = _StateMachineMap.find(guid);
    XASSERT(_StateMachineMap.end() != itor, "wtf");
    if (_StateMachineMap.end() == itor) {
        ErrorLog(g_Engine, "Can not find state machine of %llu", guid);
        return;
    }

    auto it = itor->second->_Existes.find(state);
    if (itor->second->_Existes.end() == it)
        it = itor->second->_Existes.insert({ state, std::unordered_set<IFsm::fState>() }).first;

    it->second.insert(exit);
}
