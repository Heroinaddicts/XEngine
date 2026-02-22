using System;
using XUtils;

namespace GameUnit
{
    public class eLootBoxAttr
    {

    }

    class LootBoxUnit : Unit
    {

        public override string ToString()
        {
            return $"LootBoxUnit : {_Guid}";
        }

        public override void OnInitialize(in UInt64 guid, in Int64 configId, in string file, in int line)
        {
            _Guid = guid;
            _Type = eUnitType.LootBox;
            _ConfigID = configId;
            _Name = "LootBox";

            _Attributes = new Attribute[]
            {

            };
        }

        public override void OnCreate(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Create, eUnitType.LootBox, this, false, file, line);

        }

        public override void OnRelease(in string file, in int line)
        {
            UnitFactory.GetInstance().TriggerUnitEvent(eUnitEvent.Released, eUnitType.LootBox, this, false, file, line);

        }

        public override bool Is(in eUnitType type)
        {
            return _Type == eUnitType.LootBox;
        }
    }
}
