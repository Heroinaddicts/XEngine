using System;

namespace AutoConfig
{
    public static class AutoConst
    {
        public static class AllowableError
        {
            public static readonly float PositionErrorRange = 2.5f;
        }

        public static class Aoi
        {
            public static readonly Int64 AoiGridSize = 1;
            public static readonly Int64 DefaultVisualSize = 25;
            public static readonly Int64 Visual = 250;
        }

        public static class Attribute
        {
            public static readonly string Atk = "物理攻击";
            public static readonly string Blood = "吸血";
            public static readonly string Critical = "暴击率";
            public static readonly string CriticalDamage = "暴击伤害";
            public static readonly string Defend = "物理防御";
            public static readonly string Dodge = "闪避";
            public static readonly string Hp = "血量";
            public static readonly string MagicAttack = "法术攻击";
            public static readonly string MagicDefend = "法术防御";
            public static readonly string MagicPierce = "法术穿透";
            public static readonly string PhysicalPierce = "物理穿透";
            public static readonly string Resilience = "韧性";
            public static readonly string Speed = "移速";
            public static readonly string Weight = "重量";
        }

        public static class BattleSetting
        {
            public static readonly Int64 RedScreenTime = 300;
            public static readonly Int64 SkillTolerance = 425;
        }

        public static class BuffRevelant
        {
            public static readonly string BuffPrefab = "UI/Prefabs/MainUI/UIBuffIcon";
        }

        public static class Character
        {
            public static readonly Int64 Brass = 1000000;
            public static readonly Int64 Coin = 1500;
            public static readonly Int64[] InitializeCharacterConfigID = {100001, 100005};
        }

        public static class DamageCalculate
        {
            public static readonly Int64 Fire = 200;
            public static readonly Int64 Gold = 200;
            public static readonly Int64 Physic = 300;
            public static readonly Int64 Water = 200;
        }

        public static class DamageNumber
        {
            public static readonly float CritBurstDuration = 0.14f;
            public static readonly float CritBurstScale = 3.3f;
            public static readonly float CritFadeDelay = 0.18f;
            public static readonly float CritPauseDuration = 0.28f;
            public static readonly float CritRecoilDuration = 0.12f;
            public static readonly float CritRecoilScale = 1f;
            public static readonly float CritRiseDuration = 0.05f;
            public static readonly float CritRiseHeight = 0.25f;
            public static readonly float CritRiseScaleEnd = 0.5f;
            public static readonly float CritRiseScaleStart = 0f;
            public static readonly string CriticalDamage = "UI/Sprites/Battle/CriticalDamageNumber";
            public static readonly float CriticalScale = 2.35f;
            public static readonly string NormalDamage = "UI/Sprites/Battle/NormalDamageNumber";
            public static readonly float NormalFadeDelay = 0.35f;
            public static readonly float NormalPauseDuration = 0.1f;
            public static readonly float NormalRiseDuration = 0.05f;
            public static readonly float NormalRiseHeight = 0.5f;
            public static readonly float NormalScale = 2.15f;
            public static readonly string SelfCriticalDamage = "UI/Sprites/Battle/CriticalDamageNumber";
            public static readonly float SelfCriticalScale = 1.55f;
            public static readonly float SelfFadeDelay = 0.33f;
            public static readonly string SelfNormalDamage = "UI/Sprites/Battle/NormalDamageNumber";
            public static readonly float SelfNormalScale = 1.55f;
            public static readonly float SelfPauseDuration = 0.08f;
            public static readonly float SelfRiseDuration = 0.05f;
            public static readonly float SelfRiseHeight = 0.15f;
            public static readonly float SelfdelayBeforeRecycle = 0.85f;
            public static readonly float SelffadeDuration = 0.85f;
            public static readonly float SelfmoveDistance = 1.25f;
            public static readonly float SelfmoveDuration = 1f;
            public static readonly float Spacing = 0.6f;
            public static readonly float delayBeforeRecycle = 0f;
            public static readonly float fadeDuration = 0.28f;
            public static readonly float moveDistance = 5f;
            public static readonly float moveDuration = 1f;
        }

