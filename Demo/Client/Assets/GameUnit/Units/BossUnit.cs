using System;
using XUtils;

namespace GameUnit
{
    public class eBossAttr
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
        public const int RefreshRuleGuid_UInt64 = 50; //
        public const int Target_UInt64 = 51; //
        public const int BronPosition_Vector2 = 52; //
        public const int IsPreviousSuccess_Bool = 53; //
        public const int PreviousPosition_Vector2 = 54; //
    }

    class BossUnit : MonsterUnit
    {

        public override string ToString()
        {
            return $"BossUnit : {_Guid}";
        }

        public override void OnInitialize(in UInt64 guid, in Int64 configId, in string file, in int line)
        {
            _Guid = guid;
            _Type = eUnitType.Boss;
            _ConfigID = configId;
            _Name = "Boss";

            _Attributes = new Attribute[]
            {
                new Attribute(this, eAttributeType.Float, eBossAttr.Speed_Float, "Speed", true, true, false, false),
                new Attribute(this, eAttributeType.UInt64, eBossAttr.SceneGUID_UInt64, "SceneGUID", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eBossAttr.LastPublicSceneConfigID_Int64, "LastPublicSceneConfigID", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eBossAttr.CopySceneGuid_UInt64, "CopySceneGuid", false, false, false, false),
                new Attribute(this, eAttributeType.String, eBossAttr.Nickname_String, "Nickname", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eBossAttr.Level_Int64, "Level", true, true, false, false),
                new Attribute(this, eAttributeType.UInt64, eBossAttr.LastUpdatePositionTick_UInt64, "LastUpdatePositionTick", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eBossAttr.WhoAttkMe_UInt64, "WhoAttkMe", false, false, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.Atk_Float, "Atk", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.Hp_Float, "Hp", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.MaxHp_Float, "MaxHp", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.Critical_Float, "Critical", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.CriticalDamage_Float, "CriticalDamage", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.Defend_Float, "Defend", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.MagicDefend_Float, "MagicDefend", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.Blood_Float, "Blood", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.MagicAttack_Float, "MagicAttack", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.PhysicalPierce_Float, "PhysicalPierce", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.PhysicalPierceRate_Float, "PhysicalPierceRate", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.MagicPierce_Float, "MagicPierce", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.MagicPierceRate_Float, "MagicPierceRate", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.Afford_Float, "Afford", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.Dodge_Float, "Dodge", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.Resilience_Float, "Resilience", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.VisualField_Float, "VisualField", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.DamageReduction_Float, "DamageReduction", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.Mp_Float, "Mp", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.MaxMp_Float, "MaxMp", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.AtkSpeed_Float, "AtkSpeed", true, true, false, false),
                new Attribute(this, eAttributeType.Int32, eBossAttr.CantControllerRef_Int32, "CantControllerRef", true, true, false, false),
                new Attribute(this, eAttributeType.Int32, eBossAttr.SilentRef_Int32, "SilentRef", true, true, false, false),
                new Attribute(this, eAttributeType.Int32, eBossAttr.InvincibleRef_Int32, "InvincibleRef", true, true, false, false),
                new Attribute(this, eAttributeType.Int32, eBossAttr.RestraintRef_Int32, "RestraintRef", true, true, false, false),
                new Attribute(this, eAttributeType.Int32, eBossAttr.ShiftRef_Int32, "ShiftRef", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.ExtraAddExp_Float, "ExtraAddExp", false, false, false, false),
                new Attribute(this, eAttributeType.Int64, eBossAttr.MoveSkillConfigID_Int64, "MoveSkillConfigID", false, false, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.SkillAngle_Float, "SkillAngle", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eBossAttr.MoveSkillTargetGuid_UInt64, "MoveSkillTargetGuid", false, false, false, false),
                new Attribute(this, eAttributeType.Vector2, eBossAttr.SkillTargetPos_Vector2, "SkillTargetPos", false, false, false, false),
                new Attribute(this, eAttributeType.Int32, eBossAttr.DamagePriority_Int32, "DamagePriority", true, true, false, false),
                new Attribute(this, eAttributeType.Int64, eBossAttr.LastSkillConfigID_Int64, "LastSkillConfigID", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eBossAttr.LastSkillUseTime_UInt64, "LastSkillUseTime", false, false, false, false),
                new Attribute(this, eAttributeType.Bool, eBossAttr.CastIsTrackOnCollision_Bool, "CastIsTrackOnCollision", false, false, false, false),
                new Attribute(this, eAttributeType.Bool, eBossAttr.IsCantDamaged_Bool, "IsCantDamaged", true, true, false, false),
                new Attribute(this, eAttributeType.UInt64, eBossAttr.TeamGuid_UInt64, "TeamGuid", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.MoveSpeed_Float, "MoveSpeed", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.HpRecover_Float, "HpRecover", true, true, false, false),
                new Attribute(this, eAttributeType.Float, eBossAttr.MpRecover_Float, "MpRecover", true, true, false, false),
                new Attribute(this, eAttributeType.Bool, eBossAttr.IsTeamLeader_Bool, "IsTeamLeader", true, true, false, false),
                new Attribute(this, eAttributeType.UInt64, eBossAttr.BattleStartTime_UInt64, "BattleStartTime", true, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eBossAttr.RefreshRuleGuid_UInt64, "RefreshRuleGuid", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eBossAttr.Target_UInt64, "Target", false, false, false, false),
                new Attribute(this, eAttributeType.Vector2, eBossAttr.BronPosition_Vector2, "BronPosition", false, false, false, false),
                new Attribute(this, eAttributeType.Bool, eBossAttr.IsPreviousSuccess_Bool, "IsPreviousSuccess", false, false, false, false),
                new Attribute(this, eAttributeType.Vector2, eBossAttr.PreviousPosition_Vector2, "PreviousPosition", false, false, false, false),
            };
        }

        public override void OnCreate(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.Entity, this, true, file, line);
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.Creature, this, true, file, line);
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.Monster, this, true, file, line);
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.Boss, this, false, file, line);

        }

        public override void OnRelease(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.Boss, this, false, file, line);
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.Monster, this, true, file, line);
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.Creature, this, true, file, line);
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.Entity, this, true, file, line);

        }

        public override bool Is(in eUnitType type)
        {
            return _Type == eUnitType.Boss || _Type == eUnitType.Monster || _Type == eUnitType.Creature || _Type == eUnitType.Entity;
        }
    }
}
