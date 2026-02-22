using System;
using XUtils;

namespace GameUnit
{
    public class eBuffAttr
    {
        public const int CasterGuid_UInt64 = 0; //
        public const int HostGuid_UInt64 = 1; //
        public const int ChangeValue_Float = 2; //
        public const int CreateTimeTick_UInt64 = 3; //
    }

    class BuffUnit : Unit
    {

        public override string ToString()
        {
            return $"BuffUnit : {_Guid}";
        }

        public override void OnInitialize(in UInt64 guid, in Int64 configId, in string file, in int line)
        {
            _Guid = guid;
            _Type = eUnitType.Buff;
            _ConfigID = configId;
            _Name = "Buff";

            _Attributes = new Attribute[]
            {
                new Attribute(this, eAttributeType.UInt64, eBuffAttr.CasterGuid_UInt64, "CasterGuid", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eBuffAttr.HostGuid_UInt64, "HostGuid", false, false, false, false),
                new Attribute(this, eAttributeType.Float, eBuffAttr.ChangeValue_Float, "ChangeValue", false, false, false, false),
                new Attribute(this, eAttributeType.UInt64, eBuffAttr.CreateTimeTick_UInt64, "CreateTimeTick", false, false, false, false),
            };
        }

        public override void OnCreate(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.Buff, this, false, file, line);

        }

        public override void OnRelease(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.Buff, this, false, file, line);

        }

        public override bool Is(in eUnitType type)
        {
            return _Type == eUnitType.Buff;
        }
    }
}
