#include "SkillDamage.h"

static SkillDamage* s_Self = nullptr;

static std::vector<UInt64> s_BroadcastTemp;

bool SkillDamage::AddDamage(IGameObject* const damageDealer, IGameObject* const damageReceiver, const float damageAmount, const AutoConfig::Enum::eDamageType type)
{
    if (!damageDealer->Is(eGameObjectType::Entity))
    {
        ErrorLog(g_Engine, "GameObject Is Not Entity");
        return false;
    }

    if (damageAmount < 0)
    {
        return false;
    }

    if (damageReceiver->GetInt32(Entity::InvincibleRefInt32) > 0)
    {
        return false;
    }

    if (damageReceiver->GetFloat(Entity::HpFloat) <= 0)
    {
        return false;
    }

    float atk = damageDealer->GetFloat(Entity::AtkFloat);

    bool isCritical = false;

    UInt64 dealerGuid = damageDealer->Guid();
    if (damageDealer->Is(eGameObjectType::Bullet))
    {
        dealerGuid = damageDealer->GetUInt64(Bullet::HostGuidUInt64);
    }

    float baseCritical = damageDealer->GetFloat(Entity::CriticalFloat);

    isCritical = SafeTools::Rand() % 100 < baseCritical * 100;


    float criticalDamageAmount = isCritical ? 1 + damageDealer->GetFloat(Entity::CriticalDamageFloat) : 1;

    float physicalDamageReduction = (damageReceiver->GetFloat(Entity::DefendFloat) - (damageDealer->GetFloat(Entity::PhysicalPierceFloat) + damageDealer->GetFloat(Entity::PhysicalPierceRateFloat) * damageReceiver->GetFloat(Entity::DefendFloat))) / (AutoConfig::Const::DamageCalculate::Physic + damageReceiver->GetFloat(Entity::DefendFloat));
    float elementDamageReduction = (damageReceiver->GetFloat(Entity::MagicDefendFloat) - (damageDealer->GetFloat(Entity::MagicPierceFloat) + damageDealer->GetFloat(Entity::MagicPierceRateFloat) * damageReceiver->GetFloat(Entity::MagicDefendFloat))) / (AutoConfig::Const::DamageCalculate::Fire + damageReceiver->GetFloat(Entity::MagicDefendFloat));
    float damageReduction = damageReceiver->GetFloat(Entity::DamageReductionFloat);

    float resultDamage = 0;
    switch (type)
    {
    case AutoConfig::Enum::eDamageType::Truth:
    {
        resultDamage = atk * damageAmount * criticalDamageAmount * (1 - damageReduction);
        break;
    }
    case AutoConfig::Enum::eDamageType::Physic:
    {
        resultDamage = atk * damageAmount * criticalDamageAmount * (1 - physicalDamageReduction) * (1 - damageReduction);
        break;
    }
    case AutoConfig::Enum::eDamageType::Fire:
    {
        resultDamage = atk * damageAmount * criticalDamageAmount * (1 - elementDamageReduction) * (1 - damageReduction);
        break;
    }
    }

    damageReceiver->SetUInt64(Entity::WhoAttkMeUInt64, dealerGuid);

    float leftHp = damageReceiver->GetFloat(Entity::HpFloat) - resultDamage;
    damageReceiver->SetFloat(Entity::HpFloat, leftHp);

    oSkillDamagePush push;
    push.set_targetguid(damageReceiver->Guid());
    push.set_iscritical(isCritical);
    push.set_damagenum(std::ceil(resultDamage));
    push.set_castguid(damageDealer->Guid());
    if (damageDealer->Is(eGameObjectType::Bullet))
    {
        push.set_castguid(damageDealer->GetUInt64(Bullet::HostGuidUInt64));
    }

    s_BroadcastTemp.clear();

    if (damageReceiver->Is(eGameObjectType::Character)) {
        s_BroadcastTemp.push_back(damageReceiver->GetUInt64(Character::AccountUInt64));
    }

    IScene* scene = g_SceneManager->QuerySceneByEntityGuid(damageReceiver->Guid());
    if (nullptr == scene) {
        ErrorLog(g_Engine, "can not find any scene for game object %llu", damageReceiver->Guid());
        return false;
    }

    scene->GetVisual(damageReceiver->Guid(),
        [&](const std::unordered_set<UInt64>& visual) {
            for (auto visualIt = visual.begin(); visualIt != visual.end(); visualIt++) {
                IGameObject* g = g_GameWorld->QueryGameObject(*visualIt);
                if (g && g->Is(eGameObjectType::Character)) {
                    s_BroadcastTemp.push_back(g->GetUInt64(Character::AccountUInt64));
                }
            }
        }
    );

    g_PlayerManager->Broadcast(s_BroadcastTemp, eServerID::SkillDamagePush, push);

    return true;
}

bool SkillDamage::Initialize(Api::iEngine* const engine) {

    s_BroadcastTemp.reserve(2048);
    return true;
}

bool SkillDamage::Launch(Api::iEngine* const engine) {
    GetModule(g_Engine, g_PlayerManager, PlayerManager);
    GetModule(g_Engine, g_SceneManager, SceneManager);

    return true;
}

bool SkillDamage::Destroy(Api::iEngine* const engine) {
    return true;
}
