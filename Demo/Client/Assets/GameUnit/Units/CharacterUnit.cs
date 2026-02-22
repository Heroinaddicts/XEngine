using System;
using XUtils;

namespace GameUnit
{
    public class eCharacterAttr
    {
        public const int Speed_Float = 0; //
        public const int SceneGUID_UInt64 = 1; //
        public const int LastPublicSceneConfigID_Int64 = 2; //
        public const int CopySceneGuid_UInt64 = 3; //
        public const int Nickname_String = 4; //
        public const int Level_Int64 = 5; //
        public const int LastUpdatePositionTick_UInt64 = 6; //
        public const int WhoAttkMe_UInt64 = 7; //
        public const int Atk_Float = 8; //
        public const int Hp_Float = 9; //
        public const int MaxHp_Float = 10; //
        public const int Critical_Float = 11; //
        public const int CriticalDamage_Float = 12; //
        public const int Defend_Float = 13; //
        public const int MagicDefend_Float = 14; //
        public const int Blood_Float = 15; //
        public const int MagicAttack_Float = 16; //
        public const int PhysicalPierce_Float = 17; //
        public const int PhysicalPierceRate_Float = 18; //
        public const int MagicPierce_Float = 19; //
        public const int MagicPierceRate_Float = 20; //
        public const int Afford_Float = 21; //
        public const int Dodge_Float = 22; //
        public const int Resilience_Float = 23; //
        public const int VisualField_Float = 24; //
        public const int DamageReduction_Float = 25; //
        public const int Mp_Float = 26; //
        public const int MaxMp_Float = 27; //
        public const int AtkSpeed_Float = 28; //
        public const int CantControllerRef_Int32 = 29; //
        public const int SilentRef_Int32 = 30; //
        public const int InvincibleRef_Int32 = 31; //
        public const int RestraintRef_Int32 = 32; //
        public const int ShiftRef_Int32 = 33; //
        public const int ExtraAddExp_Float = 34; //
        public const int MoveSkillConfigID_Int64 = 35; //
        public const int SkillAngle_Float = 36; //
        public const int MoveSkillTargetGuid_UInt64 = 37; //
        public const int SkillTargetPos_Vector2 = 38; //
        public const int DamagePriority_Int32 = 39; //
        public const int LastSkillConfigID_Int64 = 40; //
        public const int LastSkillUseTime_UInt64 = 41; //
        public const int CastIsTrackOnCollision_Bool = 42; //
        public const int IsCantDamaged_Bool = 43; //
        public const int TeamGuid_UInt64 = 44; //
        public const int MoveSpeed_Float = 45; //
        public const int HpRecover_Float = 46; //
        public const int MpRecover_Float = 47; //
        public const int IsTeamLeader_Bool = 48; //
        public const int BattleStartTime_UInt64 = 49; //
        public const int Account_UInt64 = 50; //
        public const int Exp_Int64 = 51; //
        public const int SlotCloths_UInt64 = 52; //
        public const int ClothsConfig_Int64 = 53; //
        public const int SlotPants_UInt64 = 54; //
        public const int PantsConfig_Int64 = 55; //
        public const int SlotBelt_UInt64 = 56; //
        public const int BeltConfig_Int64 = 57; //
        public const int SlotShoes_UInt64 = 58; //
        public const int ShoesConfig_Int64 = 59; //
        public const int SlotJewelry_UInt64 = 60; //
        public const int JewelryConfig_Int64 = 61; //
        public const int SlotWeapon_UInt64 = 62; //
        public const int WeaponConfig_Int64 = 63; //
        public const int MoveTargetGuid_UInt64 = 64; //
    }

    class CharacterUnit : CreatureUnit
    {

        public override string ToString()
        {
            return $"CharacterUnit : {_Guid}";
        }

