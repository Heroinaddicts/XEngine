using System;
using UnityEngine;

namespace GameUnit
{
    public enum eAttributeType
    {
        UInt16 = 0,
        Int16 = 1,
        UInt32 = 2,
        Int32 = 3,
        UInt64 = 4,
        Int64 = 5,
        Float = 6,
        String = 7,
        Bool = 8,
        Vector2 = 9,
        Vector3 = 10,
        Vector4 = 11,

        Unknown
    }

    public enum eUnitType
    {
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

        __Count__ = 14
    }

    static class UnityCreator
    {
        public static Unit CreateUnit(in UInt64 guid, in Int64 configId, in eUnitType type, in GameObject go)
        {
            Unit unit = null;
            switch (type)
            {
                case eUnitType.Boss:
                    {
                        unit = go.AddComponent<BossUnit>();
                        return unit;
                    }
                case eUnitType.Buff:
                    {
                        unit = go.AddComponent<BuffUnit>();
                        return unit;
                    }
                case eUnitType.Bullet:
                    {
                        unit = go.AddComponent<BulletUnit>();
                        return unit;
                    }
                case eUnitType.Character:
                    {
                        unit = go.AddComponent<CharacterUnit>();
                        return unit;
                    }
                case eUnitType.Creature:
                    {
                        unit = go.AddComponent<CreatureUnit>();
                        return unit;
                    }
                case eUnitType.Entity:
                    {
                        unit = go.AddComponent<EntityUnit>();
                        return unit;
                    }
                case eUnitType.Equip:
                    {
                        unit = go.AddComponent<EquipUnit>();
                        return unit;
                    }
                case eUnitType.Item:
                    {
                        unit = go.AddComponent<ItemUnit>();
                        return unit;
                    }
                case eUnitType.Launcher:
                    {
                        unit = go.AddComponent<LauncherUnit>();
                        return unit;
                    }
                case eUnitType.LootBox:
                    {
                        unit = go.AddComponent<LootBoxUnit>();
                        return unit;
                    }
                case eUnitType.Monster:
                    {
                        unit = go.AddComponent<MonsterUnit>();
                        return unit;
                    }
                case eUnitType.Npc:
                    {
                        unit = go.AddComponent<NpcUnit>();
                        return unit;
                    }
                case eUnitType.RefreshRule:
                    {
                        unit = go.AddComponent<RefreshRuleUnit>();
                        return unit;
                    }
                case eUnitType.Skill:
                    {
                        unit = go.AddComponent<SkillUnit>();
                        return unit;
                    }

                default:
                    throw new Exception($"GameUnit type error {type}");
            }

            return null;
        }
    }

}
