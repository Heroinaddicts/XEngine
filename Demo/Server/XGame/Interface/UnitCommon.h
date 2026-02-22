#ifndef __UnitCommon_h__
#define __UnitCommon_h__
#include "MultiSys.h"

#define UsingUnitCommon using namespace UnitCommon

#define INVALID_GAMEOBJECT_ID 0

class IGameObject;
namespace UnitCommon {
    enum eAttributeType {
        __UInt16__ = 0,
        __Int16__ = 1,
        __UInt32__ = 2,
        __Int32__ = 3,
        __UInt64__ = 4,
        __Int64__ = 5,
        __Float__ = 6,
        __String__ = 7,
        __Bool__ = 8,
        __Vector2__ = 9,
        __Vector3__ = 10,
        __Vector4__ = 11,

        __Unknown__
    };

    enum class eGameObjectType{
        Boss = 3,
        Buff = 4,
        Bullet = 5,
        Character = 6,
        Creature = 1,
        Entity = 0,
        Equip = 8,
        Item = 7,
        Launcher = 9,
        LootBox = 10,
        Monster = 2,
        Npc = 11,
        RefreshRule = 12,
        Skill = 13,
        
        __AllType__ = 14
    };

    namespace Boss {
        enum {
            SpeedFloat = 0, //
            SceneGUIDUInt64 = 1, //
            LastPublicSceneConfigIDInt64 = 2, //
            CopySceneGuidUInt64 = 3, //
            NicknameString = 4, //
            LevelInt64 = 5, //
            LastUpdatePositionTickUInt64 = 6, //
            WhoAttkMeUInt64 = 7, //
            AtkFloat = 8, //
            HpFloat = 9, //
            MaxHpFloat = 10, //
            CriticalFloat = 11, //
            CriticalDamageFloat = 12, //
            DefendFloat = 13, //
            MagicDefendFloat = 14, //
            BloodFloat = 15, //
            MagicAttackFloat = 16, //
            PhysicalPierceFloat = 17, //
            PhysicalPierceRateFloat = 18, //
            MagicPierceFloat = 19, //
            MagicPierceRateFloat = 20, //
            AffordFloat = 21, //
            DodgeFloat = 22, //
            ResilienceFloat = 23, //
            VisualFieldFloat = 24, //
            DamageReductionFloat = 25, //
            MpFloat = 26, //
            MaxMpFloat = 27, //
            AtkSpeedFloat = 28, //
            CantControllerRefInt32 = 29, //
            SilentRefInt32 = 30, //
            InvincibleRefInt32 = 31, //
            RestraintRefInt32 = 32, //
            ShiftRefInt32 = 33, //
            ExtraAddExpFloat = 34, //
            MoveSkillConfigIDInt64 = 35, //
            SkillAngleFloat = 36, //
            MoveSkillTargetGuidUInt64 = 37, //
            SkillTargetPosVector2 = 38, //
            DamagePriorityInt32 = 39, //
            LastSkillConfigIDInt64 = 40, //
            LastSkillUseTimeUInt64 = 41, //
            CastIsTrackOnCollisionBool = 42, //
            IsCantDamagedBool = 43, //
            TeamGuidUInt64 = 44, //
            MoveSpeedFloat = 45, //
            HpRecoverFloat = 46, //
            MpRecoverFloat = 47, //
            IsTeamLeaderBool = 48, //
            BattleStartTimeUInt64 = 49, //
            RefreshRuleGuidUInt64 = 50, //
            TargetUInt64 = 51, //
            BronPositionVector2 = 52, //
            IsPreviousSuccessBool = 53, //
            PreviousPositionVector2 = 54, //

            __AttributeCount__ = 55
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Boss, eGameObjectType::Monster, eGameObjectType::Creature, eGameObjectType::Entity,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 4;
    }
    namespace Buff {
        enum {
            CasterGuidUInt64 = 0, //
            HostGuidUInt64 = 1, //
            ChangeValueFloat = 2, //
            CreateTimeTickUInt64 = 3, //

            __AttributeCount__ = 4
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Buff,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 1;
    }
    namespace Bullet {
        enum {
            SpeedFloat = 0, //
            SceneGUIDUInt64 = 1, //
            LastPublicSceneConfigIDInt64 = 2, //
            CopySceneGuidUInt64 = 3, //
            NicknameString = 4, //
            LevelInt64 = 5, //
            LastUpdatePositionTickUInt64 = 6, //
            WhoAttkMeUInt64 = 7, //
            AtkFloat = 8, //
            HpFloat = 9, //
            MaxHpFloat = 10, //
            CriticalFloat = 11, //
            CriticalDamageFloat = 12, //
            DefendFloat = 13, //
            MagicDefendFloat = 14, //
            BloodFloat = 15, //
            MagicAttackFloat = 16, //
            PhysicalPierceFloat = 17, //
            PhysicalPierceRateFloat = 18, //
            MagicPierceFloat = 19, //
            MagicPierceRateFloat = 20, //
            AffordFloat = 21, //
            DodgeFloat = 22, //
            ResilienceFloat = 23, //
            VisualFieldFloat = 24, //
            DamageReductionFloat = 25, //
            MpFloat = 26, //
            MaxMpFloat = 27, //
            AtkSpeedFloat = 28, //
            CantControllerRefInt32 = 29, //
            SilentRefInt32 = 30, //
            InvincibleRefInt32 = 31, //
            RestraintRefInt32 = 32, //
            ShiftRefInt32 = 33, //
            ExtraAddExpFloat = 34, //
            MoveSkillConfigIDInt64 = 35, //
            SkillAngleFloat = 36, //
            MoveSkillTargetGuidUInt64 = 37, //
            SkillTargetPosVector2 = 38, //
            DamagePriorityInt32 = 39, //
            LastSkillConfigIDInt64 = 40, //
            LastSkillUseTimeUInt64 = 41, //
            CastIsTrackOnCollisionBool = 42, //
            IsCantDamagedBool = 43, //
            TeamGuidUInt64 = 44, //
            MoveSpeedFloat = 45, //
            HpRecoverFloat = 46, //
            MpRecoverFloat = 47, //
            IsTeamLeaderBool = 48, //
            BattleStartTimeUInt64 = 49, //
            HostGuidUInt64 = 50, //
            TargetGuidUInt64 = 51, //
            TargetPosXFloat = 52, //
            TargetPosYFloat = 53, //
            LastUpdateTimeUInt64 = 54, //
            DeltaTimeFloat = 55, //
            ControlPosVector2 = 56, //
            StartPosVector2 = 57, //
            DamageCountInt64 = 58, //
            HitUnitGuidUInt64 = 59, //

            __AttributeCount__ = 60
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Bullet, eGameObjectType::Entity,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 2;
    }
    namespace Character {
        enum {
            SpeedFloat = 0, //
            SceneGUIDUInt64 = 1, //
            LastPublicSceneConfigIDInt64 = 2, //
            CopySceneGuidUInt64 = 3, //
            NicknameString = 4, //
            LevelInt64 = 5, //
            LastUpdatePositionTickUInt64 = 6, //
            WhoAttkMeUInt64 = 7, //
            AtkFloat = 8, //
            HpFloat = 9, //
            MaxHpFloat = 10, //
            CriticalFloat = 11, //
            CriticalDamageFloat = 12, //
            DefendFloat = 13, //
            MagicDefendFloat = 14, //
            BloodFloat = 15, //
            MagicAttackFloat = 16, //
            PhysicalPierceFloat = 17, //
            PhysicalPierceRateFloat = 18, //
            MagicPierceFloat = 19, //
            MagicPierceRateFloat = 20, //
            AffordFloat = 21, //
            DodgeFloat = 22, //
            ResilienceFloat = 23, //
            VisualFieldFloat = 24, //
            DamageReductionFloat = 25, //
            MpFloat = 26, //
            MaxMpFloat = 27, //
            AtkSpeedFloat = 28, //
            CantControllerRefInt32 = 29, //
            SilentRefInt32 = 30, //
            InvincibleRefInt32 = 31, //
            RestraintRefInt32 = 32, //
            ShiftRefInt32 = 33, //
            ExtraAddExpFloat = 34, //
            MoveSkillConfigIDInt64 = 35, //
            SkillAngleFloat = 36, //
            MoveSkillTargetGuidUInt64 = 37, //
            SkillTargetPosVector2 = 38, //
            DamagePriorityInt32 = 39, //
            LastSkillConfigIDInt64 = 40, //
            LastSkillUseTimeUInt64 = 41, //
            CastIsTrackOnCollisionBool = 42, //
            IsCantDamagedBool = 43, //
            TeamGuidUInt64 = 44, //
            MoveSpeedFloat = 45, //
            HpRecoverFloat = 46, //
            MpRecoverFloat = 47, //
            IsTeamLeaderBool = 48, //
            BattleStartTimeUInt64 = 49, //
            AccountUInt64 = 50, //
            ExpInt64 = 51, //
            SlotClothsUInt64 = 52, //
            ClothsConfigInt64 = 53, //
            SlotPantsUInt64 = 54, //
            PantsConfigInt64 = 55, //
            SlotBeltUInt64 = 56, //
            BeltConfigInt64 = 57, //
            SlotShoesUInt64 = 58, //
            ShoesConfigInt64 = 59, //
            SlotJewelryUInt64 = 60, //
            JewelryConfigInt64 = 61, //
            SlotWeaponUInt64 = 62, //
            WeaponConfigInt64 = 63, //
            MoveTargetGuidUInt64 = 64, //

