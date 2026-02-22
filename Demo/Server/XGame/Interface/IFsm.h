#ifndef __IFsm_h__
#define __IFsm_h__

#include "iModule.h"
#include "Vector2.h"
#include "AutoConfig.h"
UsingXEngine;

class IFsm : public Api::iModule {
public:
    virtual ~IFsm() {}

    enum class eFsmEvent {
        FsmCreated = 0,
        FsmReleased = 1
    };

    typedef void(*fFsmEvent)(const eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState);
    virtual void RegistFsmEvent(const fFsmEvent fun) = 0;

    virtual void CreateFsm(const UInt64 guid, const AutoConfig::Enum::eState state = AutoConfig::Enum::eState::Idle) = 0;
    virtual void ReleaseFsm(const UInt64 guid) = 0;

    typedef bool(*fEnterJudge)(const UInt64, const AutoConfig::Enum::eState, const Vector2&, const float);
    virtual void RegistEnterJudge(const UInt64 guid, const AutoConfig::Enum::eState, const fEnterJudge& callback, const char* file = __FILE__, const int line = __LINE__) = 0;
    typedef bool (*fExitJudge)(const UInt64, const AutoConfig::Enum::eState, const Vector2&, const float);
    virtual void RegistExitJudge(const UInt64 guid, const AutoConfig::Enum::eState, const fExitJudge& callback, const char* file = __FILE__, const int line = __LINE__) = 0;

    typedef void(*fState)(const UInt64, const AutoConfig::Enum::eState, const Vector2&, const float);
    virtual void RegistStateEnter(const UInt64 guid, const AutoConfig::Enum::eState state, const fState& enter, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual void RegistStateExit(const UInt64 guid, const AutoConfig::Enum::eState state, const fState& exit, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual void RegistStateChanged(const UInt64 guid, const fState& changed) = 0;

    virtual AutoConfig::Enum::eState GetState(const UInt64 guid) = 0;
    virtual bool EnterState(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& position, const float angle, const bool trigger = true) = 0;
    virtual void ForceEnterState(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& position, const float angle) = 0;

    typedef bool(*fActionJudge)(const UInt64 guid, const AutoConfig::Enum::eAction action, const Int64 actionConfigID, const Vector2& position, const float angle);
    virtual void RegistActionJudge(const UInt64 guid, const AutoConfig::Enum::eAction action, const fActionJudge& callback, const char* file = __FILE__, const int line = __LINE__) = 0;

    typedef void(*fRunAction)(const UInt64 guid, const AutoConfig::Enum::eAction action, const Int64 actionConfigID, const Vector2& position, const float angle);
    virtual void RegistRunAction(const UInt64 guid, const AutoConfig::Enum::eAction action, const fRunAction& callback, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual bool RunAction(const UInt64 guid, const AutoConfig::Enum::eAction action, const Int64 actionConfigID, const Vector2& position, const float angle, const bool force = false) = 0;
};

#endif //__IFsm_h__