        public static class GameSetting
        {
            public static readonly string BrassDrop = "UI/Sprites/Team/BlueCircle";
            public static readonly string BrassIcon = "UI/Sprites/Team/BlueCircle";
            public static readonly Int64 Camera = 12;
            public static readonly Int64 ChangeCharacter = 10000;
            public static readonly string CoinDrop = "UI/Sprites/Team/BlueCircle";
            public static readonly string CoinIcon = "UI/Sprites/Team/BlueCircle";
            public static readonly Int64 DropDelayTime = 500;
            public static readonly string DropPrefab = "RewardItem/LootItem/Prefabs/lootItem";
            public static readonly float DropRange = 2.5f;
            public static readonly Int64 DropTime = 450;
            public static readonly Int64 InitialBattleBagContain = 6;
            public static readonly Int64 InitialSafeContain = 3;
            public static readonly Int64 InitialWarHouseContain = 50;
            public static readonly float Joystick = 0f;
            public static readonly Int64 MaxBattleBagContain = 12;
            public static readonly Int64 MaxSafeContain = 6;
            public static readonly Int64 MaxWarHouseContain = 999;
            public static readonly float Skill = 0.3f;
            public static readonly float ThingArea = 2f;
        }

        public static class HpGradient
        {
            public static readonly float BossHpGradient = 1f;
            public static readonly float CharacterHpGradient = 1f;
            public static readonly float MonsterHpGradient = 1f;
        }

        public static class InitialResource
        {
            public static readonly Int64 InitialWeapon = 4004;
            public static readonly Int64 InitialWeapon2 = 4006;
        }

        public static class InitialTask
        {
            public static readonly Int64 Task = 10001;
        }

        public static class MatchRules
        {
            public static readonly Int64 Countdown = 3000;
            public static readonly Int64 MaxTime = 12000;
            public static readonly Int64 PeopleLimit = 16;
            public static readonly Int64 Team = 6;
            public static readonly Int64 TimeLen = 1000;
        }

        public static class Monster
        {
            public static readonly Int64 TraceInterval = 1000;
        }

        public static class MonsterBossIdle
        {
            public static readonly Int64 HydraCypress = 0;
        }

        public static class Npc
        {
            public static readonly Int64 NpcTriggle = 10;
        }

        public static class PackRules
        {
            public static readonly Int64 MaxCount = 99;
            public static readonly float[] PackSlot = {5f, 6f};
        }

        public static class Partner
        {
            public static readonly Int64 CountLimit = 4;
            public static readonly string EnemyCircle = "UI/Sprites/Team/BlueCircle";
            public static readonly float[] EnemyHp = {1f, 0.25f, 0f};
            public static readonly Int64 PartTime = 15000;
            public static readonly string SelfCircle = "UI/Sprites/Team/YellowCircle";
            public static readonly float[] SelfHp = {0.26f, 1f, 0f};
            public static readonly string TeamCircle = "UI/Sprites/Team/BlueCircle";
            public static readonly float[] TeamHp = {0f, 0.76f, 1f};
        }

        public static class Pop
        {
            public static readonly string BigDetail = "xxxx";
            public static readonly string CommonPop = "xxxx";
            public static readonly string SlidePop = "xxxx";
            public static readonly string SmallDetail = "xxxx";
            public static readonly string Tips = "xxxxx";
        }

        public static class PositionSolide
        {
            public static readonly Int64 Solide = 33;
        }

        public static class RhombusMap
        {
            public static readonly float[] RhombusAngles = {26.57f,  153.43f,  206.57f,  333.43f};
        }

        public static class Skill
        {
            public static readonly string EnemySelected = "xx";
            public static readonly Int64 NotInSlot = -1;
        }

        public static class Sounds
        {
            public static readonly Int64 React = 4011;
        }

        public static class Tags
        {
            public static readonly string EnemyTag = "Sprites/Tags/EnemyTag";
            public static readonly string SelfTag = "Sprites/Tags/SelfTag";
            public static readonly string TeammateTag = "Sprites/Tags/TeammateTag";
        }

        public static class UI
        {
            public static readonly string BattleTopUI = "UI/Prefabs/TopUI/BattleTopUI";
            public static readonly string Monster = "UI/Prefabs/TopUI/MonsterTopUI";
            public static readonly string TopUI = "UI/Prefabs/TopUI/TopUI";
        }

        public static class World
        {
            public static readonly Int64 DefaultWorld = 20001;
        }

    }

}
