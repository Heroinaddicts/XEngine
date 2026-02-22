#ifndef __SkillManager_h__
#define __SkillManager_h__

#include "Header.h"

class SkillManager : public ISkillManager, public Api::iTimer {
public:
    virtual ~SkillManager() {}
    
private:
    // Í¨¹ý ISkillManager ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    void SkillCast(IGameObject* const cast, const UInt64 skillGuid, const Int64 skillConfigId, const float angle, const Vector2& targetPos, const Vector2& placePos, const UInt64 targetGuid) override;

    static void OnCharacterDockerLoaded(ICharacterDocker* const docker, IGameObject* const gameObject);
    static void OnCharacterLoaded(ICharacterDocker* const docker, IGameObject* const character);
    static void OnCharacterCreated(ICharacterDocker* const docker, IGameObject* const character);
    static void OnCharacterRelease(ICharacterDocker* const docker, IGameObject* const character);

    static void OnFsmEvent(const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState state);

    static bool OnActionJudge(const UInt64 guid, const AutoConfig::Enum::eAction action, const Int64 actionConfigID, const Vector2& position, const float angle);
    static void OnRunAction(const UInt64 guid, const AutoConfig::Enum::eAction action, const Int64 actionConfigID, const Vector2& position, const float angle);
    static bool OnEnterMovingJudge(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& position, const float angle);

    static void OnConfigureSkillSlot(IPlayer* const player, const UInt64 account, const oConfigureSkillSlot& pb);
    static void OnSkillCastReq(IPlayer* const player, const UInt64 account, const oSkillCastReq& pb);

    static IGameObject* GetSkillGameObject(const UInt64 characterId, const Int64 skillConfigId);
    static void SaveSkill(IGameObject* skillGo);
};

#endif //__SkillManager_h__