            __AttributeCount__ = 65
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Character, eGameObjectType::Creature, eGameObjectType::Entity,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 3;
    }
    namespace Creature {
        enum {
            SpeedFloat = 0, //
            SceneGUIDUInt64 = 1, //
            LastPublicSceneConfigIDInt64 = 2, //
            CopySceneGuidUInt64 = 3, //
            NicknameString = 4, //
            LevelInt64 = 5, //
            LastUpdatePositionTickUInt64 = 6, //
            WhoAttkMeUInt64 = 7, //
            AtkFloat = 8, //
            HpFloat = 9, //
            MaxHpFloat = 10, //
            CriticalFloat = 11, //
            CriticalDamageFloat = 12, //
            DefendFloat = 13, //
            MagicDefendFloat = 14, //
            BloodFloat = 15, //
            MagicAttackFloat = 16, //
            PhysicalPierceFloat = 17, //
            PhysicalPierceRateFloat = 18, //
            MagicPierceFloat = 19, //
            MagicPierceRateFloat = 20, //
            AffordFloat = 21, //
            DodgeFloat = 22, //
            ResilienceFloat = 23, //
            VisualFieldFloat = 24, //
            DamageReductionFloat = 25, //
            MpFloat = 26, //
            MaxMpFloat = 27, //
            AtkSpeedFloat = 28, //
            CantControllerRefInt32 = 29, //
            SilentRefInt32 = 30, //
            InvincibleRefInt32 = 31, //
            RestraintRefInt32 = 32, //
            ShiftRefInt32 = 33, //
            ExtraAddExpFloat = 34, //
            MoveSkillConfigIDInt64 = 35, //
            SkillAngleFloat = 36, //
            MoveSkillTargetGuidUInt64 = 37, //
            SkillTargetPosVector2 = 38, //
            DamagePriorityInt32 = 39, //
            LastSkillConfigIDInt64 = 40, //
            LastSkillUseTimeUInt64 = 41, //
            CastIsTrackOnCollisionBool = 42, //
            IsCantDamagedBool = 43, //
            TeamGuidUInt64 = 44, //
            MoveSpeedFloat = 45, //
            HpRecoverFloat = 46, //
            MpRecoverFloat = 47, //
            IsTeamLeaderBool = 48, //
            BattleStartTimeUInt64 = 49, //

            __AttributeCount__ = 50
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Creature, eGameObjectType::Entity,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 2;
    }
    namespace Entity {
        enum {
            SpeedFloat = 0, //
            SceneGUIDUInt64 = 1, //
            LastPublicSceneConfigIDInt64 = 2, //
            CopySceneGuidUInt64 = 3, //
            NicknameString = 4, //
            LevelInt64 = 5, //
            LastUpdatePositionTickUInt64 = 6, //
            WhoAttkMeUInt64 = 7, //
            AtkFloat = 8, //
            HpFloat = 9, //
            MaxHpFloat = 10, //
            CriticalFloat = 11, //
            CriticalDamageFloat = 12, //
            DefendFloat = 13, //
            MagicDefendFloat = 14, //
            BloodFloat = 15, //
            MagicAttackFloat = 16, //
            PhysicalPierceFloat = 17, //
            PhysicalPierceRateFloat = 18, //
            MagicPierceFloat = 19, //
            MagicPierceRateFloat = 20, //
            AffordFloat = 21, //
            DodgeFloat = 22, //
            ResilienceFloat = 23, //
            VisualFieldFloat = 24, //
            DamageReductionFloat = 25, //
            MpFloat = 26, //
            MaxMpFloat = 27, //
            AtkSpeedFloat = 28, //
            CantControllerRefInt32 = 29, //
            SilentRefInt32 = 30, //
            InvincibleRefInt32 = 31, //
            RestraintRefInt32 = 32, //
            ShiftRefInt32 = 33, //
            ExtraAddExpFloat = 34, //
            MoveSkillConfigIDInt64 = 35, //
            SkillAngleFloat = 36, //
            MoveSkillTargetGuidUInt64 = 37, //
            SkillTargetPosVector2 = 38, //
            DamagePriorityInt32 = 39, //
            LastSkillConfigIDInt64 = 40, //
            LastSkillUseTimeUInt64 = 41, //
            CastIsTrackOnCollisionBool = 42, //
            IsCantDamagedBool = 43, //
            TeamGuidUInt64 = 44, //
            MoveSpeedFloat = 45, //
            HpRecoverFloat = 46, //
            MpRecoverFloat = 47, //
            IsTeamLeaderBool = 48, //
            BattleStartTimeUInt64 = 49, //

            __AttributeCount__ = 50
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Entity,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 1;
    }
    namespace Equip {
        enum {
            ResourceIdInt64 = 0, //
            ItemTypeInt32 = 1, //
            CountInt32 = 2, //
            IsFirstCreateBool = 3, //
            StartPosVector2 = 4, //
            EndPosVector2 = 5, //
            StartTimeInt64 = 6, //
            CotrolPointVector2 = 7, //
            EquipTypeInt32 = 8, //
            EquipedBool = 9, //
            PatienceInt32 = 10, //
            StrengthLvInt32 = 11, //

            __AttributeCount__ = 12
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Equip, eGameObjectType::Item,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 2;
    }
    namespace Item {
        enum {
            ResourceIdInt64 = 0, //
            ItemTypeInt32 = 1, //
            CountInt32 = 2, //
            IsFirstCreateBool = 3, //
            StartPosVector2 = 4, //
            EndPosVector2 = 5, //
            StartTimeInt64 = 6, //
            CotrolPointVector2 = 7, //

            __AttributeCount__ = 8
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Item,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 1;
    }
    namespace Launcher {
        enum {
            SpeedFloat = 0, //
            SceneGUIDUInt64 = 1, //
            LastPublicSceneConfigIDInt64 = 2, //
            CopySceneGuidUInt64 = 3, //
            NicknameString = 4, //
            LevelInt64 = 5, //
            LastUpdatePositionTickUInt64 = 6, //
            WhoAttkMeUInt64 = 7, //
            AtkFloat = 8, //
            HpFloat = 9, //
            MaxHpFloat = 10, //
            CriticalFloat = 11, //
            CriticalDamageFloat = 12, //
            DefendFloat = 13, //
            MagicDefendFloat = 14, //
            BloodFloat = 15, //
            MagicAttackFloat = 16, //
            PhysicalPierceFloat = 17, //
            PhysicalPierceRateFloat = 18, //
            MagicPierceFloat = 19, //
            MagicPierceRateFloat = 20, //
            AffordFloat = 21, //
            DodgeFloat = 22, //
            ResilienceFloat = 23, //
            VisualFieldFloat = 24, //
            DamageReductionFloat = 25, //
            MpFloat = 26, //
            MaxMpFloat = 27, //
            AtkSpeedFloat = 28, //
            CantControllerRefInt32 = 29, //
            SilentRefInt32 = 30, //
            InvincibleRefInt32 = 31, //
            RestraintRefInt32 = 32, //
            ShiftRefInt32 = 33, //
            ExtraAddExpFloat = 34, //
            MoveSkillConfigIDInt64 = 35, //
            SkillAngleFloat = 36, //
            MoveSkillTargetGuidUInt64 = 37, //
            SkillTargetPosVector2 = 38, //
            DamagePriorityInt32 = 39, //
            LastSkillConfigIDInt64 = 40, //
            LastSkillUseTimeUInt64 = 41, //
            CastIsTrackOnCollisionBool = 42, //
            IsCantDamagedBool = 43, //
            TeamGuidUInt64 = 44, //
            MoveSpeedFloat = 45, //
            HpRecoverFloat = 46, //
            MpRecoverFloat = 47, //
            IsTeamLeaderBool = 48, //
            BattleStartTimeUInt64 = 49, //
            HostGuidUInt64 = 50, //
            TargetGuidUInt64 = 51, //
            TargetPosXFloat = 52, //
            TargetPosYFloat = 53, //

            __AttributeCount__ = 54
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Launcher, eGameObjectType::Entity,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 2;
    }
    namespace LootBox {
        enum {

            __AttributeCount__ = 0
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::LootBox,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 1;
    }
    namespace Monster {
        enum {
            SpeedFloat = 0, //
            SceneGUIDUInt64 = 1, //
            LastPublicSceneConfigIDInt64 = 2, //
            CopySceneGuidUInt64 = 3, //
            NicknameString = 4, //
            LevelInt64 = 5, //
            LastUpdatePositionTickUInt64 = 6, //
            WhoAttkMeUInt64 = 7, //
            AtkFloat = 8, //
            HpFloat = 9, //
            MaxHpFloat = 10, //
            CriticalFloat = 11, //
            CriticalDamageFloat = 12, //
            DefendFloat = 13, //
            MagicDefendFloat = 14, //
            BloodFloat = 15, //
            MagicAttackFloat = 16, //
            PhysicalPierceFloat = 17, //
            PhysicalPierceRateFloat = 18, //
            MagicPierceFloat = 19, //
            MagicPierceRateFloat = 20, //
            AffordFloat = 21, //
            DodgeFloat = 22, //
            ResilienceFloat = 23, //
            VisualFieldFloat = 24, //
            DamageReductionFloat = 25, //
            MpFloat = 26, //
            MaxMpFloat = 27, //
            AtkSpeedFloat = 28, //
            CantControllerRefInt32 = 29, //
            SilentRefInt32 = 30, //
            InvincibleRefInt32 = 31, //
            RestraintRefInt32 = 32, //
            ShiftRefInt32 = 33, //
            ExtraAddExpFloat = 34, //
            MoveSkillConfigIDInt64 = 35, //
            SkillAngleFloat = 36, //
            MoveSkillTargetGuidUInt64 = 37, //
            SkillTargetPosVector2 = 38, //
            DamagePriorityInt32 = 39, //
            LastSkillConfigIDInt64 = 40, //
            LastSkillUseTimeUInt64 = 41, //
            CastIsTrackOnCollisionBool = 42, //
            IsCantDamagedBool = 43, //
            TeamGuidUInt64 = 44, //
            MoveSpeedFloat = 45, //
            HpRecoverFloat = 46, //
            MpRecoverFloat = 47, //
            IsTeamLeaderBool = 48, //
            BattleStartTimeUInt64 = 49, //
            RefreshRuleGuidUInt64 = 50, //
            TargetUInt64 = 51, //
            BronPositionVector2 = 52, //
            IsPreviousSuccessBool = 53, //
            PreviousPositionVector2 = 54, //

            __AttributeCount__ = 55
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Monster, eGameObjectType::Creature, eGameObjectType::Entity,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 3;
    }
    namespace Npc {
        enum {

