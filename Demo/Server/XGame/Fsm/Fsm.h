#ifndef __Fsm_h__
#define __Fsm_h__

#include "Header.h"

class Fsm : public IFsm {
public:
    virtual ~Fsm() {}

    // 通过 IFsm 继承
    void RegistFsmEvent(const fFsmEvent fun) override;
    void CreateFsm(const UInt64 guid, const AutoConfig::Enum::eState state) override;
    void ReleaseFsm(const UInt64 guid) override;

    // 通过 IFsm 继承
    void RegistEnterJudge(const UInt64, const AutoConfig::Enum::eState, const IFsm::fEnterJudge& callback, const char* file, const int line) override;
    void RegistExitJudge(const UInt64, const AutoConfig::Enum::eState, const IFsm::fExitJudge& callback, const char* file, const int line) override;

    void RegistStateChanged(const UInt64 guid, const fState& changed) override;

    void RegistStateEnter(const UInt64, const AutoConfig::Enum::eState state, const IFsm::fState& enter, const char* file, const int line) override;
    void RegistStateExit(const UInt64, const AutoConfig::Enum::eState state, const IFsm::fState& exit, const char* file, const int line) override;

    // 通过 IFsm 继承
    AutoConfig::Enum::eState GetState(const UInt64 guid) override;
    bool EnterState(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& position, const float angle, const bool trigger) override;
    void ForceEnterState(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& position, const float angle) override;

    // 通过 IFsm 继承
    void RegistActionJudge(const UInt64 guid, const AutoConfig::Enum::eAction action, const fActionJudge& callback, const char* file, const int line) override;
    void RegistRunAction(const UInt64 guid, const AutoConfig::Enum::eAction action, const fRunAction& callback, const char* file, const int line) override;
    bool RunAction(const UInt64 guid, const AutoConfig::Enum::eAction action, const Int64 actionConfigID, const Vector2& position, const float angle, const bool force) override;

    struct StateMachine {
        const UInt64 _Guid;

        StateMachine(const UInt64 guid) : _Guid(guid), _State(AutoConfig::Enum::eState::Idle), _Action(AutoConfig::Enum::eAction::None) {}

        AutoConfig::Enum::eState _State;
        AutoConfig::Enum::eAction _Action;

        std::unordered_set<IFsm::fState> _Changed;

        std::unordered_map<AutoConfig::Enum::eState, std::unordered_set<IFsm::fEnterJudge>> _EnterJugdes;
        std::unordered_map<AutoConfig::Enum::eState, std::unordered_set<IFsm::fExitJudge>> _ExitJugdes;

        std::unordered_map<AutoConfig::Enum::eState, std::unordered_set<IFsm::fState>> _Enters;
        std::unordered_map<AutoConfig::Enum::eState, std::unordered_set<IFsm::fState>> _Existes;

        std::unordered_map<AutoConfig::Enum::eAction, std::unordered_set<IFsm::fActionJudge>> _ActionJugdes;
        std::unordered_map<AutoConfig::Enum::eAction, std::unordered_set<IFsm::fRunAction>> _Actions;
    };

private:
    // 通过 IFsm 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;
private:
    std::unordered_map<UInt64, StateMachine*> _StateMachineMap;
    std::unordered_set<fFsmEvent> _FsmEvents;
};

#endif //__Fsm_h__
