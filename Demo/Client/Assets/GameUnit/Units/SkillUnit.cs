using System;
using XUtils;

namespace GameUnit
{
    public class eSkillAttr
    {
        public const int Account_UInt64 = 0; //
        public const int CharacterID_UInt64 = 1; //
        public const int SkillSlot_String = 2; //
        public const int LastCastTick_UInt64 = 3; //
    }

    class SkillUnit : Unit
    {

        public override string ToString()
        {
            return $"SkillUnit : {_Guid}";
        }

        public override void OnInitialize(in UInt64 guid, in Int64 configId, in string file, in int line)
        {
            _Guid = guid;
            _Type = eUnitType.Skill;
            _ConfigID = configId;
            _Name = "Skill";

            _Attributes = new Attribute[]
            {
                new Attribute(this, eAttributeType.UInt64, eSkillAttr.Account_UInt64, "Account", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eSkillAttr.CharacterID_UInt64, "CharacterID", false, false, false, false),
                new Attribute(this, eAttributeType.String, eSkillAttr.SkillSlot_String, "SkillSlot", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eSkillAttr.LastCastTick_UInt64, "LastCastTick", false, false, false, false),
            };
        }

        public override void OnCreate(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.Skill, this, false, file, line);

        }

        public override void OnRelease(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.Skill, this, false, file, line);

        }

        public override bool Is(in eUnitType type)
        {
            return _Type == eUnitType.Skill;
        }
    }
}
