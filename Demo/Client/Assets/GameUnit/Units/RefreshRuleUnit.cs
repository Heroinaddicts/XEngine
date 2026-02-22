using System;
using XUtils;

namespace GameUnit
{
    public class eRefreshRuleAttr
    {
        public const int SceneGuid_UInt64 = 0; //
        public const int CurrentRefreshCount_Int32 = 1; //test
        public const int TotleRefreshCount_Int32 = 2; //
    }

    class RefreshRuleUnit : Unit
    {

        public override string ToString()
        {
            return $"RefreshRuleUnit : {_Guid}";
        }

        public override void OnInitialize(in UInt64 guid, in Int64 configId, in string file, in int line)
        {
            _Guid = guid;
            _Type = eUnitType.RefreshRule;
            _ConfigID = configId;
            _Name = "RefreshRule";

            _Attributes = new Attribute[]
            {
                new Attribute(this, eAttributeType.UInt64, eRefreshRuleAttr.SceneGuid_UInt64, "SceneGuid", false, false, false, false),
                new Attribute(this, eAttributeType.Int32, eRefreshRuleAttr.CurrentRefreshCount_Int32, "CurrentRefreshCount", false, false, false, false),
                new Attribute(this, eAttributeType.Int32, eRefreshRuleAttr.TotleRefreshCount_Int32, "TotleRefreshCount", false, false, false, false),
            };
        }

        public override void OnCreate(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.RefreshRule, this, false, file, line);

        }

        public override void OnRelease(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.RefreshRule, this, false, file, line);

        }

        public override bool Is(in eUnitType type)
        {
            return _Type == eUnitType.RefreshRule;
        }
    }
}