        public override void OnInitialize(in UInt64 guid, in Int64 configId, in string file, in int line)
        {
            _Guid = guid;
            _Type = eUnitType.Character;
            _ConfigID = configId;
            _Name = "Character";

            _Attributes = new Attribute[]
            {
                new Attribute(this, eAttributeType.Float, eCharacterAttr.Speed_Float, "Speed", true, true, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.SceneGUID_UInt64, "SceneGUID", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.LastPublicSceneConfigID_Int64, "LastPublicSceneConfigID", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.CopySceneGuid_UInt64, "CopySceneGuid", false, false, false, false),
                new Attribute(this, eAttributeType.String, eCharacterAttr.Nickname_String, "Nickname", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.Level_Int64, "Level", true, true, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.LastUpdatePositionTick_UInt64, "LastUpdatePositionTick", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.WhoAttkMe_UInt64, "WhoAttkMe", false, false, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.Atk_Float, "Atk", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.Hp_Float, "Hp", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.MaxHp_Float, "MaxHp", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.Critical_Float, "Critical", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.CriticalDamage_Float, "CriticalDamage", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.Defend_Float, "Defend", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.MagicDefend_Float, "MagicDefend", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.Blood_Float, "Blood", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.MagicAttack_Float, "MagicAttack", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.PhysicalPierce_Float, "PhysicalPierce", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.PhysicalPierceRate_Float, "PhysicalPierceRate", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.MagicPierce_Float, "MagicPierce", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.MagicPierceRate_Float, "MagicPierceRate", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.Afford_Float, "Afford", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.Dodge_Float, "Dodge", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.Resilience_Float, "Resilience", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.VisualField_Float, "VisualField", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.DamageReduction_Float, "DamageReduction", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.Mp_Float, "Mp", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.MaxMp_Float, "MaxMp", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.AtkSpeed_Float, "AtkSpeed", true, true, false, false),
                new Attribute(this, eAttributeType.Int32, eCharacterAttr.CantControllerRef_Int32, "CantControllerRef", true, true, false, false),
                new Attribute(this, eAttributeType.Int32, eCharacterAttr.SilentRef_Int32, "SilentRef", true, true, false, false),
                new Attribute(this, eAttributeType.Int32, eCharacterAttr.InvincibleRef_Int32, "InvincibleRef", true, true, false, false),
                new Attribute(this, eAttributeType.Int32, eCharacterAttr.RestraintRef_Int32, "RestraintRef", true, true, false, false),
                new Attribute(this, eAttributeType.Int32, eCharacterAttr.ShiftRef_Int32, "ShiftRef", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.ExtraAddExp_Float, "ExtraAddExp", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.MoveSkillConfigID_Int64, "MoveSkillConfigID", false, false, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.SkillAngle_Float, "SkillAngle", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.MoveSkillTargetGuid_UInt64, "MoveSkillTargetGuid", false, false, false, false),
                new Attribute(this, eAttributeType.Vector2, eCharacterAttr.SkillTargetPos_Vector2, "SkillTargetPos", false, false, false, false),
                new Attribute(this, eAttributeType.Int32, eCharacterAttr.DamagePriority_Int32, "DamagePriority", true, true, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.LastSkillConfigID_Int64, "LastSkillConfigID", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.LastSkillUseTime_UInt64, "LastSkillUseTime", false, false, false, false),
                new Attribute(this, eAttributeType.Bool, eCharacterAttr.CastIsTrackOnCollision_Bool, "CastIsTrackOnCollision", false, false, false, false),
                new Attribute(this, eAttributeType.Bool, eCharacterAttr.IsCantDamaged_Bool, "IsCantDamaged", true, true, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.TeamGuid_UInt64, "TeamGuid", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.MoveSpeed_Float, "MoveSpeed", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.HpRecover_Float, "HpRecover", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eCharacterAttr.MpRecover_Float, "MpRecover", true, true, false, false),
                new Attribute(this, eAttributeType.Bool, eCharacterAttr.IsTeamLeader_Bool, "IsTeamLeader", true, true, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.BattleStartTime_UInt64, "BattleStartTime", true, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.Account_UInt64, "Account", true, true, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.Exp_Int64, "Exp", true, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.SlotCloths_UInt64, "SlotCloths", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.ClothsConfig_Int64, "ClothsConfig", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.SlotPants_UInt64, "SlotPants", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.PantsConfig_Int64, "PantsConfig", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.SlotBelt_UInt64, "SlotBelt", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.BeltConfig_Int64, "BeltConfig", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.SlotShoes_UInt64, "SlotShoes", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.ShoesConfig_Int64, "ShoesConfig", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.SlotJewelry_UInt64, "SlotJewelry", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.JewelryConfig_Int64, "JewelryConfig", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.SlotWeapon_UInt64, "SlotWeapon", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eCharacterAttr.WeaponConfig_Int64, "WeaponConfig", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eCharacterAttr.MoveTargetGuid_UInt64, "MoveTargetGuid", false, false, false, false),
            };
        }

        public override void OnCreate(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.Entity, this, true, file, line);
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.Creature, this, true, file, line);
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.Character, this, false, file, line);

        }

        public override void OnRelease(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.Character, this, false, file, line);
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.Creature, this, true, file, line);
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.Entity, this, true, file, line);

        }

        public override bool Is(in eUnitType type)
        {
            return _Type == eUnitType.Character || _Type == eUnitType.Creature || _Type == eUnitType.Entity;
        }
    }
}