            __AttributeCount__ = 0
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Npc,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 1;
    }
    namespace RefreshRule {
        enum {
            SceneGuidUInt64 = 0, //
            CurrentRefreshCountInt32 = 1, //test
            TotleRefreshCountInt32 = 2, //

            __AttributeCount__ = 3
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::RefreshRule,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 1;
    }
    namespace Skill {
        enum {
            AccountUInt64 = 0, //
            CharacterIDUInt64 = 1, //
            SkillSlotString = 2, //
            LastCastTickUInt64 = 3, //

            __AttributeCount__ = 4
        };

        constexpr const eGameObjectType Types[] = { eGameObjectType::Skill,  eGameObjectType::__AllType__ };
        constexpr const int TypesCount = 1;
    }
    

    __forceinline bool IsType(const eGameObjectType type1, const eGameObjectType type2) {
        const eGameObjectType* types = nullptr;
        switch (type1) {
        case eGameObjectType::Boss: {
            types = Boss::Types;
            break;
        }
        case eGameObjectType::Buff: {
            types = Buff::Types;
            break;
        }
        case eGameObjectType::Bullet: {
            types = Bullet::Types;
            break;
        }
        case eGameObjectType::Character: {
            types = Character::Types;
            break;
        }
        case eGameObjectType::Creature: {
            types = Creature::Types;
            break;
        }
        case eGameObjectType::Entity: {
            types = Entity::Types;
            break;
        }
        case eGameObjectType::Equip: {
            types = Equip::Types;
            break;
        }
        case eGameObjectType::Item: {
            types = Item::Types;
            break;
        }
        case eGameObjectType::Launcher: {
            types = Launcher::Types;
            break;
        }
        case eGameObjectType::LootBox: {
            types = LootBox::Types;
            break;
        }
        case eGameObjectType::Monster: {
            types = Monster::Types;
            break;
        }
        case eGameObjectType::Npc: {
            types = Npc::Types;
            break;
        }
        case eGameObjectType::RefreshRule: {
            types = RefreshRule::Types;
            break;
        }
        case eGameObjectType::Skill: {
            types = Skill::Types;
            break;
        }

        }

        for (int i = 0; ; i++) {
            if (types[i] == eGameObjectType::__AllType__) {
                return false;
            }

            if (types[i] == type2) {
                return true;
            }
        }

        return false;
    }


#define __CreateGameObject__ \
    GameObject* go = nullptr; \
    switch (type) { \
        case eGameObjectType::Boss: \
        { \
            go = GameObject::Create("Boss", type, guid, config, Boss::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Boss::SpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "Speed", Boss::SpeedFloat, true, true); \
                    attributes[Boss::SceneGUIDUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SceneGUID", Boss::SceneGUIDUInt64, false, false); \
                    attributes[Boss::LastPublicSceneConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastPublicSceneConfigID", Boss::LastPublicSceneConfigIDInt64, false, false); \
                    attributes[Boss::CopySceneGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "CopySceneGuid", Boss::CopySceneGuidUInt64, false, false); \
                    attributes[Boss::NicknameString] = Attribute::Create(go, eAttributeType::__String__, "Nickname", Boss::NicknameString, false, false); \
                    attributes[Boss::LevelInt64] = Attribute::Create(go, eAttributeType::__Int64__, "Level", Boss::LevelInt64, true, true); \
                    attributes[Boss::LastUpdatePositionTickUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastUpdatePositionTick", Boss::LastUpdatePositionTickUInt64, false, false); \
                    attributes[Boss::WhoAttkMeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "WhoAttkMe", Boss::WhoAttkMeUInt64, false, false); \
                    attributes[Boss::AtkFloat] = Attribute::Create(go, eAttributeType::__Float__, "Atk", Boss::AtkFloat, true, true); \
                    attributes[Boss::HpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Hp", Boss::HpFloat, true, true); \
                    attributes[Boss::MaxHpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxHp", Boss::MaxHpFloat, true, true); \
                    attributes[Boss::CriticalFloat] = Attribute::Create(go, eAttributeType::__Float__, "Critical", Boss::CriticalFloat, true, true); \
                    attributes[Boss::CriticalDamageFloat] = Attribute::Create(go, eAttributeType::__Float__, "CriticalDamage", Boss::CriticalDamageFloat, true, true); \
                    attributes[Boss::DefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "Defend", Boss::DefendFloat, true, true); \
                    attributes[Boss::MagicDefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicDefend", Boss::MagicDefendFloat, true, true); \
                    attributes[Boss::BloodFloat] = Attribute::Create(go, eAttributeType::__Float__, "Blood", Boss::BloodFloat, true, true); \
                    attributes[Boss::MagicAttackFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicAttack", Boss::MagicAttackFloat, true, true); \
                    attributes[Boss::PhysicalPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierce", Boss::PhysicalPierceFloat, true, true); \
                    attributes[Boss::PhysicalPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierceRate", Boss::PhysicalPierceRateFloat, true, true); \
                    attributes[Boss::MagicPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierce", Boss::MagicPierceFloat, true, true); \
                    attributes[Boss::MagicPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierceRate", Boss::MagicPierceRateFloat, true, true); \
                    attributes[Boss::AffordFloat] = Attribute::Create(go, eAttributeType::__Float__, "Afford", Boss::AffordFloat, true, true); \
                    attributes[Boss::DodgeFloat] = Attribute::Create(go, eAttributeType::__Float__, "Dodge", Boss::DodgeFloat, true, true); \
                    attributes[Boss::ResilienceFloat] = Attribute::Create(go, eAttributeType::__Float__, "Resilience", Boss::ResilienceFloat, true, true); \
                    attributes[Boss::VisualFieldFloat] = Attribute::Create(go, eAttributeType::__Float__, "VisualField", Boss::VisualFieldFloat, true, true); \
                    attributes[Boss::DamageReductionFloat] = Attribute::Create(go, eAttributeType::__Float__, "DamageReduction", Boss::DamageReductionFloat, true, true); \
                    attributes[Boss::MpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Mp", Boss::MpFloat, true, true); \
                    attributes[Boss::MaxMpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxMp", Boss::MaxMpFloat, true, true); \
                    attributes[Boss::AtkSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "AtkSpeed", Boss::AtkSpeedFloat, true, true); \
                    attributes[Boss::CantControllerRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "CantControllerRef", Boss::CantControllerRefInt32, true, true); \
                    attributes[Boss::SilentRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "SilentRef", Boss::SilentRefInt32, true, true); \
                    attributes[Boss::InvincibleRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "InvincibleRef", Boss::InvincibleRefInt32, true, true); \
                    attributes[Boss::RestraintRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "RestraintRef", Boss::RestraintRefInt32, true, true); \
                    attributes[Boss::ShiftRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "ShiftRef", Boss::ShiftRefInt32, true, true); \
                    attributes[Boss::ExtraAddExpFloat] = Attribute::Create(go, eAttributeType::__Float__, "ExtraAddExp", Boss::ExtraAddExpFloat, false, false); \
                    attributes[Boss::MoveSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "MoveSkillConfigID", Boss::MoveSkillConfigIDInt64, false, false); \
                    attributes[Boss::SkillAngleFloat] = Attribute::Create(go, eAttributeType::__Float__, "SkillAngle", Boss::SkillAngleFloat, false, false); \
                    attributes[Boss::MoveSkillTargetGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "MoveSkillTargetGuid", Boss::MoveSkillTargetGuidUInt64, false, false); \
                    attributes[Boss::SkillTargetPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "SkillTargetPos", Boss::SkillTargetPosVector2, false, false); \
                    attributes[Boss::DamagePriorityInt32] = Attribute::Create(go, eAttributeType::__Int32__, "DamagePriority", Boss::DamagePriorityInt32, true, true); \
                    attributes[Boss::LastSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastSkillConfigID", Boss::LastSkillConfigIDInt64, false, false); \
                    attributes[Boss::LastSkillUseTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastSkillUseTime", Boss::LastSkillUseTimeUInt64, false, false); \
                    attributes[Boss::CastIsTrackOnCollisionBool] = Attribute::Create(go, eAttributeType::__Bool__, "CastIsTrackOnCollision", Boss::CastIsTrackOnCollisionBool, false, false); \
                    attributes[Boss::IsCantDamagedBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsCantDamaged", Boss::IsCantDamagedBool, true, true); \
                    attributes[Boss::TeamGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "TeamGuid", Boss::TeamGuidUInt64, true, true); \
                    attributes[Boss::MoveSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "MoveSpeed", Boss::MoveSpeedFloat, true, true); \
                    attributes[Boss::HpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "HpRecover", Boss::HpRecoverFloat, true, true); \
                    attributes[Boss::MpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "MpRecover", Boss::MpRecoverFloat, true, true); \
                    attributes[Boss::IsTeamLeaderBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsTeamLeader", Boss::IsTeamLeaderBool, true, true); \
                    attributes[Boss::BattleStartTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "BattleStartTime", Boss::BattleStartTimeUInt64, true, false); \
                    attributes[Boss::RefreshRuleGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "RefreshRuleGuid", Boss::RefreshRuleGuidUInt64, false, false); \
                    attributes[Boss::TargetUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "Target", Boss::TargetUInt64, false, false); \
                    attributes[Boss::BronPositionVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "BronPosition", Boss::BronPositionVector2, false, false); \
                    attributes[Boss::IsPreviousSuccessBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsPreviousSuccess", Boss::IsPreviousSuccessBool, false, false); \
                    attributes[Boss::PreviousPositionVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "PreviousPosition", Boss::PreviousPositionVector2, false, false); \
                    }, \
                    Boss::Types, \
                    Boss::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Buff: \
        { \
            go = GameObject::Create("Buff", type, guid, config, Buff::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Buff::CasterGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "CasterGuid", Buff::CasterGuidUInt64, false, false); \
                    attributes[Buff::HostGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "HostGuid", Buff::HostGuidUInt64, false, false); \
                    attributes[Buff::ChangeValueFloat] = Attribute::Create(go, eAttributeType::__Float__, "ChangeValue", Buff::ChangeValueFloat, false, false); \
                    attributes[Buff::CreateTimeTickUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "CreateTimeTick", Buff::CreateTimeTickUInt64, false, false); \
                    }, \
                    Buff::Types, \
                    Buff::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Bullet: \
        { \
            go = GameObject::Create("Bullet", type, guid, config, Bullet::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Bullet::SpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "Speed", Bullet::SpeedFloat, true, true); \
                    attributes[Bullet::SceneGUIDUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SceneGUID", Bullet::SceneGUIDUInt64, false, false); \
                    attributes[Bullet::LastPublicSceneConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastPublicSceneConfigID", Bullet::LastPublicSceneConfigIDInt64, false, false); \
                    attributes[Bullet::CopySceneGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "CopySceneGuid", Bullet::CopySceneGuidUInt64, false, false); \
                    attributes[Bullet::NicknameString] = Attribute::Create(go, eAttributeType::__String__, "Nickname", Bullet::NicknameString, false, false); \
                    attributes[Bullet::LevelInt64] = Attribute::Create(go, eAttributeType::__Int64__, "Level", Bullet::LevelInt64, true, true); \
                    attributes[Bullet::LastUpdatePositionTickUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastUpdatePositionTick", Bullet::LastUpdatePositionTickUInt64, false, false); \
                    attributes[Bullet::WhoAttkMeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "WhoAttkMe", Bullet::WhoAttkMeUInt64, false, false); \
                    attributes[Bullet::AtkFloat] = Attribute::Create(go, eAttributeType::__Float__, "Atk", Bullet::AtkFloat, true, true); \
                    attributes[Bullet::HpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Hp", Bullet::HpFloat, true, true); \
                    attributes[Bullet::MaxHpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxHp", Bullet::MaxHpFloat, true, true); \
                    attributes[Bullet::CriticalFloat] = Attribute::Create(go, eAttributeType::__Float__, "Critical", Bullet::CriticalFloat, true, true); \
                    attributes[Bullet::CriticalDamageFloat] = Attribute::Create(go, eAttributeType::__Float__, "CriticalDamage", Bullet::CriticalDamageFloat, true, true); \
                    attributes[Bullet::DefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "Defend", Bullet::DefendFloat, true, true); \
                    attributes[Bullet::MagicDefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicDefend", Bullet::MagicDefendFloat, true, true); \
                    attributes[Bullet::BloodFloat] = Attribute::Create(go, eAttributeType::__Float__, "Blood", Bullet::BloodFloat, true, true); \
                    attributes[Bullet::MagicAttackFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicAttack", Bullet::MagicAttackFloat, true, true); \
                    attributes[Bullet::PhysicalPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierce", Bullet::PhysicalPierceFloat, true, true); \
                    attributes[Bullet::PhysicalPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierceRate", Bullet::PhysicalPierceRateFloat, true, true); \
                    attributes[Bullet::MagicPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierce", Bullet::MagicPierceFloat, true, true); \
                    attributes[Bullet::MagicPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierceRate", Bullet::MagicPierceRateFloat, true, true); \
                    attributes[Bullet::AffordFloat] = Attribute::Create(go, eAttributeType::__Float__, "Afford", Bullet::AffordFloat, true, true); \
                    attributes[Bullet::DodgeFloat] = Attribute::Create(go, eAttributeType::__Float__, "Dodge", Bullet::DodgeFloat, true, true); \
                    attributes[Bullet::ResilienceFloat] = Attribute::Create(go, eAttributeType::__Float__, "Resilience", Bullet::ResilienceFloat, true, true); \
                    attributes[Bullet::VisualFieldFloat] = Attribute::Create(go, eAttributeType::__Float__, "VisualField", Bullet::VisualFieldFloat, true, true); \
                    attributes[Bullet::DamageReductionFloat] = Attribute::Create(go, eAttributeType::__Float__, "DamageReduction", Bullet::DamageReductionFloat, true, true); \
                    attributes[Bullet::MpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Mp", Bullet::MpFloat, true, true); \
                    attributes[Bullet::MaxMpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxMp", Bullet::MaxMpFloat, true, true); \
                    attributes[Bullet::AtkSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "AtkSpeed", Bullet::AtkSpeedFloat, true, true); \
                    attributes[Bullet::CantControllerRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "CantControllerRef", Bullet::CantControllerRefInt32, true, true); \
                    attributes[Bullet::SilentRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "SilentRef", Bullet::SilentRefInt32, true, true); \
                    attributes[Bullet::InvincibleRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "InvincibleRef", Bullet::InvincibleRefInt32, true, true); \
                    attributes[Bullet::RestraintRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "RestraintRef", Bullet::RestraintRefInt32, true, true); \
                    attributes[Bullet::ShiftRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "ShiftRef", Bullet::ShiftRefInt32, true, true); \
                    attributes[Bullet::ExtraAddExpFloat] = Attribute::Create(go, eAttributeType::__Float__, "ExtraAddExp", Bullet::ExtraAddExpFloat, false, false); \
                    attributes[Bullet::MoveSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "MoveSkillConfigID", Bullet::MoveSkillConfigIDInt64, false, false); \
                    attributes[Bullet::SkillAngleFloat] = Attribute::Create(go, eAttributeType::__Float__, "SkillAngle", Bullet::SkillAngleFloat, false, false); \
                    attributes[Bullet::MoveSkillTargetGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "MoveSkillTargetGuid", Bullet::MoveSkillTargetGuidUInt64, false, false); \
                    attributes[Bullet::SkillTargetPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "SkillTargetPos", Bullet::SkillTargetPosVector2, false, false); \
                    attributes[Bullet::DamagePriorityInt32] = Attribute::Create(go, eAttributeType::__Int32__, "DamagePriority", Bullet::DamagePriorityInt32, true, true); \
                    attributes[Bullet::LastSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastSkillConfigID", Bullet::LastSkillConfigIDInt64, false, false); \
                    attributes[Bullet::LastSkillUseTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastSkillUseTime", Bullet::LastSkillUseTimeUInt64, false, false); \
                    attributes[Bullet::CastIsTrackOnCollisionBool] = Attribute::Create(go, eAttributeType::__Bool__, "CastIsTrackOnCollision", Bullet::CastIsTrackOnCollisionBool, false, false); \
                    attributes[Bullet::IsCantDamagedBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsCantDamaged", Bullet::IsCantDamagedBool, true, true); \
                    attributes[Bullet::TeamGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "TeamGuid", Bullet::TeamGuidUInt64, true, true); \
                    attributes[Bullet::MoveSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "MoveSpeed", Bullet::MoveSpeedFloat, true, true); \
                    attributes[Bullet::HpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "HpRecover", Bullet::HpRecoverFloat, true, true); \
                    attributes[Bullet::MpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "MpRecover", Bullet::MpRecoverFloat, true, true); \
                    attributes[Bullet::IsTeamLeaderBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsTeamLeader", Bullet::IsTeamLeaderBool, true, true); \
                    attributes[Bullet::BattleStartTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "BattleStartTime", Bullet::BattleStartTimeUInt64, true, false); \
                    attributes[Bullet::HostGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "HostGuid", Bullet::HostGuidUInt64, false, false); \
                    attributes[Bullet::TargetGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "TargetGuid", Bullet::TargetGuidUInt64, false, false); \
                    attributes[Bullet::TargetPosXFloat] = Attribute::Create(go, eAttributeType::__Float__, "TargetPosX", Bullet::TargetPosXFloat, false, false); \
                    attributes[Bullet::TargetPosYFloat] = Attribute::Create(go, eAttributeType::__Float__, "TargetPosY", Bullet::TargetPosYFloat, false, false); \
                    attributes[Bullet::LastUpdateTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastUpdateTime", Bullet::LastUpdateTimeUInt64, false, false); \
                    attributes[Bullet::DeltaTimeFloat] = Attribute::Create(go, eAttributeType::__Float__, "DeltaTime", Bullet::DeltaTimeFloat, false, false); \
                    attributes[Bullet::ControlPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "ControlPos", Bullet::ControlPosVector2, false, false); \
                    attributes[Bullet::StartPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "StartPos", Bullet::StartPosVector2, false, false); \
                    attributes[Bullet::DamageCountInt64] = Attribute::Create(go, eAttributeType::__Int64__, "DamageCount", Bullet::DamageCountInt64, false, false); \
                    attributes[Bullet::HitUnitGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "HitUnitGuid", Bullet::HitUnitGuidUInt64, false, false); \
                    }, \
                    Bullet::Types, \
                    Bullet::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Character: \
        { \
            go = GameObject::Create("Character", type, guid, config, Character::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Character::SpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "Speed", Character::SpeedFloat, true, true); \
                    attributes[Character::SceneGUIDUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SceneGUID", Character::SceneGUIDUInt64, false, false); \
                    attributes[Character::LastPublicSceneConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastPublicSceneConfigID", Character::LastPublicSceneConfigIDInt64, false, false); \
                    attributes[Character::CopySceneGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "CopySceneGuid", Character::CopySceneGuidUInt64, false, false); \
                    attributes[Character::NicknameString] = Attribute::Create(go, eAttributeType::__String__, "Nickname", Character::NicknameString, false, false); \
                    attributes[Character::LevelInt64] = Attribute::Create(go, eAttributeType::__Int64__, "Level", Character::LevelInt64, true, true); \
                    attributes[Character::LastUpdatePositionTickUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastUpdatePositionTick", Character::LastUpdatePositionTickUInt64, false, false); \
                    attributes[Character::WhoAttkMeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "WhoAttkMe", Character::WhoAttkMeUInt64, false, false); \
                    attributes[Character::AtkFloat] = Attribute::Create(go, eAttributeType::__Float__, "Atk", Character::AtkFloat, true, true); \
                    attributes[Character::HpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Hp", Character::HpFloat, true, true); \
                    attributes[Character::MaxHpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxHp", Character::MaxHpFloat, true, true); \
                    attributes[Character::CriticalFloat] = Attribute::Create(go, eAttributeType::__Float__, "Critical", Character::CriticalFloat, true, true); \
                    attributes[Character::CriticalDamageFloat] = Attribute::Create(go, eAttributeType::__Float__, "CriticalDamage", Character::CriticalDamageFloat, true, true); \
                    attributes[Character::DefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "Defend", Character::DefendFloat, true, true); \
                    attributes[Character::MagicDefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicDefend", Character::MagicDefendFloat, true, true); \
                    attributes[Character::BloodFloat] = Attribute::Create(go, eAttributeType::__Float__, "Blood", Character::BloodFloat, true, true); \
                    attributes[Character::MagicAttackFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicAttack", Character::MagicAttackFloat, true, true); \
                    attributes[Character::PhysicalPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierce", Character::PhysicalPierceFloat, true, true); \
                    attributes[Character::PhysicalPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierceRate", Character::PhysicalPierceRateFloat, true, true); \
                    attributes[Character::MagicPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierce", Character::MagicPierceFloat, true, true); \
                    attributes[Character::MagicPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierceRate", Character::MagicPierceRateFloat, true, true); \
                    attributes[Character::AffordFloat] = Attribute::Create(go, eAttributeType::__Float__, "Afford", Character::AffordFloat, true, true); \
                    attributes[Character::DodgeFloat] = Attribute::Create(go, eAttributeType::__Float__, "Dodge", Character::DodgeFloat, true, true); \
                    attributes[Character::ResilienceFloat] = Attribute::Create(go, eAttributeType::__Float__, "Resilience", Character::ResilienceFloat, true, true); \
                    attributes[Character::VisualFieldFloat] = Attribute::Create(go, eAttributeType::__Float__, "VisualField", Character::VisualFieldFloat, true, true); \
                    attributes[Character::DamageReductionFloat] = Attribute::Create(go, eAttributeType::__Float__, "DamageReduction", Character::DamageReductionFloat, true, true); \
                    attributes[Character::MpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Mp", Character::MpFloat, true, true); \
                    attributes[Character::MaxMpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxMp", Character::MaxMpFloat, true, true); \
                    attributes[Character::AtkSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "AtkSpeed", Character::AtkSpeedFloat, true, true); \
                    attributes[Character::CantControllerRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "CantControllerRef", Character::CantControllerRefInt32, true, true); \
                    attributes[Character::SilentRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "SilentRef", Character::SilentRefInt32, true, true); \
                    attributes[Character::InvincibleRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "InvincibleRef", Character::InvincibleRefInt32, true, true); \
                    attributes[Character::RestraintRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "RestraintRef", Character::RestraintRefInt32, true, true); \
                    attributes[Character::ShiftRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "ShiftRef", Character::ShiftRefInt32, true, true); \
                    attributes[Character::ExtraAddExpFloat] = Attribute::Create(go, eAttributeType::__Float__, "ExtraAddExp", Character::ExtraAddExpFloat, false, false); \
                    attributes[Character::MoveSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "MoveSkillConfigID", Character::MoveSkillConfigIDInt64, false, false); \
                    attributes[Character::SkillAngleFloat] = Attribute::Create(go, eAttributeType::__Float__, "SkillAngle", Character::SkillAngleFloat, false, false); \
                    attributes[Character::MoveSkillTargetGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "MoveSkillTargetGuid", Character::MoveSkillTargetGuidUInt64, false, false); \
                    attributes[Character::SkillTargetPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "SkillTargetPos", Character::SkillTargetPosVector2, false, false); \
                    attributes[Character::DamagePriorityInt32] = Attribute::Create(go, eAttributeType::__Int32__, "DamagePriority", Character::DamagePriorityInt32, true, true); \
                    attributes[Character::LastSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastSkillConfigID", Character::LastSkillConfigIDInt64, false, false); \
                    attributes[Character::LastSkillUseTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastSkillUseTime", Character::LastSkillUseTimeUInt64, false, false); \
                    attributes[Character::CastIsTrackOnCollisionBool] = Attribute::Create(go, eAttributeType::__Bool__, "CastIsTrackOnCollision", Character::CastIsTrackOnCollisionBool, false, false); \
                    attributes[Character::IsCantDamagedBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsCantDamaged", Character::IsCantDamagedBool, true, true); \
                    attributes[Character::TeamGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "TeamGuid", Character::TeamGuidUInt64, true, true); \
                    attributes[Character::MoveSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "MoveSpeed", Character::MoveSpeedFloat, true, true); \
                    attributes[Character::HpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "HpRecover", Character::HpRecoverFloat, true, true); \
                    attributes[Character::MpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "MpRecover", Character::MpRecoverFloat, true, true); \
                    attributes[Character::IsTeamLeaderBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsTeamLeader", Character::IsTeamLeaderBool, true, true); \
                    attributes[Character::BattleStartTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "BattleStartTime", Character::BattleStartTimeUInt64, true, false); \
                    attributes[Character::AccountUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "Account", Character::AccountUInt64, true, true); \
                    attributes[Character::ExpInt64] = Attribute::Create(go, eAttributeType::__Int64__, "Exp", Character::ExpInt64, true, false); \
                    attributes[Character::SlotClothsUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SlotCloths", Character::SlotClothsUInt64, false, false); \
                    attributes[Character::ClothsConfigInt64] = Attribute::Create(go, eAttributeType::__Int64__, "ClothsConfig", Character::ClothsConfigInt64, false, false); \
                    attributes[Character::SlotPantsUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SlotPants", Character::SlotPantsUInt64, false, false); \
                    attributes[Character::PantsConfigInt64] = Attribute::Create(go, eAttributeType::__Int64__, "PantsConfig", Character::PantsConfigInt64, false, false); \
                    attributes[Character::SlotBeltUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SlotBelt", Character::SlotBeltUInt64, false, false); \
                    attributes[Character::BeltConfigInt64] = Attribute::Create(go, eAttributeType::__Int64__, "BeltConfig", Character::BeltConfigInt64, false, false); \
                    attributes[Character::SlotShoesUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SlotShoes", Character::SlotShoesUInt64, false, false); \
                    attributes[Character::ShoesConfigInt64] = Attribute::Create(go, eAttributeType::__Int64__, "ShoesConfig", Character::ShoesConfigInt64, false, false); \
                    attributes[Character::SlotJewelryUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SlotJewelry", Character::SlotJewelryUInt64, false, false); \
                    attributes[Character::JewelryConfigInt64] = Attribute::Create(go, eAttributeType::__Int64__, "JewelryConfig", Character::JewelryConfigInt64, false, false); \
                    attributes[Character::SlotWeaponUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SlotWeapon", Character::SlotWeaponUInt64, false, false); \
                    attributes[Character::WeaponConfigInt64] = Attribute::Create(go, eAttributeType::__Int64__, "WeaponConfig", Character::WeaponConfigInt64, false, false); \
                    attributes[Character::MoveTargetGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "MoveTargetGuid", Character::MoveTargetGuidUInt64, false, false); \
                    }, \
                    Character::Types, \
                    Character::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Creature: \
        { \
            go = GameObject::Create("Creature", type, guid, config, Creature::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Creature::SpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "Speed", Creature::SpeedFloat, true, true); \
                    attributes[Creature::SceneGUIDUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SceneGUID", Creature::SceneGUIDUInt64, false, false); \
                    attributes[Creature::LastPublicSceneConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastPublicSceneConfigID", Creature::LastPublicSceneConfigIDInt64, false, false); \
                    attributes[Creature::CopySceneGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "CopySceneGuid", Creature::CopySceneGuidUInt64, false, false); \
                    attributes[Creature::NicknameString] = Attribute::Create(go, eAttributeType::__String__, "Nickname", Creature::NicknameString, false, false); \
                    attributes[Creature::LevelInt64] = Attribute::Create(go, eAttributeType::__Int64__, "Level", Creature::LevelInt64, true, true); \
                    attributes[Creature::LastUpdatePositionTickUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastUpdatePositionTick", Creature::LastUpdatePositionTickUInt64, false, false); \
                    attributes[Creature::WhoAttkMeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "WhoAttkMe", Creature::WhoAttkMeUInt64, false, false); \
                    attributes[Creature::AtkFloat] = Attribute::Create(go, eAttributeType::__Float__, "Atk", Creature::AtkFloat, true, true); \
                    attributes[Creature::HpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Hp", Creature::HpFloat, true, true); \
                    attributes[Creature::MaxHpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxHp", Creature::MaxHpFloat, true, true); \
                    attributes[Creature::CriticalFloat] = Attribute::Create(go, eAttributeType::__Float__, "Critical", Creature::CriticalFloat, true, true); \
                    attributes[Creature::CriticalDamageFloat] = Attribute::Create(go, eAttributeType::__Float__, "CriticalDamage", Creature::CriticalDamageFloat, true, true); \
                    attributes[Creature::DefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "Defend", Creature::DefendFloat, true, true); \
                    attributes[Creature::MagicDefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicDefend", Creature::MagicDefendFloat, true, true); \
                    attributes[Creature::BloodFloat] = Attribute::Create(go, eAttributeType::__Float__, "Blood", Creature::BloodFloat, true, true); \
                    attributes[Creature::MagicAttackFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicAttack", Creature::MagicAttackFloat, true, true); \
                    attributes[Creature::PhysicalPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierce", Creature::PhysicalPierceFloat, true, true); \
                    attributes[Creature::PhysicalPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierceRate", Creature::PhysicalPierceRateFloat, true, true); \
                    attributes[Creature::MagicPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierce", Creature::MagicPierceFloat, true, true); \
                    attributes[Creature::MagicPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierceRate", Creature::MagicPierceRateFloat, true, true); \
                    attributes[Creature::AffordFloat] = Attribute::Create(go, eAttributeType::__Float__, "Afford", Creature::AffordFloat, true, true); \
                    attributes[Creature::DodgeFloat] = Attribute::Create(go, eAttributeType::__Float__, "Dodge", Creature::DodgeFloat, true, true); \
                    attributes[Creature::ResilienceFloat] = Attribute::Create(go, eAttributeType::__Float__, "Resilience", Creature::ResilienceFloat, true, true); \
                    attributes[Creature::VisualFieldFloat] = Attribute::Create(go, eAttributeType::__Float__, "VisualField", Creature::VisualFieldFloat, true, true); \
                    attributes[Creature::DamageReductionFloat] = Attribute::Create(go, eAttributeType::__Float__, "DamageReduction", Creature::DamageReductionFloat, true, true); \
                    attributes[Creature::MpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Mp", Creature::MpFloat, true, true); \
                    attributes[Creature::MaxMpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxMp", Creature::MaxMpFloat, true, true); \
                    attributes[Creature::AtkSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "AtkSpeed", Creature::AtkSpeedFloat, true, true); \
                    attributes[Creature::CantControllerRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "CantControllerRef", Creature::CantControllerRefInt32, true, true); \
                    attributes[Creature::SilentRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "SilentRef", Creature::SilentRefInt32, true, true); \
                    attributes[Creature::InvincibleRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "InvincibleRef", Creature::InvincibleRefInt32, true, true); \
                    attributes[Creature::RestraintRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "RestraintRef", Creature::RestraintRefInt32, true, true); \
                    attributes[Creature::ShiftRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "ShiftRef", Creature::ShiftRefInt32, true, true); \
                    attributes[Creature::ExtraAddExpFloat] = Attribute::Create(go, eAttributeType::__Float__, "ExtraAddExp", Creature::ExtraAddExpFloat, false, false); \
                    attributes[Creature::MoveSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "MoveSkillConfigID", Creature::MoveSkillConfigIDInt64, false, false); \
                    attributes[Creature::SkillAngleFloat] = Attribute::Create(go, eAttributeType::__Float__, "SkillAngle", Creature::SkillAngleFloat, false, false); \
                    attributes[Creature::MoveSkillTargetGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "MoveSkillTargetGuid", Creature::MoveSkillTargetGuidUInt64, false, false); \
                    attributes[Creature::SkillTargetPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "SkillTargetPos", Creature::SkillTargetPosVector2, false, false); \
                    attributes[Creature::DamagePriorityInt32] = Attribute::Create(go, eAttributeType::__Int32__, "DamagePriority", Creature::DamagePriorityInt32, true, true); \
                    attributes[Creature::LastSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastSkillConfigID", Creature::LastSkillConfigIDInt64, false, false); \
                    attributes[Creature::LastSkillUseTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastSkillUseTime", Creature::LastSkillUseTimeUInt64, false, false); \
                    attributes[Creature::CastIsTrackOnCollisionBool] = Attribute::Create(go, eAttributeType::__Bool__, "CastIsTrackOnCollision", Creature::CastIsTrackOnCollisionBool, false, false); \
                    attributes[Creature::IsCantDamagedBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsCantDamaged", Creature::IsCantDamagedBool, true, true); \
                    attributes[Creature::TeamGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "TeamGuid", Creature::TeamGuidUInt64, true, true); \
                    attributes[Creature::MoveSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "MoveSpeed", Creature::MoveSpeedFloat, true, true); \
                    attributes[Creature::HpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "HpRecover", Creature::HpRecoverFloat, true, true); \
                    attributes[Creature::MpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "MpRecover", Creature::MpRecoverFloat, true, true); \
                    attributes[Creature::IsTeamLeaderBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsTeamLeader", Creature::IsTeamLeaderBool, true, true); \
                    attributes[Creature::BattleStartTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "BattleStartTime", Creature::BattleStartTimeUInt64, true, false); \
                    }, \
                    Creature::Types, \
                    Creature::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Entity: \
        { \
            go = GameObject::Create("Entity", type, guid, config, Entity::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Entity::SpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "Speed", Entity::SpeedFloat, true, true); \
                    attributes[Entity::SceneGUIDUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SceneGUID", Entity::SceneGUIDUInt64, false, false); \
                    attributes[Entity::LastPublicSceneConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastPublicSceneConfigID", Entity::LastPublicSceneConfigIDInt64, false, false); \
                    attributes[Entity::CopySceneGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "CopySceneGuid", Entity::CopySceneGuidUInt64, false, false); \
                    attributes[Entity::NicknameString] = Attribute::Create(go, eAttributeType::__String__, "Nickname", Entity::NicknameString, false, false); \
                    attributes[Entity::LevelInt64] = Attribute::Create(go, eAttributeType::__Int64__, "Level", Entity::LevelInt64, true, true); \
                    attributes[Entity::LastUpdatePositionTickUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastUpdatePositionTick", Entity::LastUpdatePositionTickUInt64, false, false); \
                    attributes[Entity::WhoAttkMeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "WhoAttkMe", Entity::WhoAttkMeUInt64, false, false); \
                    attributes[Entity::AtkFloat] = Attribute::Create(go, eAttributeType::__Float__, "Atk", Entity::AtkFloat, true, true); \
                    attributes[Entity::HpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Hp", Entity::HpFloat, true, true); \
                    attributes[Entity::MaxHpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxHp", Entity::MaxHpFloat, true, true); \
                    attributes[Entity::CriticalFloat] = Attribute::Create(go, eAttributeType::__Float__, "Critical", Entity::CriticalFloat, true, true); \
                    attributes[Entity::CriticalDamageFloat] = Attribute::Create(go, eAttributeType::__Float__, "CriticalDamage", Entity::CriticalDamageFloat, true, true); \
                    attributes[Entity::DefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "Defend", Entity::DefendFloat, true, true); \
                    attributes[Entity::MagicDefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicDefend", Entity::MagicDefendFloat, true, true); \
                    attributes[Entity::BloodFloat] = Attribute::Create(go, eAttributeType::__Float__, "Blood", Entity::BloodFloat, true, true); \
                    attributes[Entity::MagicAttackFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicAttack", Entity::MagicAttackFloat, true, true); \
                    attributes[Entity::PhysicalPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierce", Entity::PhysicalPierceFloat, true, true); \
                    attributes[Entity::PhysicalPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierceRate", Entity::PhysicalPierceRateFloat, true, true); \
                    attributes[Entity::MagicPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierce", Entity::MagicPierceFloat, true, true); \
                    attributes[Entity::MagicPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierceRate", Entity::MagicPierceRateFloat, true, true); \
                    attributes[Entity::AffordFloat] = Attribute::Create(go, eAttributeType::__Float__, "Afford", Entity::AffordFloat, true, true); \
                    attributes[Entity::DodgeFloat] = Attribute::Create(go, eAttributeType::__Float__, "Dodge", Entity::DodgeFloat, true, true); \
                    attributes[Entity::ResilienceFloat] = Attribute::Create(go, eAttributeType::__Float__, "Resilience", Entity::ResilienceFloat, true, true); \
                    attributes[Entity::VisualFieldFloat] = Attribute::Create(go, eAttributeType::__Float__, "VisualField", Entity::VisualFieldFloat, true, true); \
                    attributes[Entity::DamageReductionFloat] = Attribute::Create(go, eAttributeType::__Float__, "DamageReduction", Entity::DamageReductionFloat, true, true); \
                    attributes[Entity::MpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Mp", Entity::MpFloat, true, true); \
                    attributes[Entity::MaxMpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxMp", Entity::MaxMpFloat, true, true); \
                    attributes[Entity::AtkSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "AtkSpeed", Entity::AtkSpeedFloat, true, true); \
                    attributes[Entity::CantControllerRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "CantControllerRef", Entity::CantControllerRefInt32, true, true); \
                    attributes[Entity::SilentRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "SilentRef", Entity::SilentRefInt32, true, true); \
                    attributes[Entity::InvincibleRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "InvincibleRef", Entity::InvincibleRefInt32, true, true); \
                    attributes[Entity::RestraintRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "RestraintRef", Entity::RestraintRefInt32, true, true); \
                    attributes[Entity::ShiftRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "ShiftRef", Entity::ShiftRefInt32, true, true); \
                    attributes[Entity::ExtraAddExpFloat] = Attribute::Create(go, eAttributeType::__Float__, "ExtraAddExp", Entity::ExtraAddExpFloat, false, false); \
                    attributes[Entity::MoveSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "MoveSkillConfigID", Entity::MoveSkillConfigIDInt64, false, false); \
                    attributes[Entity::SkillAngleFloat] = Attribute::Create(go, eAttributeType::__Float__, "SkillAngle", Entity::SkillAngleFloat, false, false); \
                    attributes[Entity::MoveSkillTargetGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "MoveSkillTargetGuid", Entity::MoveSkillTargetGuidUInt64, false, false); \
                    attributes[Entity::SkillTargetPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "SkillTargetPos", Entity::SkillTargetPosVector2, false, false); \
                    attributes[Entity::DamagePriorityInt32] = Attribute::Create(go, eAttributeType::__Int32__, "DamagePriority", Entity::DamagePriorityInt32, true, true); \
                    attributes[Entity::LastSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastSkillConfigID", Entity::LastSkillConfigIDInt64, false, false); \
                    attributes[Entity::LastSkillUseTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastSkillUseTime", Entity::LastSkillUseTimeUInt64, false, false); \
                    attributes[Entity::CastIsTrackOnCollisionBool] = Attribute::Create(go, eAttributeType::__Bool__, "CastIsTrackOnCollision", Entity::CastIsTrackOnCollisionBool, false, false); \
                    attributes[Entity::IsCantDamagedBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsCantDamaged", Entity::IsCantDamagedBool, true, true); \
                    attributes[Entity::TeamGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "TeamGuid", Entity::TeamGuidUInt64, true, true); \
                    attributes[Entity::MoveSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "MoveSpeed", Entity::MoveSpeedFloat, true, true); \
                    attributes[Entity::HpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "HpRecover", Entity::HpRecoverFloat, true, true); \
                    attributes[Entity::MpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "MpRecover", Entity::MpRecoverFloat, true, true); \
                    attributes[Entity::IsTeamLeaderBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsTeamLeader", Entity::IsTeamLeaderBool, true, true); \
                    attributes[Entity::BattleStartTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "BattleStartTime", Entity::BattleStartTimeUInt64, true, false); \
                    }, \
                    Entity::Types, \
                    Entity::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Equip: \
        { \
            go = GameObject::Create("Equip", type, guid, config, Equip::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Equip::ResourceIdInt64] = Attribute::Create(go, eAttributeType::__Int64__, "ResourceId", Equip::ResourceIdInt64, false, false); \
                    attributes[Equip::ItemTypeInt32] = Attribute::Create(go, eAttributeType::__Int32__, "ItemType", Equip::ItemTypeInt32, false, false); \
                    attributes[Equip::CountInt32] = Attribute::Create(go, eAttributeType::__Int32__, "Count", Equip::CountInt32, false, false); \
                    attributes[Equip::IsFirstCreateBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsFirstCreate", Equip::IsFirstCreateBool, false, false); \
                    attributes[Equip::StartPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "StartPos", Equip::StartPosVector2, false, false); \
                    attributes[Equip::EndPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "EndPos", Equip::EndPosVector2, false, false); \
                    attributes[Equip::StartTimeInt64] = Attribute::Create(go, eAttributeType::__Int64__, "StartTime", Equip::StartTimeInt64, false, false); \
                    attributes[Equip::CotrolPointVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "CotrolPoint", Equip::CotrolPointVector2, false, false); \
                    attributes[Equip::EquipTypeInt32] = Attribute::Create(go, eAttributeType::__Int32__, "EquipType", Equip::EquipTypeInt32, false, false); \
                    attributes[Equip::EquipedBool] = Attribute::Create(go, eAttributeType::__Bool__, "Equiped", Equip::EquipedBool, false, false); \
                    attributes[Equip::PatienceInt32] = Attribute::Create(go, eAttributeType::__Int32__, "Patience", Equip::PatienceInt32, false, false); \
                    attributes[Equip::StrengthLvInt32] = Attribute::Create(go, eAttributeType::__Int32__, "StrengthLv", Equip::StrengthLvInt32, false, false); \
                    }, \
                    Equip::Types, \
                    Equip::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Item: \
        { \
            go = GameObject::Create("Item", type, guid, config, Item::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Item::ResourceIdInt64] = Attribute::Create(go, eAttributeType::__Int64__, "ResourceId", Item::ResourceIdInt64, false, false); \
                    attributes[Item::ItemTypeInt32] = Attribute::Create(go, eAttributeType::__Int32__, "ItemType", Item::ItemTypeInt32, false, false); \
                    attributes[Item::CountInt32] = Attribute::Create(go, eAttributeType::__Int32__, "Count", Item::CountInt32, false, false); \
                    attributes[Item::IsFirstCreateBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsFirstCreate", Item::IsFirstCreateBool, false, false); \
                    attributes[Item::StartPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "StartPos", Item::StartPosVector2, false, false); \
                    attributes[Item::EndPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "EndPos", Item::EndPosVector2, false, false); \
                    attributes[Item::StartTimeInt64] = Attribute::Create(go, eAttributeType::__Int64__, "StartTime", Item::StartTimeInt64, false, false); \
                    attributes[Item::CotrolPointVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "CotrolPoint", Item::CotrolPointVector2, false, false); \
                    }, \
                    Item::Types, \
                    Item::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Launcher: \
        { \
            go = GameObject::Create("Launcher", type, guid, config, Launcher::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Launcher::SpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "Speed", Launcher::SpeedFloat, true, true); \
                    attributes[Launcher::SceneGUIDUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SceneGUID", Launcher::SceneGUIDUInt64, false, false); \
                    attributes[Launcher::LastPublicSceneConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastPublicSceneConfigID", Launcher::LastPublicSceneConfigIDInt64, false, false); \
                    attributes[Launcher::CopySceneGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "CopySceneGuid", Launcher::CopySceneGuidUInt64, false, false); \
                    attributes[Launcher::NicknameString] = Attribute::Create(go, eAttributeType::__String__, "Nickname", Launcher::NicknameString, false, false); \
                    attributes[Launcher::LevelInt64] = Attribute::Create(go, eAttributeType::__Int64__, "Level", Launcher::LevelInt64, true, true); \
                    attributes[Launcher::LastUpdatePositionTickUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastUpdatePositionTick", Launcher::LastUpdatePositionTickUInt64, false, false); \
                    attributes[Launcher::WhoAttkMeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "WhoAttkMe", Launcher::WhoAttkMeUInt64, false, false); \
                    attributes[Launcher::AtkFloat] = Attribute::Create(go, eAttributeType::__Float__, "Atk", Launcher::AtkFloat, true, true); \
                    attributes[Launcher::HpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Hp", Launcher::HpFloat, true, true); \
                    attributes[Launcher::MaxHpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxHp", Launcher::MaxHpFloat, true, true); \
                    attributes[Launcher::CriticalFloat] = Attribute::Create(go, eAttributeType::__Float__, "Critical", Launcher::CriticalFloat, true, true); \
                    attributes[Launcher::CriticalDamageFloat] = Attribute::Create(go, eAttributeType::__Float__, "CriticalDamage", Launcher::CriticalDamageFloat, true, true); \
                    attributes[Launcher::DefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "Defend", Launcher::DefendFloat, true, true); \
                    attributes[Launcher::MagicDefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicDefend", Launcher::MagicDefendFloat, true, true); \
                    attributes[Launcher::BloodFloat] = Attribute::Create(go, eAttributeType::__Float__, "Blood", Launcher::BloodFloat, true, true); \
                    attributes[Launcher::MagicAttackFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicAttack", Launcher::MagicAttackFloat, true, true); \
                    attributes[Launcher::PhysicalPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierce", Launcher::PhysicalPierceFloat, true, true); \
                    attributes[Launcher::PhysicalPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierceRate", Launcher::PhysicalPierceRateFloat, true, true); \
                    attributes[Launcher::MagicPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierce", Launcher::MagicPierceFloat, true, true); \
                    attributes[Launcher::MagicPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierceRate", Launcher::MagicPierceRateFloat, true, true); \
                    attributes[Launcher::AffordFloat] = Attribute::Create(go, eAttributeType::__Float__, "Afford", Launcher::AffordFloat, true, true); \
                    attributes[Launcher::DodgeFloat] = Attribute::Create(go, eAttributeType::__Float__, "Dodge", Launcher::DodgeFloat, true, true); \
                    attributes[Launcher::ResilienceFloat] = Attribute::Create(go, eAttributeType::__Float__, "Resilience", Launcher::ResilienceFloat, true, true); \
                    attributes[Launcher::VisualFieldFloat] = Attribute::Create(go, eAttributeType::__Float__, "VisualField", Launcher::VisualFieldFloat, true, true); \
                    attributes[Launcher::DamageReductionFloat] = Attribute::Create(go, eAttributeType::__Float__, "DamageReduction", Launcher::DamageReductionFloat, true, true); \
                    attributes[Launcher::MpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Mp", Launcher::MpFloat, true, true); \
                    attributes[Launcher::MaxMpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxMp", Launcher::MaxMpFloat, true, true); \
                    attributes[Launcher::AtkSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "AtkSpeed", Launcher::AtkSpeedFloat, true, true); \
                    attributes[Launcher::CantControllerRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "CantControllerRef", Launcher::CantControllerRefInt32, true, true); \
                    attributes[Launcher::SilentRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "SilentRef", Launcher::SilentRefInt32, true, true); \
                    attributes[Launcher::InvincibleRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "InvincibleRef", Launcher::InvincibleRefInt32, true, true); \
                    attributes[Launcher::RestraintRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "RestraintRef", Launcher::RestraintRefInt32, true, true); \
                    attributes[Launcher::ShiftRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "ShiftRef", Launcher::ShiftRefInt32, true, true); \
                    attributes[Launcher::ExtraAddExpFloat] = Attribute::Create(go, eAttributeType::__Float__, "ExtraAddExp", Launcher::ExtraAddExpFloat, false, false); \
                    attributes[Launcher::MoveSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "MoveSkillConfigID", Launcher::MoveSkillConfigIDInt64, false, false); \
                    attributes[Launcher::SkillAngleFloat] = Attribute::Create(go, eAttributeType::__Float__, "SkillAngle", Launcher::SkillAngleFloat, false, false); \
                    attributes[Launcher::MoveSkillTargetGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "MoveSkillTargetGuid", Launcher::MoveSkillTargetGuidUInt64, false, false); \
                    attributes[Launcher::SkillTargetPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "SkillTargetPos", Launcher::SkillTargetPosVector2, false, false); \
                    attributes[Launcher::DamagePriorityInt32] = Attribute::Create(go, eAttributeType::__Int32__, "DamagePriority", Launcher::DamagePriorityInt32, true, true); \
                    attributes[Launcher::LastSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastSkillConfigID", Launcher::LastSkillConfigIDInt64, false, false); \
                    attributes[Launcher::LastSkillUseTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastSkillUseTime", Launcher::LastSkillUseTimeUInt64, false, false); \
                    attributes[Launcher::CastIsTrackOnCollisionBool] = Attribute::Create(go, eAttributeType::__Bool__, "CastIsTrackOnCollision", Launcher::CastIsTrackOnCollisionBool, false, false); \
                    attributes[Launcher::IsCantDamagedBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsCantDamaged", Launcher::IsCantDamagedBool, true, true); \
                    attributes[Launcher::TeamGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "TeamGuid", Launcher::TeamGuidUInt64, true, true); \
                    attributes[Launcher::MoveSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "MoveSpeed", Launcher::MoveSpeedFloat, true, true); \
                    attributes[Launcher::HpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "HpRecover", Launcher::HpRecoverFloat, true, true); \
                    attributes[Launcher::MpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "MpRecover", Launcher::MpRecoverFloat, true, true); \
                    attributes[Launcher::IsTeamLeaderBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsTeamLeader", Launcher::IsTeamLeaderBool, true, true); \
                    attributes[Launcher::BattleStartTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "BattleStartTime", Launcher::BattleStartTimeUInt64, true, false); \
                    attributes[Launcher::HostGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "HostGuid", Launcher::HostGuidUInt64, false, false); \
                    attributes[Launcher::TargetGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "TargetGuid", Launcher::TargetGuidUInt64, false, false); \
                    attributes[Launcher::TargetPosXFloat] = Attribute::Create(go, eAttributeType::__Float__, "TargetPosX", Launcher::TargetPosXFloat, false, false); \
                    attributes[Launcher::TargetPosYFloat] = Attribute::Create(go, eAttributeType::__Float__, "TargetPosY", Launcher::TargetPosYFloat, false, false); \
                    }, \
                    Launcher::Types, \
                    Launcher::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::LootBox: \
        { \
            go = GameObject::Create("LootBox", type, guid, config, LootBox::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    }, \
                    LootBox::Types, \
                    LootBox::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Monster: \
        { \
            go = GameObject::Create("Monster", type, guid, config, Monster::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Monster::SpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "Speed", Monster::SpeedFloat, true, true); \
                    attributes[Monster::SceneGUIDUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SceneGUID", Monster::SceneGUIDUInt64, false, false); \
                    attributes[Monster::LastPublicSceneConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastPublicSceneConfigID", Monster::LastPublicSceneConfigIDInt64, false, false); \
                    attributes[Monster::CopySceneGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "CopySceneGuid", Monster::CopySceneGuidUInt64, false, false); \
                    attributes[Monster::NicknameString] = Attribute::Create(go, eAttributeType::__String__, "Nickname", Monster::NicknameString, false, false); \
                    attributes[Monster::LevelInt64] = Attribute::Create(go, eAttributeType::__Int64__, "Level", Monster::LevelInt64, true, true); \
                    attributes[Monster::LastUpdatePositionTickUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastUpdatePositionTick", Monster::LastUpdatePositionTickUInt64, false, false); \
                    attributes[Monster::WhoAttkMeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "WhoAttkMe", Monster::WhoAttkMeUInt64, false, false); \
                    attributes[Monster::AtkFloat] = Attribute::Create(go, eAttributeType::__Float__, "Atk", Monster::AtkFloat, true, true); \
                    attributes[Monster::HpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Hp", Monster::HpFloat, true, true); \
                    attributes[Monster::MaxHpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxHp", Monster::MaxHpFloat, true, true); \
                    attributes[Monster::CriticalFloat] = Attribute::Create(go, eAttributeType::__Float__, "Critical", Monster::CriticalFloat, true, true); \
                    attributes[Monster::CriticalDamageFloat] = Attribute::Create(go, eAttributeType::__Float__, "CriticalDamage", Monster::CriticalDamageFloat, true, true); \
                    attributes[Monster::DefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "Defend", Monster::DefendFloat, true, true); \
                    attributes[Monster::MagicDefendFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicDefend", Monster::MagicDefendFloat, true, true); \
                    attributes[Monster::BloodFloat] = Attribute::Create(go, eAttributeType::__Float__, "Blood", Monster::BloodFloat, true, true); \
                    attributes[Monster::MagicAttackFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicAttack", Monster::MagicAttackFloat, true, true); \
                    attributes[Monster::PhysicalPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierce", Monster::PhysicalPierceFloat, true, true); \
                    attributes[Monster::PhysicalPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "PhysicalPierceRate", Monster::PhysicalPierceRateFloat, true, true); \
                    attributes[Monster::MagicPierceFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierce", Monster::MagicPierceFloat, true, true); \
                    attributes[Monster::MagicPierceRateFloat] = Attribute::Create(go, eAttributeType::__Float__, "MagicPierceRate", Monster::MagicPierceRateFloat, true, true); \
                    attributes[Monster::AffordFloat] = Attribute::Create(go, eAttributeType::__Float__, "Afford", Monster::AffordFloat, true, true); \
                    attributes[Monster::DodgeFloat] = Attribute::Create(go, eAttributeType::__Float__, "Dodge", Monster::DodgeFloat, true, true); \
                    attributes[Monster::ResilienceFloat] = Attribute::Create(go, eAttributeType::__Float__, "Resilience", Monster::ResilienceFloat, true, true); \
                    attributes[Monster::VisualFieldFloat] = Attribute::Create(go, eAttributeType::__Float__, "VisualField", Monster::VisualFieldFloat, true, true); \
                    attributes[Monster::DamageReductionFloat] = Attribute::Create(go, eAttributeType::__Float__, "DamageReduction", Monster::DamageReductionFloat, true, true); \
                    attributes[Monster::MpFloat] = Attribute::Create(go, eAttributeType::__Float__, "Mp", Monster::MpFloat, true, true); \
                    attributes[Monster::MaxMpFloat] = Attribute::Create(go, eAttributeType::__Float__, "MaxMp", Monster::MaxMpFloat, true, true); \
                    attributes[Monster::AtkSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "AtkSpeed", Monster::AtkSpeedFloat, true, true); \
                    attributes[Monster::CantControllerRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "CantControllerRef", Monster::CantControllerRefInt32, true, true); \
                    attributes[Monster::SilentRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "SilentRef", Monster::SilentRefInt32, true, true); \
                    attributes[Monster::InvincibleRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "InvincibleRef", Monster::InvincibleRefInt32, true, true); \
                    attributes[Monster::RestraintRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "RestraintRef", Monster::RestraintRefInt32, true, true); \
                    attributes[Monster::ShiftRefInt32] = Attribute::Create(go, eAttributeType::__Int32__, "ShiftRef", Monster::ShiftRefInt32, true, true); \
                    attributes[Monster::ExtraAddExpFloat] = Attribute::Create(go, eAttributeType::__Float__, "ExtraAddExp", Monster::ExtraAddExpFloat, false, false); \
                    attributes[Monster::MoveSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "MoveSkillConfigID", Monster::MoveSkillConfigIDInt64, false, false); \
                    attributes[Monster::SkillAngleFloat] = Attribute::Create(go, eAttributeType::__Float__, "SkillAngle", Monster::SkillAngleFloat, false, false); \
                    attributes[Monster::MoveSkillTargetGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "MoveSkillTargetGuid", Monster::MoveSkillTargetGuidUInt64, false, false); \
                    attributes[Monster::SkillTargetPosVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "SkillTargetPos", Monster::SkillTargetPosVector2, false, false); \
                    attributes[Monster::DamagePriorityInt32] = Attribute::Create(go, eAttributeType::__Int32__, "DamagePriority", Monster::DamagePriorityInt32, true, true); \
                    attributes[Monster::LastSkillConfigIDInt64] = Attribute::Create(go, eAttributeType::__Int64__, "LastSkillConfigID", Monster::LastSkillConfigIDInt64, false, false); \
                    attributes[Monster::LastSkillUseTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastSkillUseTime", Monster::LastSkillUseTimeUInt64, false, false); \
                    attributes[Monster::CastIsTrackOnCollisionBool] = Attribute::Create(go, eAttributeType::__Bool__, "CastIsTrackOnCollision", Monster::CastIsTrackOnCollisionBool, false, false); \
                    attributes[Monster::IsCantDamagedBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsCantDamaged", Monster::IsCantDamagedBool, true, true); \
                    attributes[Monster::TeamGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "TeamGuid", Monster::TeamGuidUInt64, true, true); \
                    attributes[Monster::MoveSpeedFloat] = Attribute::Create(go, eAttributeType::__Float__, "MoveSpeed", Monster::MoveSpeedFloat, true, true); \
                    attributes[Monster::HpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "HpRecover", Monster::HpRecoverFloat, true, true); \
                    attributes[Monster::MpRecoverFloat] = Attribute::Create(go, eAttributeType::__Float__, "MpRecover", Monster::MpRecoverFloat, true, true); \
                    attributes[Monster::IsTeamLeaderBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsTeamLeader", Monster::IsTeamLeaderBool, true, true); \
                    attributes[Monster::BattleStartTimeUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "BattleStartTime", Monster::BattleStartTimeUInt64, true, false); \
                    attributes[Monster::RefreshRuleGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "RefreshRuleGuid", Monster::RefreshRuleGuidUInt64, false, false); \
                    attributes[Monster::TargetUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "Target", Monster::TargetUInt64, false, false); \
                    attributes[Monster::BronPositionVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "BronPosition", Monster::BronPositionVector2, false, false); \
                    attributes[Monster::IsPreviousSuccessBool] = Attribute::Create(go, eAttributeType::__Bool__, "IsPreviousSuccess", Monster::IsPreviousSuccessBool, false, false); \
                    attributes[Monster::PreviousPositionVector2] = Attribute::Create(go, eAttributeType::__Vector2__, "PreviousPosition", Monster::PreviousPositionVector2, false, false); \
                    }, \
                    Monster::Types, \
                    Monster::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Npc: \
        { \
            go = GameObject::Create("Npc", type, guid, config, Npc::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    }, \
                    Npc::Types, \
                    Npc::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::RefreshRule: \
        { \
            go = GameObject::Create("RefreshRule", type, guid, config, RefreshRule::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[RefreshRule::SceneGuidUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "SceneGuid", RefreshRule::SceneGuidUInt64, false, false); \
                    attributes[RefreshRule::CurrentRefreshCountInt32] = Attribute::Create(go, eAttributeType::__Int32__, "CurrentRefreshCount", RefreshRule::CurrentRefreshCountInt32, false, false); \
                    attributes[RefreshRule::TotleRefreshCountInt32] = Attribute::Create(go, eAttributeType::__Int32__, "TotleRefreshCount", RefreshRule::TotleRefreshCountInt32, false, false); \
                    }, \
                    RefreshRule::Types, \
                    RefreshRule::TypesCount, \
                file, line); \
            break; \
        } \
        case eGameObjectType::Skill: \
        { \
            go = GameObject::Create("Skill", type, guid, config, Skill::__AttributeCount__, \
                [](GameObject* const go, Attribute** const attributes) { \
                    attributes[Skill::AccountUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "Account", Skill::AccountUInt64, false, false); \
                    attributes[Skill::CharacterIDUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "CharacterID", Skill::CharacterIDUInt64, false, false); \
                    attributes[Skill::SkillSlotString] = Attribute::Create(go, eAttributeType::__String__, "SkillSlot", Skill::SkillSlotString, false, false); \
                    attributes[Skill::LastCastTickUInt64] = Attribute::Create(go, eAttributeType::__UInt64__, "LastCastTick", Skill::LastCastTickUInt64, false, false); \
                    }, \
                    Skill::Types, \
                    Skill::TypesCount, \
                file, line); \
            break; \
        } \
}


}

#endif //__UnitCommon_h__
