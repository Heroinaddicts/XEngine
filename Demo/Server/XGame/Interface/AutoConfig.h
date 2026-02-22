#pragma once

#include "MultiSys.h"
#include <string>
#include <vector>
#include <unordered_map>

#define UsingAutoConfig using namespace AutoConfig;
namespace AutoConfig {
    inline static const Int64 INVALID_CONFIG_ID = 0;

    namespace Enum {
        enum class ConditionalLogic { //条件逻辑
            And = 0, //和
            Or = 1, //或

            __Count__ = 2
        };

        enum class eActTrack { //动作分轨
            One = 0, //第一轨
            Two = 1, //第二轨
            Three = 2, //第三轨
            Four = 3, //第四轨
            Five = 4, //第5

            __Count__ = 5
        };

        enum class eAction { //
            None = 0, //
            Charge = 1, //
            Casting = 2, //

            __Count__ = 3
        };

        enum class eAddType { //加成类型
            Max = 0, //最大值加成
            Current = 1, //当前值加成
            None = 2, //非数值加成类型

            __Count__ = 3
        };

        enum class eAttribute { //修改属性类型
            None = 0, //不改变任何属性
            MoveSpeed = 1, //移动速度
            Attack = 2, //攻击力
            Hp = 3, //持续改变血量
            DamageReduction = 4, //减伤
            Invincible = 5, //无敌
            CantController = 6, //无法被操作控制
            Restraint = 7, //束缚(免除物理伤害)
            Critical = 8, //暴击率
            Blood = 9, //吸血
            Defend = 10, //防御
            CriticalAtk = 11, //暴击伤害
            Mockery = 12, //嘲讽
            ExpAdd = 13, //加经验
            AddDefend = 14, //增加物理防御
            AddMagicDefend = 15, //增加魔法防御
            ExpandVield = 16, //扩大可看视范围
            SuperArmor = 17, //免控霸体（不会受到击退。击飞等buff状态的影响）
            AtkSpeed = 18, //攻击速度
            Mp = 19, //改变蓝量
            Silent = 20, //沉默
            Rewind = 21, //击退

            __Count__ = 22
        };

        enum class eBallistics { //弹道类型
            Line = 0, //直线弹道（在技能行进过程中会产生效果）
            Parabolic = 1, //抛物线弹道（只在技能目的点造成效果）
            Range = 2, //范围性伤害（立即造成伤害）
            Track = 3, //跟踪弹

            __Count__ = 4
        };

        enum class eBattleType { //副本类型
            PvP = 0, //允许pvp的副本
            PvE = 1, //不允许pvp的副本只允许pve

            __Count__ = 2
        };

        enum class eBoss { //boss类型
            HydraCypress = 0, //九头蛇柏

            __Count__ = 1
        };

        enum class eCalculationMethod { //机关的状态
            Percent = 0, //百分比计算
            Normal = 1, //实际数值计算

            __Count__ = 2
        };

        enum class eConsumeType { //消耗类型
            Hp = 0, //血
            Mp = 1, //蓝
            Angry = 2, //怒气值

            __Count__ = 3
        };

        enum class eDamagePriority { //伤害优先级别分类
            First = 0, //第一
            Second = 1, //第二
            Third = 2, //第三
            Fourth = 3, //第四
            Fifth = 4, //第五
            None = 5, //无优先级

            __Count__ = 6
        };

        enum class eDamageType { //伤害属性
            Physic = 0, //物理伤害
            Fire = 1, //火属性
            Water = 2, //水属性
            Gold = 3, //金属性
            Truth = 4, //真实伤害

            __Count__ = 5
        };

        enum class eEquipmentType { //装备类型
            Shose = 0, //鞋子
            Weapon = 1, //武器
            Pants = 2, //裤子
            Cloths = 3, //衣服
            Belt = 4, //腰带
            Jewelry = 5, //饰品

            __Count__ = 6
        };

        enum class eFollowType { //技能跟随类型
            Follow = 0, //追随角色
            NoFollow = 1, //不追随角色

            __Count__ = 2
        };

        enum class eInfluenceOrganType { //影响机关的类型
            Prediction = 0, //预测
            Drop = 1, //掉落
            Recyle = 2, //回收机关

            __Count__ = 3
        };

        enum class eInteractType { //Npc的任务交互事件类型
            Accpet = 0, //Npc发放任务
            Wait = 1, //等待玩家完成任务
            Complete = 2, //Npc完成玩家任务

            __Count__ = 3
        };

        enum class eLifePanType { //生命周期
            HurtLife = 0, //在造成伤害后才能变成正常生命周期，未造成伤害灰提前结束
            Normal = 1, //正常周期

            __Count__ = 2
        };

        enum class eMonsterSkillTrack { //怪物技能是否跟踪
            EffectAll = 0, //全能听
            EffectSelf = 1, //个人能听到

            __Count__ = 2
        };

        enum class eMoveEaseType { //dotown缓动类型
            Linear = 0, //
            InQuad = 1, //
            OutQuad = 2, //
            InOutQuad = 3, //
            InCubic = 4, //
            OutCubic = 5, //
            InOutCubic = 6, //
            InQuart = 7, //
            OutQuart = 8, //
            InOutQuart = 9, //
            InSine = 10, //
            OutSine = 11, //
            InOutSine = 12, //
            InExpo = 13, //
            OutExpo = 14, //
            InOutExpo = 15, //
            InCirc = 16, //
            OutCirc = 17, //
            InOutCirc = 18, //
            InBounce = 19, //
            OutBounce = 20, //
            InOutBounce = 21, //
            InElastic = 22, //
            OutElastic = 23, //
            InOutElastic = 24, //
            InBack = 25, //
            OutBack = 26, //
            InOutBack = 27, //

            __Count__ = 28
        };

        enum class eMoveType { //位移类型
            LineMove = 0, //直线位移
            ParabolaMove = 1, //抛物线位移
            Immediate = 2, //瞬间移动
            TrackMove = 3, //跟踪位移
            EverLineMove = 4, //自由选择目的地直线位移

            __Count__ = 5
        };

        enum class eNpcCareer { //Npc职业
            Blacksmith = 0, //铁匠
            Shop = 1, //商店
            Driver = 2, //巴士司机

            __Count__ = 3
        };

        enum class eNpcEventType { //Npc身上的事件类型
            TaskInteract = 0, //任务事件
            TalkContent = 1, //谈话事件
            TransPort = 2, //传送事件
            SwitchEvent = 3, //打开界面事件

            __Count__ = 4
        };

        enum class eNpcType { //Npc的任务交互事件类型
            Normal = 0, //正常任务类Npc
            TransPort = 1, //传送类Npc

            __Count__ = 2
        };

        enum class ePhysicType { //物理分类
            Monster = 0, //怪物
            EnemyCharacter = 1, //敌对角色
            Wall = 2, //人无法穿过但子弹可以穿的墙体
            ParterCharacter = 3, //友军角色
            Organ = 4, //机关类型物品
            Npc = 5, //Npc人物
            None = 6, //无
            StrengthWall = 7, //人和子弹都无法穿过的墙

            __Count__ = 8
        };

        enum class ePosition { //发射器位置类型
            WorldPosition = 0, //摆放在世界中
            TraceHost = 1, //跟随宿主
            MountBone = 2, //挂在骨骼上
            None = 3, //用不到位置

            __Count__ = 4
        };

        enum class eRangeType { //范围类型
            Sector = 0, //扇形
            Circle = 1, //圆形
            Cube = 2, //长方形
            None = 3, //
            Ellipse = 4, //椭圆

            __Count__ = 5
        };

        enum class eRefreshThing { //出现的物品类型
            Monster = 0, //野怪
            OrganType = 1, //机关
            Resource = 2, //奖励物品1
            MonsterBoss = 3, //boss

            __Count__ = 4
        };

        enum class eResources { //资源类型
            Coin = 0, //现金
            Brass = 1, //陨铜币
            Material = 2, //普通材料
            Consume = 3, //消耗品
            Special = 4, //特殊
            FatigueValue = 5, //疲劳值
            Equipment = 6, //装备

            __Count__ = 7
        };

        enum class eSceneType { //场景类型
            Public = 0, //公共场景
            BattleCopy = 1, //战斗副本
            PublicBattle = 2, //野外战斗场景

            __Count__ = 3
        };

        enum class eSelectedType { //主动释放技能指向类型
            Aim = 0, //指向性（技能点击后必须选择了目标后才能释放）
            UnAim = 1, //非指向性（没有目标也可以释放玩家可以选择技能释放的位置）
            Liberty = 2, //没有选择贴花等选择区间只有范围区间。

            __Count__ = 3
        };

        enum class eSheetsType { //使用什么类型的面片
            Circle = 0, //圆形
            Rectangle = 1, //矩形
            sector = 2, //扇形
            None = 3, //无图形

            __Count__ = 4
        };

        enum class eSkillChargeType { //技能蓄力类型
            EndEarly = 0, //可以提前释放技能的蓄力（提前结束蓄力一样可以释放出来）
            NoEndEarly = 1, //不可以提前释放技能的蓄力（提前结束蓄力会导致技能无法释放出来）
            None = 4, //不可以蓄力

            __Count__ = 3
        };

        enum class eSkillEffect { //技能效果类型
            Launcher = 0, //发射器
            None = 1, //纯用来加buff的技能事件。
            Move = 2, //位移

            __Count__ = 3
        };

        enum class eSkillReleaseType { //技能释放类型
            Sing = 0, //吟唱性技能（使用后不可以自行决定时长使用后就会一直使用）
            Normal = 1, //正常释放的技能（不需要通过吟唱和蓄力来释放的技能）

            __Count__ = 2
        };

        enum class eSkillTriggleType { //技能激活类型
            Auto = 0, //被动触发
            Proactive = 1, //主动释放触发

            __Count__ = 2
        };

        enum class eSkillType { //技能类型
            Range = 0, //范围性技能
            Move = 1, //位移性技能
            Launch = 2, //弹道类技能
            Organ = 3, //机关型技能
            NonePerfom = 4, //无表现形式的技能

            __Count__ = 5
        };

        enum class eSoldType { //售卖类型
            Sold = 0, //可售卖
            None = 1, //不可售卖

            __Count__ = 2
        };

        enum class eSoundType { //音效类型
            EffectAll = 0, //全能听
            EffectSelf = 1, //个人能听到

            __Count__ = 2
        };

        enum class eState { //
            Idle = 0, //站立
            Moving = 1, //移动
            Jumping = 2, //
            Stuned = 3, //
            Shift = 4, //位移
            Death = 5, //死亡

            __Count__ = 6
        };

        enum class eSustainabilityType { //是否技能表
            None = 0, //非持续性技能在释放后自动结束该技能
            Again = 1, //需要再次点击同按钮后结束
            AddHp = 2, //在加血后结束

            __Count__ = 3
        };

        enum class eTargetType { //buff目标
            Monster = 0, //野怪
            All = 1, //全部可伤害敌对目标
            EnemyCharacter = 2, //敌方人物
            PlayerCharacter = 3, //除我以外的我方队友目标
            PlayerSelf = 4, //玩家自己
            Character = 5, //全部玩家
            Boss = 6, //Bosss

            __Count__ = 7
        };

        enum class eTaskEvent { //玩家任务进度事件类型
            PreRecive = 0, //玩家待接受任务
            Received = 1, //玩家已接受但未完成
            Complete = 2, //玩家已完成该任务
            Completed = 3, //玩家完成并交了
            None = 4, //没有任何状态纯对话

            __Count__ = 5
        };

        enum class eTaskReceiveType { //任务触发类型
            TriggleNpc = 0, //达到触发任务条件下触碰任务Npc后触发任务环节
            NoTriggle = 1, //达到触发任务条件下后直接触发任务环节

            __Count__ = 2
        };

        enum class eTaskType { //任务类型
            TaskLevel = 0, //关卡型任务
            TaskCollect = 1, //收集型任务
            TaskKill = 2, //击杀野怪数量累积型任务
            TaskUse = 3, //在使用了某件物品后
            TaskContact = 4, //在触碰了某个npc时间后完成
            TaskMaterail = 5, //收集资料型任务

            __Count__ = 6
        };

        enum class eTaskVariety { //任务品种
            MainMission = 0, //主线任务
            SideMission = 1, //支线任务
            SpecialMission = 2, //特殊任务
            DayMission = 3, //日常任务

            __Count__ = 4
        };

        enum class eTransPortType { //传送点类型
            Normal = 0, //正常传送点
            Evacuate = 1, //撤离点

            __Count__ = 2
        };

        enum class eTransactionType { //交易类型
            Tradable = 0, //可交易
            None = 1, //不可交易

            __Count__ = 2
        };

        enum class eTriggleCondition { //技能激活条件类型
            ReleaseSkill = 0, //释放任意技能后可以有条件触发
            HpDecrease = 1, //损失血量后可以有条件触发
            None = 2, //无条件
            Organ = 3, //在视野范围内看到机关
            Angry = 4, //有怒气值
            Monster = 5, //击杀野怪

            __Count__ = 6
        };

        enum class eTriggleType { //机关触发类型
            Atk = 0, //击打
            Near = 1, //靠近

            __Count__ = 2
        };

        enum class eTriggleWay { //触发方式
            Skill = 0, //释放某个技能后触发
            None = 1, //正常触发（被动技能就是自动触发的主动技能就是点击一个按钮触发）

            __Count__ = 2
        };

        enum class eValueChangeType { //数值修改方式
            Add = 0, //加
            Sub = 1, //减
            Multiply = 2, //乘
            Divide = 3, //除

            __Count__ = 4
        };

        enum class eVisualSceneType { //视野类型用哪种
            Public = 0, //公共地图视野
            Entity = 1, //角色个人视野

            __Count__ = 2
        };

        enum class eWeaponType { //武器类型
            Arrow = 0, //弓箭
            BluntWeapon = 1, //钝器
            Detonator = 2, //雷管
            Dagger = 3, //匕首
            Sword = 4, //剑
            None = 5, //非武器

            __Count__ = 6
        };


    }

    namespace Const {
        namespace AllowableError {
            inline static const float PositionErrorRange = { 2.5 };

        }

        namespace Aoi {
            inline static const Int64 AoiGridSize = { 1 };
            inline static const Int64 DefaultVisualSize = { 25 };
            inline static const Int64 Visual = { 250 };

        }

        namespace Attribute {
            inline static const char * Weight = {"重量"};
            inline static const char * Atk = {"物理攻击"};
            inline static const char * Speed = {"移速"};
            inline static const char * Hp = {"血量"};
            inline static const char * Critical = {"暴击率"};
            inline static const char * CriticalDamage = {"暴击伤害"};
            inline static const char * Defend = {"物理防御"};
            inline static const char * MagicDefend = {"法术防御"};
            inline static const char * Blood = {"吸血"};
            inline static const char * MagicAttack = {"法术攻击"};
            inline static const char * PhysicalPierce = {"物理穿透"};
            inline static const char * MagicPierce = {"法术穿透"};
            inline static const char * Dodge = {"闪避"};
            inline static const char * Resilience = {"韧性"};

        }

        namespace BattleSetting {
            inline static const Int64 RedScreenTime = { 300 };
            inline static const Int64 SkillTolerance = { 425 };

        }

        namespace BuffRevelant {
            inline static const char * BuffPrefab = {"UI/Prefabs/MainUI/UIBuffIcon"};

        }

        namespace Character {
            inline static const std::vector<Int64> InitializeCharacterConfigID = {100001,100005};
            inline static const Int64 Coin = { 1500 };
            inline static const Int64 Brass = { 1000000 };

        }

        namespace DamageCalculate {
            inline static const Int64 Physic = { 300 };
            inline static const Int64 Fire = { 200 };
            inline static const Int64 Water = { 200 };
            inline static const Int64 Gold = { 200 };

        }

        namespace DamageNumber {
            inline static const char * NormalDamage = {"UI/Sprites/Battle/NormalDamageNumber"};
            inline static const char * CriticalDamage = {"UI/Sprites/Battle/CriticalDamageNumber"};
            inline static const float Spacing = { 0.6 };
            inline static const float moveDistance = { 5 };
            inline static const float moveDuration = { 1 };
            inline static const float fadeDuration = { 0.28 };
            inline static const float delayBeforeRecycle = { 0 };
            inline static const float CriticalScale = { 2.35 };
            inline static const float NormalScale = { 2.15 };
            inline static const float SelfCriticalScale = { 1.55 };
            inline static const float SelfNormalScale = { 1.55 };
            inline static const float SelfmoveDistance = { 1.25 };
            inline static const float SelfmoveDuration = { 1 };
            inline static const float SelffadeDuration = { 0.85 };
            inline static const float SelfdelayBeforeRecycle = { 0.85 };
            inline static const char * SelfNormalDamage = {"UI/Sprites/Battle/NormalDamageNumber"};
            inline static const char * SelfCriticalDamage = {"UI/Sprites/Battle/CriticalDamageNumber"};
            inline static const float CritRiseDuration = { 0.05 };
            inline static const float CritRiseHeight = { 0.25 };
            inline static const float CritRiseScaleStart = { 0 };
            inline static const float CritRiseScaleEnd = { 0.5 };
            inline static const float CritBurstDuration = { 0.14 };
            inline static const float CritBurstScale = { 3.3 };
            inline static const float CritRecoilDuration = { 0.12 };
            inline static const float CritRecoilScale = { 1 };
            inline static const float CritPauseDuration = { 0.28 };
            inline static const float CritFadeDelay = { 0.18 };
            inline static const float NormalRiseDuration = { 0.05 };
            inline static const float NormalRiseHeight = { 0.5 };
            inline static const float NormalPauseDuration = { 0.1 };
            inline static const float NormalFadeDelay = { 0.35 };
            inline static const float SelfRiseDuration = { 0.05 };
            inline static const float SelfRiseHeight = { 0.15 };
            inline static const float SelfPauseDuration = { 0.08 };
            inline static const float SelfFadeDelay = { 0.33 };

        }

        namespace GameSetting {
            inline static const float Joystick = { 0 };
            inline static const float Skill = { 0.3 };
            inline static const Int64 Camera = { 12 };
            inline static const Int64 ChangeCharacter = { 10000 };
            inline static const Int64 DropTime = { 450 };
            inline static const Int64 DropDelayTime = { 500 };
            inline static const char * DropPrefab = {"RewardItem/LootItem/Prefabs/lootItem"};
            inline static const char * CoinDrop = {"UI/Sprites/Team/BlueCircle"};
            inline static const char * CoinIcon = {"UI/Sprites/Team/BlueCircle"};
            inline static const char * BrassDrop = {"UI/Sprites/Team/BlueCircle"};
            inline static const char * BrassIcon = {"UI/Sprites/Team/BlueCircle"};
            inline static const float ThingArea = { 2 };
            inline static const float DropRange = { 2.5 };
            inline static const Int64 MaxSafeContain = { 6 };
            inline static const Int64 InitialSafeContain = { 3 };
            inline static const Int64 InitialBattleBagContain = { 6 };
            inline static const Int64 MaxBattleBagContain = { 12 };
            inline static const Int64 InitialWarHouseContain = { 50 };
            inline static const Int64 MaxWarHouseContain = { 999 };

        }

        namespace HpGradient {
            inline static const float BossHpGradient = { 1 };
            inline static const float CharacterHpGradient = { 1 };
            inline static const float MonsterHpGradient = { 1 };

        }

        namespace InitialResource {
            inline static const Int64 InitialWeapon = { 4004 };
            inline static const Int64 InitialWeapon2 = { 4006 };

        }

        namespace InitialTask {
            inline static const Int64 Task = { 10001 };

        }

        namespace MatchRules {
            inline static const Int64 PeopleLimit = { 16 };
            inline static const Int64 MaxTime = { 12000 };
            inline static const Int64 Team = { 6 };
            inline static const Int64 TimeLen = { 1000 };
            inline static const Int64 Countdown = { 3000 };

        }

        namespace Monster {
            inline static const Int64 TraceInterval = { 1000 };

        }

        namespace MonsterBossIdle {
            inline static const Int64 HydraCypress = { 0 };

        }

        namespace Npc {
            inline static const Int64 NpcTriggle = { 10 };

        }

        namespace PackRules {
            inline static const Int64 MaxCount = { 99 };
            inline static const std::vector<float> PackSlot = {5,6};

        }

        namespace Partner {
            inline static const Int64 CountLimit = { 4 };
            inline static const Int64 PartTime = { 15000 };
            inline static const std::vector<float> TeamHp = {0,0.76,1};
            inline static const std::vector<float> EnemyHp = {1,0.25,0};
            inline static const std::vector<float> SelfHp = {0.26,1,0};
            inline static const char * TeamCircle = {"UI/Sprites/Team/BlueCircle"};
            inline static const char * EnemyCircle = {"UI/Sprites/Team/BlueCircle"};
            inline static const char * SelfCircle = {"UI/Sprites/Team/YellowCircle"};

        }

        namespace Pop {
            inline static const char * SlidePop = {"xxxx"};
            inline static const char * SmallDetail = {"xxxx"};
            inline static const char * BigDetail = {"xxxx"};
            inline static const char * CommonPop = {"xxxx"};
            inline static const char * Tips = {"xxxxx"};

        }

        namespace PositionSolide {
            inline static const Int64 Solide = { 33 };

        }

        namespace RhombusMap {
            inline static const std::vector<float> RhombusAngles = {26.57, 153.43, 206.57, 333.43};

        }

        namespace Skill {
            inline static const Int64 NotInSlot = { -1 };
            inline static const char * EnemySelected = {"xx"};

        }

        namespace Sounds {
            inline static const Int64 React = { 4011 };

        }

        namespace Tags {
            inline static const char * TeammateTag = {"Sprites/Tags/TeammateTag"};
            inline static const char * EnemyTag = {"Sprites/Tags/EnemyTag"};
            inline static const char * SelfTag = {"Sprites/Tags/SelfTag"};

        }

        namespace UI {
            inline static const char * TopUI = {"UI/Prefabs/TopUI/TopUI"};
            inline static const char * BattleTopUI = {"UI/Prefabs/TopUI/BattleTopUI"};
            inline static const char * Monster = {"UI/Prefabs/TopUI/MonsterTopUI"};

        }

        namespace World {
            inline static const Int64 DefaultWorld = { 20001 };

        }


    }


    namespace Struct {
        struct Buff {
            const Int64 ID; //BuffID
            const Enum::eAttribute Attribute; //改变属性类型
            const char * Name; //Buff名字
            const float EffectParameter; //Buff的数值（只有加速，加攻击力，破甲，加速等才会有实际参数），沉默、眩晕和无敌等状态没有实际参数
            const Int64 Delay; //延迟生效时间
            const Int64 LifeCycle; //buff生命周期
            const Int64 Interval; //buff生效次数的间隔时间（s/次）
            const Int64 MaxStackCount; //最高叠加层数
            const char * Material; //Buff生效时的图标
            const bool IfRfresh; //是否返回原来的值
            const Enum::eCalculationMethod CalculationMethod; //参数计算方式
            const Enum::eDamageType DamageType; //伤害属性
            const Int64 SpecialEffects; //Buff特效ID
            const bool ProgressBar; //有没有进度条ture有，false没有
            const Enum::eAddType AddType; //加成类型
            const Enum::eTargetType eBuffTargetConfig; //挂buff的目标
            const bool IsBuffAct; //buff是否会播放动作
            const char * BuffAct; //挂buff的时候人物的动作
            const bool actIsLoop; //act是否循环
            const bool IsHostDisappear; //是否随着宿主死亡而消失
            const bool CanAct; //是否能动
            const bool CanCamera; //是否拉动摄像头
            const Int64 CameraDisatance; //拉动后的摄像头位置
            const Int64 CameraTime; //拉动摄像头需要的时间
            const Int64 CameraRecover; //摄像头回正需要的时间


            Buff (const Int64 id, const Enum::eAttribute attribute, const char * name, const float effectparameter, const Int64 delay, const Int64 lifecycle, const Int64 interval, const Int64 maxstackcount, const char * material, const bool ifrfresh, const Enum::eCalculationMethod calculationmethod, const Enum::eDamageType damagetype, const Int64 specialeffects, const bool progressbar, const Enum::eAddType addtype, const Enum::eTargetType ebufftargetconfig, const bool isbuffact, const char * buffact, const bool actisloop, const bool ishostdisappear, const bool canact, const bool cancamera, const Int64 cameradisatance, const Int64 cameratime, const Int64 camerarecover) : ID(id), Attribute(attribute), Name(name), EffectParameter(effectparameter), Delay(delay), LifeCycle(lifecycle), Interval(interval), MaxStackCount(maxstackcount), Material(material), IfRfresh(ifrfresh), CalculationMethod(calculationmethod), DamageType(damagetype), SpecialEffects(specialeffects), ProgressBar(progressbar), AddType(addtype), eBuffTargetConfig(ebufftargetconfig), IsBuffAct(isbuffact), BuffAct(buffact), actIsLoop(actisloop), IsHostDisappear(ishostdisappear), CanAct(canact), CanCamera(cancamera), CameraDisatance(cameradisatance), CameraTime(cameratime), CameraRecover(camerarecover) {}
        };
        struct Bullet {
            const Int64 ID; //子弹ID
            const char * Name; //子弹名字
            const Enum::eBallistics Ballistics; //弹道类型
            const float Speed; //子弹速度单位 米/秒
            const Enum::eRangeType RangeType; //如果弹道类型是范围这里填范围类型
            const float Radius; //长半径单位米
            const float SectorAngle; //扇形角度范围
            const std::vector<float> RectangleSize; //矩形长宽
            const bool IsCollision; //是否碰撞检测
            const Int64 CollisionCount; //如果碰撞检测这里填检测次数
            const Int64 CollisionInterval; //子弹检测碰撞间隔
            const Int64 UpdateInterval; //角度和位置更新频率
            const float MaxTurnAngle; //如果是跟踪弹填最大转角角度
            const Enum::eDamageType DamageType; //伤害类型
            const std::vector<Int64> BuffConfigIDs; //命中产生的bug
            const std::vector<Int64> DeathLaunchers; //子弹消亡时产生发射器
            const std::vector<Enum::eTargetType> TargetType; //子弹生效目标
            const Int64 Angle; //角度（抛物线用到的是投射角度，直线的角度）
            const float AddSpeed; //子弹加速度（抛物线弹道时为重力加速度，直线则为加速度）
            const char * BulletPrefab; //预制体资源度SpcialEffect表
            const float BulletMutiple; //子弹最大倍率
            const Int64 EndPrefab; //子弹结束特效
            const Int64 LifeCycle; //子弹生命周期
            const Int64 DestroyTime; //爆炸特效销毁时间
            const float HeightParameter; //高度和距离差系数
            const std::vector<Enum::ePhysicType> PhysicType; //可阻挡子弹的物理碰撞物体
            const float SkillDamage; //伤害加成
            const Int64 TouchEffect; //子弹碰到人的特效
            const Int64 TrailerEffect; //子弹拖尾特效
            const bool EffectTarget; //是否只对目标产生效果
            const Int64 IntervalTime; //对一个人造成效果后间隔多久再次造成效果
            const bool IsMultiple; //子弹特效是否是多段的
            const char * WhereLoop; //在第几段的时候需要loop
            const char * PreviousLoop; //循环之前那一段的特效名称
            const char * LaterLoop; //循环后面那一段的特效名称
            const Int64 LoopTime; //循环特效的时长
            const float ShortRadius; //短半径单位米
            const bool IsHitLoop; //是否命中才会循环
            const Enum::eLifePanType LifePanType; //生命周期类型
            const Int64 AddHurtLife; //在造成别人伤害后可以增加的周期时间
            const bool IsDisappera; //在子弹碰撞完了后子弹预制体会不会消失
            const bool IsFollowHost; //子弹会不会随着宿主消亡而消亡
            const Int64 ChangeZ; //子弹生成出来的时候z轴变化多少
            const char * CanMoveLayer; //子弹可行走地图层


            Bullet (const Int64 id, const char * name, const Enum::eBallistics ballistics, const float speed, const Enum::eRangeType rangetype, const float radius, const float sectorangle, const std::vector<float> rectanglesize, const bool iscollision, const Int64 collisioncount, const Int64 collisioninterval, const Int64 updateinterval, const float maxturnangle, const Enum::eDamageType damagetype, const std::vector<Int64> buffconfigids, const std::vector<Int64> deathlaunchers, const std::vector<Enum::eTargetType> targettype, const Int64 angle, const float addspeed, const char * bulletprefab, const float bulletmutiple, const Int64 endprefab, const Int64 lifecycle, const Int64 destroytime, const float heightparameter, const std::vector<Enum::ePhysicType> physictype, const float skilldamage, const Int64 toucheffect, const Int64 trailereffect, const bool effecttarget, const Int64 intervaltime, const bool ismultiple, const char * whereloop, const char * previousloop, const char * laterloop, const Int64 looptime, const float shortradius, const bool ishitloop, const Enum::eLifePanType lifepantype, const Int64 addhurtlife, const bool isdisappera, const bool isfollowhost, const Int64 changez, const char * canmovelayer) : ID(id), Name(name), Ballistics(ballistics), Speed(speed), RangeType(rangetype), Radius(radius), SectorAngle(sectorangle), RectangleSize(rectanglesize), IsCollision(iscollision), CollisionCount(collisioncount), CollisionInterval(collisioninterval), UpdateInterval(updateinterval), MaxTurnAngle(maxturnangle), DamageType(damagetype), BuffConfigIDs(buffconfigids), DeathLaunchers(deathlaunchers), TargetType(targettype), Angle(angle), AddSpeed(addspeed), BulletPrefab(bulletprefab), BulletMutiple(bulletmutiple), EndPrefab(endprefab), LifeCycle(lifecycle), DestroyTime(destroytime), HeightParameter(heightparameter), PhysicType(physictype), SkillDamage(skilldamage), TouchEffect(toucheffect), TrailerEffect(trailereffect), EffectTarget(effecttarget), IntervalTime(intervaltime), IsMultiple(ismultiple), WhereLoop(whereloop), PreviousLoop(previousloop), LaterLoop(laterloop), LoopTime(looptime), ShortRadius(shortradius), IsHitLoop(ishitloop), LifePanType(lifepantype), AddHurtLife(addhurtlife), IsDisappera(isdisappera), IsFollowHost(isfollowhost), ChangeZ(changez), CanMoveLayer(canmovelayer) {}
        };
        struct Character {
            const Int64 ID; //CharacterID
            const char * Name; //描述
            const char * Prefab; //预制体
            const float InitialAtk; //初始物理攻击力
            const float InitialSpeed; //初始移速
            const float InitialHp; //初始生命值
            const float InitialCritical; //初始暴击率
            const float InitialCriticalDamage; //初始暴击伤害
            const float InitialDefend; //初始防御力
            const float InitialMagicDefend; //初始魔法抗性
            const float InitialBlood; //初始生命偷取值
            const float InitialMagicAttack; //初始智力强度
            const float InitialPhysicalPierce; //初始物理攻击穿透
            const float InitialMagicPierce; //初始法术穿透
            const float InitialAfford; //初始负重（单位kg）
            const float InitialDodge; //初始闪避率
            const float InitialResilience; //初始韧性
            const Int64 CharacterStrengthen; //人物升级强化id
            const float VisualField; //在战斗场景中的视角范围格子
            const Int64 CharacterLevel; //角色初始等级id
            const float InitialMp; //初始蓝量
            const float InitialHpReserve; //每次回复多少血
            const float InitialMpReserve; //每次回复多少蓝
            const Int64 InitialWeapon; //玩家初始掌握的剑ID
            const char * PrefabShow; //展示用的spine
            const std::vector<Int64> Skill; //包含的技能
            const std::vector<Int64> InitialSkill; //初始技能
            const char * Icon; //头像
            const Int64 Atk1; //第一段普攻
            const Int64 Atk2; //第二段普攻
            const Int64 Atk3; //第三段普攻
            const char * DirIcon; //在选择人物界面的目录头像
            const char * CharacterLogo; //人物属于什么类型的logo
            const Enum::eResources Resource; //解锁需要用的资源
            const Int64 Count; //需要的数量
            const float InitialAtkSpeed1; //第一段的攻击时间
            const float PhysicalPierceRate; //物理穿透率
            const float MagicPierceRate; //火元素穿透率
            const float AtkInteval; //普攻攻击间隔多久内可以接上下一段
            const float InitialFireDefend; //初始火元素抗性
            const float InitialWaterDefend; //初始水元素抗性
            const Int64 InitialAtkSpeed2; //第二段攻击时间
            const Int64 InitialAtkSpeed3; //第三段攻击时间
            const float CharacterBody; //角色身体半径大小
            const Enum::eDamagePriority DamagePriority; //伤害优先级
            const char * DeadAct; //死亡动作
            const Int64 DeadSound; //死亡声音
            const Int64 HpReserveSpeed; //多少秒回复一次血
            const Int64 MpReserveSpeed; //多少秒回复一次蓝
            const float CircleArea; //人物脚下的光圈大小乘积


            Character (const Int64 id, const char * name, const char * prefab, const float initialatk, const float initialspeed, const float initialhp, const float initialcritical, const float initialcriticaldamage, const float initialdefend, const float initialmagicdefend, const float initialblood, const float initialmagicattack, const float initialphysicalpierce, const float initialmagicpierce, const float initialafford, const float initialdodge, const float initialresilience, const Int64 characterstrengthen, const float visualfield, const Int64 characterlevel, const float initialmp, const float initialhpreserve, const float initialmpreserve, const Int64 initialweapon, const char * prefabshow, const std::vector<Int64> skill, const std::vector<Int64> initialskill, const char * icon, const Int64 atk1, const Int64 atk2, const Int64 atk3, const char * diricon, const char * characterlogo, const Enum::eResources resource, const Int64 count, const float initialatkspeed1, const float physicalpiercerate, const float magicpiercerate, const float atkinteval, const float initialfiredefend, const float initialwaterdefend, const Int64 initialatkspeed2, const Int64 initialatkspeed3, const float characterbody, const Enum::eDamagePriority damagepriority, const char * deadact, const Int64 deadsound, const Int64 hpreservespeed, const Int64 mpreservespeed, const float circlearea) : ID(id), Name(name), Prefab(prefab), InitialAtk(initialatk), InitialSpeed(initialspeed), InitialHp(initialhp), InitialCritical(initialcritical), InitialCriticalDamage(initialcriticaldamage), InitialDefend(initialdefend), InitialMagicDefend(initialmagicdefend), InitialBlood(initialblood), InitialMagicAttack(initialmagicattack), InitialPhysicalPierce(initialphysicalpierce), InitialMagicPierce(initialmagicpierce), InitialAfford(initialafford), InitialDodge(initialdodge), InitialResilience(initialresilience), CharacterStrengthen(characterstrengthen), VisualField(visualfield), CharacterLevel(characterlevel), InitialMp(initialmp), InitialHpReserve(initialhpreserve), InitialMpReserve(initialmpreserve), InitialWeapon(initialweapon), PrefabShow(prefabshow), Skill(skill), InitialSkill(initialskill), Icon(icon), Atk1(atk1), Atk2(atk2), Atk3(atk3), DirIcon(diricon), CharacterLogo(characterlogo), Resource(resource), Count(count), InitialAtkSpeed1(initialatkspeed1), PhysicalPierceRate(physicalpiercerate), MagicPierceRate(magicpiercerate), AtkInteval(atkinteval), InitialFireDefend(initialfiredefend), InitialWaterDefend(initialwaterdefend), InitialAtkSpeed2(initialatkspeed2), InitialAtkSpeed3(initialatkspeed3), CharacterBody(characterbody), DamagePriority(damagepriority), DeadAct(deadact), DeadSound(deadsound), HpReserveSpeed(hpreservespeed), MpReserveSpeed(mpreservespeed), CircleArea(circlearea) {}
        };
        struct CharacterLevel {
            const Int64 ID; //人物等级id
            const char * LevelDes; //等级描述
            const Int64 UpExp; //升级到下一级的经验要求
            const Int64 SkillPoint; //到达该等级后赠送的技能点数
            const Int64 SpecialEffect; //到达该等级后的特效
            const Int64 Level; //等级id


            CharacterLevel (const Int64 id, const char * leveldes, const Int64 upexp, const Int64 skillpoint, const Int64 specialeffect, const Int64 level) : ID(id), LevelDes(leveldes), UpExp(upexp), SkillPoint(skillpoint), SpecialEffect(specialeffect), Level(level) {}
        };
        struct CharacterStrengthen {
            const Int64 ID; //强化id
            const float AtkMutiple; //物理攻击强化等级增长的倍率基数；【物理攻击每次强化增强的公式=基数*玩家当前等级】,【装备物理攻击总和公式=基数*强化等级(强化等级² + 基数)/6】
            const float SpeedMutiple; //移速强化的等级增长倍率
            const float HpMutiple; //血量基数
            const float CriticalMutiple; //暴击率基数
            const float CriticalDamageMutiple; //暴击伤害基数
            const float DefendMutiple; //物理防御基数
            const float MagicDefendMutiple; //法术防御基数
            const float BloodMutiple; //吸血
            const float MagicAttackMutiple; //法术攻击基数
            const float PhysicalPierceMutiple; //物理穿透基数
            const float MagicPierceMutiple; //法术穿透基数
            const float DodgeMutiple; //闪避基数
            const float HitRate; //命中率基数


            CharacterStrengthen (const Int64 id, const float atkmutiple, const float speedmutiple, const float hpmutiple, const float criticalmutiple, const float criticaldamagemutiple, const float defendmutiple, const float magicdefendmutiple, const float bloodmutiple, const float magicattackmutiple, const float physicalpiercemutiple, const float magicpiercemutiple, const float dodgemutiple, const float hitrate) : ID(id), AtkMutiple(atkmutiple), SpeedMutiple(speedmutiple), HpMutiple(hpmutiple), CriticalMutiple(criticalmutiple), CriticalDamageMutiple(criticaldamagemutiple), DefendMutiple(defendmutiple), MagicDefendMutiple(magicdefendmutiple), BloodMutiple(bloodmutiple), MagicAttackMutiple(magicattackmutiple), PhysicalPierceMutiple(physicalpiercemutiple), MagicPierceMutiple(magicpiercemutiple), DodgeMutiple(dodgemutiple), HitRate(hitrate) {}
        };
        struct Consume {
            const Int64 ID; //消耗品id
            const char * Name; //消耗品名字名字
            const Enum::eTransactionType TransactionType; //交易类型
            const Enum::eSoldType SoldType; //售卖类型
            const char * Des; //材料描述
            const Enum::eResources SoldCurrency; //售卖货币类型
            const Int64 Price; //售卖价格
            const char * Icon; //材料图片
            const std::vector<Int64> CharacterUse; //哪个角色使用id
            const Int64 LevelLimit; //等级限制
            const char * ConsumeName; //消耗品名字
            const Int64 Weight; //重量
            const char * DropIcon; //物品掉落长什么样
            const Int64 Quality; //物品质量


            Consume (const Int64 id, const char * name, const Enum::eTransactionType transactiontype, const Enum::eSoldType soldtype, const char * des, const Enum::eResources soldcurrency, const Int64 price, const char * icon, const std::vector<Int64> characteruse, const Int64 levellimit, const char * consumename, const Int64 weight, const char * dropicon, const Int64 quality) : ID(id), Name(name), TransactionType(transactiontype), SoldType(soldtype), Des(des), SoldCurrency(soldcurrency), Price(price), Icon(icon), CharacterUse(characteruse), LevelLimit(levellimit), ConsumeName(consumename), Weight(weight), DropIcon(dropicon), Quality(quality) {}
        };
        struct DropResource {
            const Int64 ID; //资源id
            const Enum::eResources ResourceType; //资源类型
            const Int64 RelevantID; //资源类型下对应的id
            const Int64 Count; //资源数量
            const char * Des; //备注
            const float DropRate; //资源掉落概率


            DropResource (const Int64 id, const Enum::eResources resourcetype, const Int64 relevantid, const Int64 count, const char * des, const float droprate) : ID(id), ResourceType(resourcetype), RelevantID(relevantid), Count(count), Des(des), DropRate(droprate) {}
        };
        struct Dynamic {
            const Int64 ID; //动态属性id
            const std::vector<float> Atk; //物理攻击
            const std::vector<float> Speed; //移速
            const std::vector<float> Hp; //血量
            const std::vector<float> Critical; //暴击率
            const std::vector<float> CriticalDamage; //暴击伤害
            const std::vector<float> Defend; //物理防御
            const std::vector<float> MagicDefend; //法术防御
            const std::vector<float> Blood; //吸血
            const std::vector<float> MagicAttack; //法术攻击
            const std::vector<float> PhysicalPierce; //物理穿透
            const std::vector<float> MagicPierce; //法术穿透
            const std::vector<float> Dodge; //闪避


            Dynamic (const Int64 id, const std::vector<float> atk, const std::vector<float> speed, const std::vector<float> hp, const std::vector<float> critical, const std::vector<float> criticaldamage, const std::vector<float> defend, const std::vector<float> magicdefend, const std::vector<float> blood, const std::vector<float> magicattack, const std::vector<float> physicalpierce, const std::vector<float> magicpierce, const std::vector<float> dodge) : ID(id), Atk(atk), Speed(speed), Hp(hp), Critical(critical), CriticalDamage(criticaldamage), Defend(defend), MagicDefend(magicdefend), Blood(blood), MagicAttack(magicattack), PhysicalPierce(physicalpierce), MagicPierce(magicpierce), Dodge(dodge) {}
        };
        struct Equipment {
            const Int64 ID; //装备id
            const char * Name; //装备名字
            const Enum::eEquipmentType EquipmentType; //装备类型
            const Enum::eTransactionType TransactionType; //交易类型
            const Enum::eSoldType SoldType; //售卖类型
            const Enum::eResources SoldCurrency; //售卖货币类型
            const Int64 Price; //售卖价格
            const char * Des; //装备描述
            const Int64 LevelLimit; //装备的等级限制
            const Int64 Occupy; //占据格子
            const Int64 Patience; //装备耐久度
            const Int64 WeaponQuality; //装备品质id
            const float Weight; //重量（kg）
            const float Atk; //物理攻击
            const float Speed; //移速
            const float Hp; //血量
            const float Critical; //暴击率
            const float CriticalDamage; //暴击伤害
            const float Defend; //物理防御
            const float MagicDefend; //火元素防御
            const float Blood; //吸血
            const float MagicAttack; //法术攻击
            const float PhysicalPierce; //物理穿透
            const float MagicPierce; //法术穿透
            const float Dodge; //闪避
            const float Resilience; //韧性
            const Int64 Dynamic; //动态属性id
            const Int64 Skill; //装备技能
            const char * Icon; //装备icon
            const Int64 EquipmentStrengthen; //强化类型id
            const bool EquipmentAppearType; //装备是否显示类型
            const char * AppearPrefab; //装备显示在人物身上的图片资源
            const Enum::eWeaponType WeaponType; //武器类型
            const char * EquipmentTypeName; //装备类型名字
            const std::vector<Int64> CharacterUse; //可以使用的角色id
            const float InitialStrengthen; //初始强化等级
            const float PhysicalPierceRate; //物理穿透率
            const float MagicPierceRate; //火元素穿透率
            const char * DropIcon; //物品掉落长什么样
            const Int64 Quality; //物品质量


            Equipment (const Int64 id, const char * name, const Enum::eEquipmentType equipmenttype, const Enum::eTransactionType transactiontype, const Enum::eSoldType soldtype, const Enum::eResources soldcurrency, const Int64 price, const char * des, const Int64 levellimit, const Int64 occupy, const Int64 patience, const Int64 weaponquality, const float weight, const float atk, const float speed, const float hp, const float critical, const float criticaldamage, const float defend, const float magicdefend, const float blood, const float magicattack, const float physicalpierce, const float magicpierce, const float dodge, const float resilience, const Int64 dynamic, const Int64 skill, const char * icon, const Int64 equipmentstrengthen, const bool equipmentappeartype, const char * appearprefab, const Enum::eWeaponType weapontype, const char * equipmenttypename, const std::vector<Int64> characteruse, const float initialstrengthen, const float physicalpiercerate, const float magicpiercerate, const char * dropicon, const Int64 quality) : ID(id), Name(name), EquipmentType(equipmenttype), TransactionType(transactiontype), SoldType(soldtype), SoldCurrency(soldcurrency), Price(price), Des(des), LevelLimit(levellimit), Occupy(occupy), Patience(patience), WeaponQuality(weaponquality), Weight(weight), Atk(atk), Speed(speed), Hp(hp), Critical(critical), CriticalDamage(criticaldamage), Defend(defend), MagicDefend(magicdefend), Blood(blood), MagicAttack(magicattack), PhysicalPierce(physicalpierce), MagicPierce(magicpierce), Dodge(dodge), Resilience(resilience), Dynamic(dynamic), Skill(skill), Icon(icon), EquipmentStrengthen(equipmentstrengthen), EquipmentAppearType(equipmentappeartype), AppearPrefab(appearprefab), WeaponType(weapontype), EquipmentTypeName(equipmenttypename), CharacterUse(characteruse), InitialStrengthen(initialstrengthen), PhysicalPierceRate(physicalpiercerate), MagicPierceRate(magicpiercerate), DropIcon(dropicon), Quality(quality) {}
        };
        struct EquipmentStrengthen {
            const Int64 ID; //强化id
            const float AtkMutiple; //物理攻击强化等级增长的倍率基数；【物理攻击每次强化增强的公式=基数*玩家当前等级】,【装备物理攻击总和公式=基数*强化等级(强化等级² + 基数)/6】
            const float SpeedMutiple; //移速强化的等级增长倍率
            const float HpMutiple; //血量基数
            const float CriticalMutiple; //暴击率基数
            const float CriticalDamageMutiple; //暴击伤害基数
            const float DefendMutiple; //物理防御基数
            const float MagicDefendMutiple; //法术防御基数
            const float BloodMutiple; //吸血
            const float MagicAttackMutiple; //法术攻击基数
            const float PhysicalPierceMutiple; //物理穿透基数
            const float MagicPierceMutiple; //法术穿透基数
            const float DodgeMutiple; //闪避基数
            const float HitRate; //命中率基数


            EquipmentStrengthen (const Int64 id, const float atkmutiple, const float speedmutiple, const float hpmutiple, const float criticalmutiple, const float criticaldamagemutiple, const float defendmutiple, const float magicdefendmutiple, const float bloodmutiple, const float magicattackmutiple, const float physicalpiercemutiple, const float magicpiercemutiple, const float dodgemutiple, const float hitrate) : ID(id), AtkMutiple(atkmutiple), SpeedMutiple(speedmutiple), HpMutiple(hpmutiple), CriticalMutiple(criticalmutiple), CriticalDamageMutiple(criticaldamagemutiple), DefendMutiple(defendmutiple), MagicDefendMutiple(magicdefendmutiple), BloodMutiple(bloodmutiple), MagicAttackMutiple(magicattackmutiple), PhysicalPierceMutiple(physicalpiercemutiple), MagicPierceMutiple(magicpiercemutiple), DodgeMutiple(dodgemutiple), HitRate(hitrate) {}
        };
        struct Launcher {
            const Int64 ID; //LauncherID
            const char * Name; //发射器名字
            const Enum::ePosition LauncherPositionType; //发射器位置类型
            const std::vector<float> PositionOffset; //如果发射器位置类型是Offset这里配置,相对宿主的偏移位置(并且后期按照这个偏移跟随宿主)
            const bool RecycleWithDeath; //宿主死亡时是否回收
            const bool RotateWithHost; //是否跟随宿主的角度
            const Int64 FireDelay; //发射延时单位毫秒
            const Int64 FireCount; //发射次数
            const Int64 FireInterval; //发射频率单位毫秒
            const char * Resource; //发射器资源
            const std::vector<Int64> BulletConfigID; //子弹配置ID


            Launcher (const Int64 id, const char * name, const Enum::ePosition launcherpositiontype, const std::vector<float> positionoffset, const bool recyclewithdeath, const bool rotatewithhost, const Int64 firedelay, const Int64 firecount, const Int64 fireinterval, const char * resource, const std::vector<Int64> bulletconfigid) : ID(id), Name(name), LauncherPositionType(launcherpositiontype), PositionOffset(positionoffset), RecycleWithDeath(recyclewithdeath), RotateWithHost(rotatewithhost), FireDelay(firedelay), FireCount(firecount), FireInterval(fireinterval), Resource(resource), BulletConfigID(bulletconfigid) {}
        };
        struct Material {
            const Int64 ID; //材料id
            const char * Name; //材料名字
            const Enum::eTransactionType TransactionType; //交易类型
            const Enum::eSoldType SoldType; //售卖类型
            const char * Des; //材料描述
            const Enum::eResources SoldCurrency; //售卖货币类型
            const Int64 Price; //售卖价格
            const char * Icon; //材料图片
            const char * MaterialTypeName; //材料类型名字
            const Int64 Weight; //重量
            const char * DropIcon; //物品掉落长什么样
            const Int64 Quality; //物品质量


            Material (const Int64 id, const char * name, const Enum::eTransactionType transactiontype, const Enum::eSoldType soldtype, const char * des, const Enum::eResources soldcurrency, const Int64 price, const char * icon, const char * materialtypename, const Int64 weight, const char * dropicon, const Int64 quality) : ID(id), Name(name), TransactionType(transactiontype), SoldType(soldtype), Des(des), SoldCurrency(soldcurrency), Price(price), Icon(icon), MaterialTypeName(materialtypename), Weight(weight), DropIcon(dropicon), Quality(quality) {}
        };
        struct Monster {
            const Int64 Id; //怪物id
            const char * Name; //怪物名字
            const float Atk; //物理攻击
            const float Speed; //巡逻移速
            const float Hp; //血量
            const float Critical; //暴击率
            const float CriticalDamage; //暴击伤害
            const float Defend; //物理防御
            const float MagicDefend; //火元素防御
            const float Blood; //吸血
            const float MagicAttack; //法术攻击
            const float PhysicalPierce; //物理穿透
            const float MagicPierce; //法术穿透
            const float Dodge; //闪避
            const float Resilience; //韧性
            const float ChaseSpeed; //追踪速度
            const std::vector<Int64> DropResource; //击杀后掉落的物品
            const Int64 Exp; //击杀后掉落的经验
            const char * Prefab; //怪物预制体资源
            const float AwarenessRange; //警觉范围
            const float PatrolRange; //巡逻范围
            const float ExitRange; //超出怪物出生点多少范围返回
            const float AtkRange; //攻击的伤害范围
            const Int64 AttackInterval; //攻击间隔时间
            const float BackBorn; //脱离出生点多远开始返回出生点
            const float RegainHp; //每秒回血的系数
            const Int64 RegainHpSpeed; //回血速率S/次
            const Int64 StartRecover; //多少秒不受攻击就开始扣血
            const float PhysicalPierceRate; //物理穿透率
            const float MagicPierceRate; //火元素穿透率
            const Int64 MonsterQuality; //怪物品质id
            const float WaterMagicDefend; //水元素防御
            const float GoldMagicDefend; //金元素防御
            const char * DeadAct; //怪物死亡动作
            const Int64 SoundHurt; //怪物受击音效
            const Int64 SoundDie; //怪物死亡音效
            const float MonsterBody; //怪物身体大小半径
            const Enum::eDamagePriority DamagePriority; //优先级
            const bool IsHurtAppear; //是否放完技能后才会显现
            const Int64 DieTime; //在释放完技能多久后会自动死亡
            const Int64 DelayDieTime; //在对别人造成伤后会延长多久死亡时间
            const Int64 MonsterSkill; //怪物拥有的技能
            const Int64 HurtEffect; //受击特效
            const float VisualField; //怪物视野范围
            const float HurtDistance; //受击打后后退距离
            const float CircleArea; //怪脚底下的光圈面积大小乘积
            const Int64 Die; //怪物消失时间
            const Int64 MonsteAtkSkill; //怪物的普攻技能
            const Int64 DropTime; //怪物掉落物品动画时长
            const Int64 DropDelayTime; //怪物掉落物品延迟时间


            Monster (const Int64 id, const char * name, const float atk, const float speed, const float hp, const float critical, const float criticaldamage, const float defend, const float magicdefend, const float blood, const float magicattack, const float physicalpierce, const float magicpierce, const float dodge, const float resilience, const float chasespeed, const std::vector<Int64> dropresource, const Int64 exp, const char * prefab, const float awarenessrange, const float patrolrange, const float exitrange, const float atkrange, const Int64 attackinterval, const float backborn, const float regainhp, const Int64 regainhpspeed, const Int64 startrecover, const float physicalpiercerate, const float magicpiercerate, const Int64 monsterquality, const float watermagicdefend, const float goldmagicdefend, const char * deadact, const Int64 soundhurt, const Int64 sounddie, const float monsterbody, const Enum::eDamagePriority damagepriority, const bool ishurtappear, const Int64 dietime, const Int64 delaydietime, const Int64 monsterskill, const Int64 hurteffect, const float visualfield, const float hurtdistance, const float circlearea, const Int64 die, const Int64 monsteatkskill, const Int64 droptime, const Int64 dropdelaytime) : Id(id), Name(name), Atk(atk), Speed(speed), Hp(hp), Critical(critical), CriticalDamage(criticaldamage), Defend(defend), MagicDefend(magicdefend), Blood(blood), MagicAttack(magicattack), PhysicalPierce(physicalpierce), MagicPierce(magicpierce), Dodge(dodge), Resilience(resilience), ChaseSpeed(chasespeed), DropResource(dropresource), Exp(exp), Prefab(prefab), AwarenessRange(awarenessrange), PatrolRange(patrolrange), ExitRange(exitrange), AtkRange(atkrange), AttackInterval(attackinterval), BackBorn(backborn), RegainHp(regainhp), RegainHpSpeed(regainhpspeed), StartRecover(startrecover), PhysicalPierceRate(physicalpiercerate), MagicPierceRate(magicpiercerate), MonsterQuality(monsterquality), WaterMagicDefend(watermagicdefend), GoldMagicDefend(goldmagicdefend), DeadAct(deadact), SoundHurt(soundhurt), SoundDie(sounddie), MonsterBody(monsterbody), DamagePriority(damagepriority), IsHurtAppear(ishurtappear), DieTime(dietime), DelayDieTime(delaydietime), MonsterSkill(monsterskill), HurtEffect(hurteffect), VisualField(visualfield), HurtDistance(hurtdistance), CircleArea(circlearea), Die(die), MonsteAtkSkill(monsteatkskill), DropTime(droptime), DropDelayTime(dropdelaytime) {}
        };
        struct MonsterBoss {
            const Int64 Id; //怪物Bossid
            const char * Name; //Boss怪物名字
            const float Atk; //物理攻击
            const float Speed; //巡逻移速
            const float Hp; //血量
            const float Critical; //暴击率
            const float CriticalDamage; //暴击伤害
            const float Defend; //物理防御
            const float MagicDefend; //火元素防御
            const float Blood; //吸血
            const float MagicAttack; //法术攻击
            const float PhysicalPierce; //物理穿透
            const float MagicPierce; //法术穿透
            const float Dodge; //闪避
            const float Resilience; //韧性
            const float ChaseSpeed; //追踪速度
            const std::vector<Int64> DropResource; //击杀后掉落的物品
            const Int64 Exp; //击杀后掉落的经验
            const char * Prefab; //怪物预制体资源
            const float AwarenessRange; //警觉范围
            const float PatrolRange; //巡逻范围
            const float ExitRange; //超出怪物出生点多少范围返回
            const Int64 Atk1; //普攻技能1（读取MonsterSkill表）第一阶段
            const Int64 Atk2; //普攻技能2（读取monsterskll表）第一阶段
            const Int64 SecondAtk1; //普攻技能1（读取MonsterSkill表）第二阶段
            const Int64 SecondAtk2; //普攻技能2（读取monsterskll表）第二阶段
            const Int64 ThirdAtk1; //普攻技能1（读取MonsterSkill表）第三阶段
            const Int64 ThirdAtk2; //普攻技能2（读取monsterskll表）第三阶段
            const float BackBorn; //脱离出生点多远开始返回出生点
            const float HpRecover; //每次回复多少比例的生命值
            const Int64 HpRecoverRate; //多少毫秒回复一次
            const Int64 StartRecover; //多久不受到伤害开始回复
            const float ExchangeMode2; //切换到第二阶段的血量
            const float ExchangeMode3; //切换到第三阶段的血量百分比
            const std::vector<Int64> MonsterSkill1; //怪物第一阶段拥有的技能
            const std::vector<Int64> MonsterSkill2; //第二阶段拥有的技能
            const std::vector<Int64> MonsterSkill3; //怪物第三阶段拥有的技能
            const float PhysicalPierceRate; //物理穿透率
            const float MagicPierceRate; //火元素穿透率
            const Int64 MonsterQuality; //怪物品质id
            const float WaterMagicDefend; //水元素防御
            const float GoldMagicDefend; //金元素防御
            const char * DeadAct; //怪物死亡动作
            const Int64 SoundHurt; //怪物受击音效
            const Int64 SoundDie; //怪物死亡音效
            const float MonsterBody; //怪物身体大小半径
            const Enum::eDamagePriority DamagePriority; //优先级
            const std::vector<Int64> MonsterDie; //Boss死亡后副本哪些野怪会一同死亡
            const Enum::eBoss BossType; //boss类型
            const char * MonsterIdle; //Boss的待机动作名
            const char * BossHead; //Boss头像
            const Int64 HurtEffect; //受击特效
            const float VisualField; //怪物视野范围
            const float CircleArea; //怪脚底下的光圈面积大小乘积
            const std::vector<const char *> ConcludePartDie; //包括了哪些会死亡的节点
            const std::vector<const char *> PartDieAct; //各节点死亡动作
            const Int64 Die; //怪物消失时间
            const Int64 DropTime; //怪物掉落物品动画时长
            const Int64 DropDelayTime; //怪物掉落物品延迟时间


            MonsterBoss (const Int64 id, const char * name, const float atk, const float speed, const float hp, const float critical, const float criticaldamage, const float defend, const float magicdefend, const float blood, const float magicattack, const float physicalpierce, const float magicpierce, const float dodge, const float resilience, const float chasespeed, const std::vector<Int64> dropresource, const Int64 exp, const char * prefab, const float awarenessrange, const float patrolrange, const float exitrange, const Int64 atk1, const Int64 atk2, const Int64 secondatk1, const Int64 secondatk2, const Int64 thirdatk1, const Int64 thirdatk2, const float backborn, const float hprecover, const Int64 hprecoverrate, const Int64 startrecover, const float exchangemode2, const float exchangemode3, const std::vector<Int64> monsterskill1, const std::vector<Int64> monsterskill2, const std::vector<Int64> monsterskill3, const float physicalpiercerate, const float magicpiercerate, const Int64 monsterquality, const float watermagicdefend, const float goldmagicdefend, const char * deadact, const Int64 soundhurt, const Int64 sounddie, const float monsterbody, const Enum::eDamagePriority damagepriority, const std::vector<Int64> monsterdie, const Enum::eBoss bosstype, const char * monsteridle, const char * bosshead, const Int64 hurteffect, const float visualfield, const float circlearea, const std::vector<const char *> concludepartdie, const std::vector<const char *> partdieact, const Int64 die, const Int64 droptime, const Int64 dropdelaytime) : Id(id), Name(name), Atk(atk), Speed(speed), Hp(hp), Critical(critical), CriticalDamage(criticaldamage), Defend(defend), MagicDefend(magicdefend), Blood(blood), MagicAttack(magicattack), PhysicalPierce(physicalpierce), MagicPierce(magicpierce), Dodge(dodge), Resilience(resilience), ChaseSpeed(chasespeed), DropResource(dropresource), Exp(exp), Prefab(prefab), AwarenessRange(awarenessrange), PatrolRange(patrolrange), ExitRange(exitrange), Atk1(atk1), Atk2(atk2), SecondAtk1(secondatk1), SecondAtk2(secondatk2), ThirdAtk1(thirdatk1), ThirdAtk2(thirdatk2), BackBorn(backborn), HpRecover(hprecover), HpRecoverRate(hprecoverrate), StartRecover(startrecover), ExchangeMode2(exchangemode2), ExchangeMode3(exchangemode3), MonsterSkill1(monsterskill1), MonsterSkill2(monsterskill2), MonsterSkill3(monsterskill3), PhysicalPierceRate(physicalpiercerate), MagicPierceRate(magicpiercerate), MonsterQuality(monsterquality), WaterMagicDefend(watermagicdefend), GoldMagicDefend(goldmagicdefend), DeadAct(deadact), SoundHurt(soundhurt), SoundDie(sounddie), MonsterBody(monsterbody), DamagePriority(damagepriority), MonsterDie(monsterdie), BossType(bosstype), MonsterIdle(monsteridle), BossHead(bosshead), HurtEffect(hurteffect), VisualField(visualfield), CircleArea(circlearea), ConcludePartDie(concludepartdie), PartDieAct(partdieact), Die(die), DropTime(droptime), DropDelayTime(dropdelaytime) {}
        };
        struct MonsterQuality {
            const Int64 Id; //怪物品质id
            const char * Monster; //品质名
            const std::vector<float> Color; //色号
            const Enum::eDamagePriority DamagePriority; //优先级


            MonsterQuality (const Int64 id, const char * monster, const std::vector<float> color, const Enum::eDamagePriority damagepriority) : Id(id), Monster(monster), Color(color), DamagePriority(damagepriority) {}
        };
        struct MonsterSkill {
            const Int64 ID; //SkillID
            const char * Name; //技能说明
            const char * Desc; //技能描述
            const std::vector<Enum::eSkillEffect> SkillEffect; //技能效果枚举
            const std::vector<Int64> SkillEffectConfigID; //技能效果ID
            const Int64 StartRelease; //野怪激活后首次施法的时间
            const Int64 CoolDown; //技能冷却时间
            const bool IsTrack; //技能释放是否跟随玩家释放
            const bool SelfIsTrack; //怪物本体是否跟随玩家释放
            const Int64 MaxSkillDistacne; //技能的最大施法范围
            const bool IsSheetsPrefab; //是否有预警面片（true是有，false是无）
            const char * MaxSheetsPrefab; //技能预警面片资源
            const Int64 PreviousTime; //预警面片在技能出现的前多少秒出现
            const char * ActPrefab; //怪物技能动作
            const Int64 Sound; //音效
            const Int64 SpecialEffects; //技能特效id
            const Int64 SpecialEffectsDelay; //技特效延时
            const bool IsEffectLoop; //特效需不需要循环
            const Int64 BuffID; //buff
            const bool IsAgain; //是否会对命中该技能期间的敌人再次释放
            const Enum::eSheetsType SheetsType; //技能预警面片类型
            const std::vector<float> RangeParameter; //边框用
            const char * EnoughMaxSheetsPrefab; //技能填充预警满了后的材质图片
            const bool IsNromalAtk; //是否是普攻
            const bool IsMultiple; //动作是否是多段的
            const char * WhereLoop; //在第几段的时候需要loop
            const char * PreviousLoop; //循环之前那一段的特效名称
            const char * LaterLoop; //循环后面那一段的特效名称
            const Int64 LoopTime; //循环特效的时长
            const Int64 ActTrack; //动作分轨
            const char * IdleAct; //播完该技能动作后的进入的idle动作
            const Int64 IdleActTrack; //播完后回到对应的idle动作分轨
            const Int64 ActTime; //动作时间
            const bool IsSonPart; //技能是不是子节点的技能
            const char * SonPartName; //子节点路径名字
            const bool IsVibe; //技能是否震动
            const Int64 VibeStartTime; //技能释放后多久会开始震动
            const float VibeStrength; //震动强度
            const Int64 VibeDuration; //震动持续时间
            const Int64 Randomness; //震动随机性
            const Int64 Vibrato; //震动频率
            const bool IsFadeOut; //是否淡出
            const std::vector<float> FullRangeParameter; //填充内容用
            const bool CanMove; //技能释放的时候能否移动
            const Int64 SpecialEffectTime; //持续多久销毁


            MonsterSkill (const Int64 id, const char * name, const char * desc, const std::vector<Enum::eSkillEffect> skilleffect, const std::vector<Int64> skilleffectconfigid, const Int64 startrelease, const Int64 cooldown, const bool istrack, const bool selfistrack, const Int64 maxskilldistacne, const bool issheetsprefab, const char * maxsheetsprefab, const Int64 previoustime, const char * actprefab, const Int64 sound, const Int64 specialeffects, const Int64 specialeffectsdelay, const bool iseffectloop, const Int64 buffid, const bool isagain, const Enum::eSheetsType sheetstype, const std::vector<float> rangeparameter, const char * enoughmaxsheetsprefab, const bool isnromalatk, const bool ismultiple, const char * whereloop, const char * previousloop, const char * laterloop, const Int64 looptime, const Int64 acttrack, const char * idleact, const Int64 idleacttrack, const Int64 acttime, const bool issonpart, const char * sonpartname, const bool isvibe, const Int64 vibestarttime, const float vibestrength, const Int64 vibeduration, const Int64 randomness, const Int64 vibrato, const bool isfadeout, const std::vector<float> fullrangeparameter, const bool canmove, const Int64 specialeffecttime) : ID(id), Name(name), Desc(desc), SkillEffect(skilleffect), SkillEffectConfigID(skilleffectconfigid), StartRelease(startrelease), CoolDown(cooldown), IsTrack(istrack), SelfIsTrack(selfistrack), MaxSkillDistacne(maxskilldistacne), IsSheetsPrefab(issheetsprefab), MaxSheetsPrefab(maxsheetsprefab), PreviousTime(previoustime), ActPrefab(actprefab), Sound(sound), SpecialEffects(specialeffects), SpecialEffectsDelay(specialeffectsdelay), IsEffectLoop(iseffectloop), BuffID(buffid), IsAgain(isagain), SheetsType(sheetstype), RangeParameter(rangeparameter), EnoughMaxSheetsPrefab(enoughmaxsheetsprefab), IsNromalAtk(isnromalatk), IsMultiple(ismultiple), WhereLoop(whereloop), PreviousLoop(previousloop), LaterLoop(laterloop), LoopTime(looptime), ActTrack(acttrack), IdleAct(idleact), IdleActTrack(idleacttrack), ActTime(acttime), IsSonPart(issonpart), SonPartName(sonpartname), IsVibe(isvibe), VibeStartTime(vibestarttime), VibeStrength(vibestrength), VibeDuration(vibeduration), Randomness(randomness), Vibrato(vibrato), IsFadeOut(isfadeout), FullRangeParameter(fullrangeparameter), CanMove(canmove), SpecialEffectTime(specialeffecttime) {}
        };
        struct Monsterkill {
            const Int64 Id; //怪物id
            const Int64 Monster; //怪物id
            const Int64 Count; //数量


            Monsterkill (const Int64 id, const Int64 monster, const Int64 count) : Id(id), Monster(monster), Count(count) {}
        };
        struct Move {
            const Int64 ID; //位移形技能的id
            const char * Des; //备注
            const Enum::eMoveType MoveType; //位移类型
            const Int64 Speed; //    位移速度   （M/S）
            const float AddSpeed; //加速度
            const Int64 Time; //技能持续时间
            const bool ProcessDamageType; //位移过程中是否造成技能效果
            const float SkillDamage; //技能伤害
            const Int64 DamageRange; //伤害范围半径
            const std::vector<Int64> Buff; //技能带的buff
            const std::vector<Enum::ePhysicType> Obstacle; //技能会被什么阻挡
            const std::vector<float> ReleasePosition; //技能位置相对偏移(x,y)
            const float DelayTime; //技能释放的延迟时间
            const Enum::eDamageType DamgaeType; //伤害类型
            const Int64 DamageCount; //可以造成几次伤害(0的时候为无限次)
            const Int64 DamageInteval; //在对一个人造成伤害后间隔多久再次对该人造成伤害
            const Int64 DelayEffectTime; //释放技能后延迟多久后开始计算碰撞
            const Enum::eTargetType TargetType; //技能生效目标
            const bool DamageTarget; //技能是否只对目标物体造成效果
            const Int64 UpdateInterval; //角度更新频率
            const float MaxTurnAngle; //如果是跟踪弹填最大转角角度
            const Int64 TrailerEffect; //位移拖尾特效
            const Int64 MoveEffect; //位移特效
            const float HeightParameter; //高度和距离差系数


            Move (const Int64 id, const char * des, const Enum::eMoveType movetype, const Int64 speed, const float addspeed, const Int64 time, const bool processdamagetype, const float skilldamage, const Int64 damagerange, const std::vector<Int64> buff, const std::vector<Enum::ePhysicType> obstacle, const std::vector<float> releaseposition, const float delaytime, const Enum::eDamageType damgaetype, const Int64 damagecount, const Int64 damageinteval, const Int64 delayeffecttime, const Enum::eTargetType targettype, const bool damagetarget, const Int64 updateinterval, const float maxturnangle, const Int64 trailereffect, const Int64 moveeffect, const float heightparameter) : ID(id), Des(des), MoveType(movetype), Speed(speed), AddSpeed(addspeed), Time(time), ProcessDamageType(processdamagetype), SkillDamage(skilldamage), DamageRange(damagerange), Buff(buff), Obstacle(obstacle), ReleasePosition(releaseposition), DelayTime(delaytime), DamgaeType(damgaetype), DamageCount(damagecount), DamageInteval(damageinteval), DelayEffectTime(delayeffecttime), TargetType(targettype), DamageTarget(damagetarget), UpdateInterval(updateinterval), MaxTurnAngle(maxturnangle), TrailerEffect(trailereffect), MoveEffect(moveeffect), HeightParameter(heightparameter) {}
        };
        struct NonePerfom {
            const Int64 ID; //没有表现力的技能
            const char * Des; //备注
            const std::vector<Int64> Buff; //技能带的buff


            NonePerfom (const Int64 id, const char * des, const std::vector<Int64> buff) : ID(id), Des(des), Buff(buff) {}
        };
        struct Npc {
            const Int64 ID; //NpcID
            const char * Name; //Npc名字
            const Enum::eNpcType NpcType; //Npc类型
            const std::vector<Int64> NpcTalk; //Npc对话ID
            const char * HeadIcon; //Npc使用的头像资源
            const char * Prefab; //Npc在场景中用的资源
            const std::vector<float> NpcPosition; //任务Npc初始节点位置
            const std::vector<Int64> NpcPositionChange; //Npc改变位置id
            const Int64 TaskEvent; //在哪个任务事件要求ID后会消失
            const Int64 TransPort; //触碰到后传送的ID
            const char * BubbleTalk; //谈话的气泡弹窗对话内容


            Npc (const Int64 id, const char * name, const Enum::eNpcType npctype, const std::vector<Int64> npctalk, const char * headicon, const char * prefab, const std::vector<float> npcposition, const std::vector<Int64> npcpositionchange, const Int64 taskevent, const Int64 transport, const char * bubbletalk) : ID(id), Name(name), NpcType(npctype), NpcTalk(npctalk), HeadIcon(headicon), Prefab(prefab), NpcPosition(npcposition), NpcPositionChange(npcpositionchange), TaskEvent(taskevent), TransPort(transport), BubbleTalk(bubbletalk) {}
        };
        struct NpcPositionChange {
            const Int64 ID; //改变位置id
            const Int64 TaskEvent; //改变位置的任务事件id
            const std::vector<float> ChangePosition; //打开哪个位置的Npc节点名


            NpcPositionChange (const Int64 id, const Int64 taskevent, const std::vector<float> changeposition) : ID(id), TaskEvent(taskevent), ChangePosition(changeposition) {}
        };
        struct NpcTalk {
            const Int64 ID; //Npc的谈话id
            const std::vector<Int64> TalkContent; //对话组id
            const Int64 TaskEvent; //在什么样任务事件下触发该对话组


            NpcTalk (const Int64 id, const std::vector<Int64> talkcontent, const Int64 taskevent) : ID(id), TalkContent(talkcontent), TaskEvent(taskevent) {}
        };
        struct OrganType {
            const Int64 ID; //机关id
            const char * Des; //备注
            const std::vector<Int64> Buff; //技能带的buff
            const Enum::eSkillType SkillType; //机关技能类型
            const Int64 RelevantID; //对于机关技能相对应的id
            const std::vector<float> Position; //机关的位置
            const char * Material; //机关使用的技能使用的面片资源
            const Enum::eTriggleType TriggleType; //机关触发类型
            const bool Destroy; //机关是否可销毁
            const Int64 Hp; //机关血量


            OrganType (const Int64 id, const char * des, const std::vector<Int64> buff, const Enum::eSkillType skilltype, const Int64 relevantid, const std::vector<float> position, const char * material, const Enum::eTriggleType triggletype, const bool destroy, const Int64 hp) : ID(id), Des(des), Buff(buff), SkillType(skilltype), RelevantID(relevantid), Position(position), Material(material), TriggleType(triggletype), Destroy(destroy), Hp(hp) {}
        };
        struct Prerequisite {
            const Int64 ID; //前提要求id
            const Int64 CharacterLevel; //人物等级限制id
            const Int64 TaskEvent; //任务要求id
            const std::vector<Int64> Resource; //要求玩家持有什么资源id


            Prerequisite (const Int64 id, const Int64 characterlevel, const Int64 taskevent, const std::vector<Int64> resource) : ID(id), CharacterLevel(characterlevel), TaskEvent(taskevent), Resource(resource) {}
        };
        struct Quality {
            const Int64 ID; //物品品质id
            const char * QualityName; //物品品质名
            const std::vector<float> QualityColor; //该品质物品名字颜色色号
            const char * QualityPicture; //该品质物品的底图


            Quality (const Int64 id, const char * qualityname, const std::vector<float> qualitycolor, const char * qualitypicture) : ID(id), QualityName(qualityname), QualityColor(qualitycolor), QualityPicture(qualitypicture) {}
        };
        struct RefreshRule {
            const Int64 ID; //刷怪规则id
            const Enum::eRefreshThing RefreshThing; //刷新物品类型
            const Int64 RelevantId; //相关物品id
            const Int64 Count; //数量
            const Int64 MaxCount; //最多刷多少只
            const Int64 Interval; //刷新频率(毫秒)
            const std::vector<float> MonsterPosition; //出现在哪个位置附近
            const Int64 PositionRange; //位置的半径范围


            RefreshRule (const Int64 id, const Enum::eRefreshThing refreshthing, const Int64 relevantid, const Int64 count, const Int64 maxcount, const Int64 interval, const std::vector<float> monsterposition, const Int64 positionrange) : ID(id), RefreshThing(refreshthing), RelevantId(relevantid), Count(count), MaxCount(maxcount), Interval(interval), MonsterPosition(monsterposition), PositionRange(positionrange) {}
        };
        struct Resource {
            const Int64 ID; //资源id
            const Enum::eResources ResourceType; //资源类型
            const Int64 RelevantID; //资源类型下对应的id
            const Int64 Count; //资源数量
            const char * Des; //备注
            const char * Icon; //资源Icon


            Resource (const Int64 id, const Enum::eResources resourcetype, const Int64 relevantid, const Int64 count, const char * des, const char * icon) : ID(id), ResourceType(resourcetype), RelevantID(relevantid), Count(count), Des(des), Icon(icon) {}
        };
        struct ResourceQuality {
            const Int64 ID; //物品的品质id
            const char * Name; //品质名
            const char * Icon; //品质底图
            const char * Icon2; //品质详情底图


            ResourceQuality (const Int64 id, const char * name, const char * icon, const char * icon2) : ID(id), Name(name), Icon(icon), Icon2(icon2) {}
        };
        struct Scene {
            const Int64 ID; //世界id
            const Enum::eSceneType SceneType; //副本类型
            const char * Name; //场景名字
            const std::vector<float> StartPoint; //出生点
            const char * Preload; //资源文件名
            const std::vector<Int64> TaskNpc; //场景内Npc
            const Int64 AdimitPeople; //场景允许小队最大一次性进入人数
            const Int64 AdmitGroup; //一张地图最大允许多少组人同时存在
            const Int64 Prerequisite; //进入要求id
            const std::vector<Int64> Resource; //进入消耗id
            const std::vector<Int64> RefreshRule; //包含的刷新规则id
            const Int64 Sound; //背景音乐
            const std::vector<Int64> TransPort; //该场景包含的传送点（读Transport配置表)
            const Enum::eVisualSceneType VisualSceneType; //视野类型用哪种
            const Int64 DisappearTime; //副本消失时间
            const bool IsPvP; //支不支持pvp


            Scene (const Int64 id, const Enum::eSceneType scenetype, const char * name, const std::vector<float> startpoint, const char * preload, const std::vector<Int64> tasknpc, const Int64 adimitpeople, const Int64 admitgroup, const Int64 prerequisite, const std::vector<Int64> resource, const std::vector<Int64> refreshrule, const Int64 sound, const std::vector<Int64> transport, const Enum::eVisualSceneType visualscenetype, const Int64 disappeartime, const bool ispvp) : ID(id), SceneType(scenetype), Name(name), StartPoint(startpoint), Preload(preload), TaskNpc(tasknpc), AdimitPeople(adimitpeople), AdmitGroup(admitgroup), Prerequisite(prerequisite), Resource(resource), RefreshRule(refreshrule), Sound(sound), TransPort(transport), VisualSceneType(visualscenetype), DisappearTime(disappeartime), IsPvP(ispvp) {}
        };
        struct Skill {
            const Int64 ID; //SkillID
            const char * Name; //技能明
            const char * Desc; //技能描述
            const std::vector<Enum::eSkillEffect> SkillEffect; //技能效果枚举
            const std::vector<Int64> SkillEffectConfigID; //技能效果ID
            const char * SkillIcon; //技能icon
            const bool CanEquip; //是否可装备
            const char * UINode; //对应技能设置界面的节点
            const Int64 CoolDown; //技能冷却事件
            const Int64 SpecialEffects; //技能特效id
            const Enum::eSelectedType SelectedType; //指向类型
            const std::vector<Enum::eTargetType> TargetType; //可指向的目标（指向性技能的时候可以选择的目标）
            const bool StudyOrNot; //技能是否需要学习才能拥有
            const Int64 CharacterLevel; //玩家学习技能的等级要求id
            const Int64 UpInterval; //玩家升级技能的等级间隔
            const std::vector<Int64> SkillCommand; //学习技能的前提技能要求（读Skill表）
            const Enum::eConsumeType Consume; //消耗类型
            const Int64 ConsumeCount; //消耗数量
            const Int64 MaxSkillDistacne; //技能最大可选择的释放位置范围半径（后端用）
            const char * MaxSheetsPrefab; //最大范围目标使用的面片材质路径
            const float MaxRangeRadius; //最大范围面片的面积半径乘值（前端）
            const char * UseSheetsPrefab; //玩家选择技能施法的导航条面片资源
            const Enum::eSheetsType SheetsType; //导航条用什么类型的面片资源
            const std::vector<float> RangeParameter; //导航条使用的面片面积大小（圆形的话就是半径，长方形就是长*宽，扇形的话就是半径和角度）
            const bool CanSeparated; //导航面片能否脱离玩家（true是能脱离，false是不能脱离，能脱离就是面片可以不粘着玩家自由选择位置，不能脱离玩家就是始终会以玩家为圆心面片不能脱离玩家“前端用”）
            const char * ActPrefab; //技能动作
            const Int64 ActTime; //动作时间
            const Int64 ShowRangeTime; //按多久出现范围
            const bool ActIsLoop; //动作是否循环
            const bool IsNromalAtk; //是否是普攻
            const bool IsPost; //技能动作有没有后摇
            const char * PostName; //后摇动作
            const bool CanMove; //放技能期间能否进入走路状态机
            const Int64 CantMoveTime; //多久不能进入移动状态机
            const Int64 Sound; //用什么声音id
            const Int64 SpecialEffectsDelay; //技特效延时
            const Int64 EffectCycleLife; //特效时间
            const bool IsEffectLoop; //特效需不需要循环
            const Int64 SkillInterval; //多久时间后才能衔接下一个技能
            const float SheetsMove; //技能面片摇杆速率
            const Int64 ConsumeSkill; //升级消耗的技能点数
            const bool IsAtkSheets; //普攻是否会出面片


            Skill (const Int64 id, const char * name, const char * desc, const std::vector<Enum::eSkillEffect> skilleffect, const std::vector<Int64> skilleffectconfigid, const char * skillicon, const bool canequip, const char * uinode, const Int64 cooldown, const Int64 specialeffects, const Enum::eSelectedType selectedtype, const std::vector<Enum::eTargetType> targettype, const bool studyornot, const Int64 characterlevel, const Int64 upinterval, const std::vector<Int64> skillcommand, const Enum::eConsumeType consume, const Int64 consumecount, const Int64 maxskilldistacne, const char * maxsheetsprefab, const float maxrangeradius, const char * usesheetsprefab, const Enum::eSheetsType sheetstype, const std::vector<float> rangeparameter, const bool canseparated, const char * actprefab, const Int64 acttime, const Int64 showrangetime, const bool actisloop, const bool isnromalatk, const bool ispost, const char * postname, const bool canmove, const Int64 cantmovetime, const Int64 sound, const Int64 specialeffectsdelay, const Int64 effectcyclelife, const bool iseffectloop, const Int64 skillinterval, const float sheetsmove, const Int64 consumeskill, const bool isatksheets) : ID(id), Name(name), Desc(desc), SkillEffect(skilleffect), SkillEffectConfigID(skilleffectconfigid), SkillIcon(skillicon), CanEquip(canequip), UINode(uinode), CoolDown(cooldown), SpecialEffects(specialeffects), SelectedType(selectedtype), TargetType(targettype), StudyOrNot(studyornot), CharacterLevel(characterlevel), UpInterval(upinterval), SkillCommand(skillcommand), Consume(consume), ConsumeCount(consumecount), MaxSkillDistacne(maxskilldistacne), MaxSheetsPrefab(maxsheetsprefab), MaxRangeRadius(maxrangeradius), UseSheetsPrefab(usesheetsprefab), SheetsType(sheetstype), RangeParameter(rangeparameter), CanSeparated(canseparated), ActPrefab(actprefab), ActTime(acttime), ShowRangeTime(showrangetime), ActIsLoop(actisloop), IsNromalAtk(isnromalatk), IsPost(ispost), PostName(postname), CanMove(canmove), CantMoveTime(cantmovetime), Sound(sound), SpecialEffectsDelay(specialeffectsdelay), EffectCycleLife(effectcyclelife), IsEffectLoop(iseffectloop), SkillInterval(skillinterval), SheetsMove(sheetsmove), ConsumeSkill(consumeskill), IsAtkSheets(isatksheets) {}
        };
        struct Sound {
            const Int64 ID; //音效id
            const char * Des; //注释
            const Enum::eSoundType eSoundType; //音效类型
            const char * AudioMaterial; //音效文件资源位置
            const bool Loop; //是否循环


            Sound (const Int64 id, const char * des, const Enum::eSoundType esoundtype, const char * audiomaterial, const bool loop) : ID(id), Des(des), eSoundType(esoundtype), AudioMaterial(audiomaterial), Loop(loop) {}
        };
        struct Special {
            const Int64 ID; //材料id
            const char * Name; //材料名字
            const Enum::eTransactionType TransactionType; //交易类型
            const Enum::eSoldType SoldType; //售卖类型
            const char * Des; //材料描述
            const Enum::eResources SoldCurrency; //售卖货币类型
            const Int64 Price; //售卖价格
            const char * Icon; //材料图片
            const char * SpecialTypeName; //特殊物品类型名字
            const Int64 Weight; //重量
            const char * DropIcon; //物品掉落长什么样
            const Int64 Quality; //物品质量


            Special (const Int64 id, const char * name, const Enum::eTransactionType transactiontype, const Enum::eSoldType soldtype, const char * des, const Enum::eResources soldcurrency, const Int64 price, const char * icon, const char * specialtypename, const Int64 weight, const char * dropicon, const Int64 quality) : ID(id), Name(name), TransactionType(transactiontype), SoldType(soldtype), Des(des), SoldCurrency(soldcurrency), Price(price), Icon(icon), SpecialTypeName(specialtypename), Weight(weight), DropIcon(dropicon), Quality(quality) {}
        };
        struct SpecialEffects {
            const Int64 ID; //特效表
            const char * Des; //备注
            const char * Prefab; //使用的特效资源
            const Enum::ePosition EffectPositionType; //特效出现的位置
            const Int64 Time; //特效时长


            SpecialEffects (const Int64 id, const char * des, const char * prefab, const Enum::ePosition effectpositiontype, const Int64 time) : ID(id), Des(des), Prefab(prefab), EffectPositionType(effectpositiontype), Time(time) {}
        };
        struct TalkContent {
            const Int64 ID; //对话内容id
            const char * Content; //对话内容
            const char * Title; //对话标题
            const char * Picture; //对话人物图片资源路径


            TalkContent (const Int64 id, const char * content, const char * title, const char * picture) : ID(id), Content(content), Title(title), Picture(picture) {}
        };
        struct Task {
            const Int64 ID; //任务id
            const Enum::eTaskReceiveType TaskReceiveType; //触发任务接受类型
            const std::vector<Int64> Resource; //任务奖励
            const char * TaskBrief; //任务缩略描述
            const Int64 Prerequisite; //接受任务的前提条件ID
            const std::vector<Int64> TaskCompleteCondition; //任务完成条件id
            const Int64 AcceptNpc; //在哪个Npc那接任务（读Npc表）
            const Int64 CompleteNpc; //在哪个Npc上交任务（读Npc表）
            const char * TaskAnimation; //任务触发的动画资源
            const std::vector<const char *> TaskPicture; //任务触发的插画
            const Int64 TaskSystem; //属于哪个任务系列
            const char * TaskName; //任务名字
            const char * TaskDes; //任务描述


            Task (const Int64 id, const Enum::eTaskReceiveType taskreceivetype, const std::vector<Int64> resource, const char * taskbrief, const Int64 prerequisite, const std::vector<Int64> taskcompletecondition, const Int64 acceptnpc, const Int64 completenpc, const char * taskanimation, const std::vector<const char *> taskpicture, const Int64 tasksystem, const char * taskname, const char * taskdes) : ID(id), TaskReceiveType(taskreceivetype), Resource(resource), TaskBrief(taskbrief), Prerequisite(prerequisite), TaskCompleteCondition(taskcompletecondition), AcceptNpc(acceptnpc), CompleteNpc(completenpc), TaskAnimation(taskanimation), TaskPicture(taskpicture), TaskSystem(tasksystem), TaskName(taskname), TaskDes(taskdes) {}
        };
        struct TaskCompleteCondition {
            const Int64 ID; //任务完成条件id
            const Enum::eTaskType TaskType; //任务完成条件类型
            const Int64 TaskTypeID; //任务完成类型对应的id


            TaskCompleteCondition (const Int64 id, const Enum::eTaskType tasktype, const Int64 tasktypeid) : ID(id), TaskType(tasktype), TaskTypeID(tasktypeid) {}
        };
        struct TaskContact {
            const Int64 ID; //交谈类型
            const char * Des; //备注
            const Int64 TaskNpc; //接触哪个Npc的ID后完成


            TaskContact (const Int64 id, const char * des, const Int64 tasknpc) : ID(id), Des(des), TaskNpc(tasknpc) {}
        };
        struct TaskEvent {
            const Int64 ID; //任务事件id
            const Enum::eTaskEvent TaskProgress; //任务进度类型
            const Int64 Task; //对应的任务id


            TaskEvent (const Int64 id, const Enum::eTaskEvent taskprogress, const Int64 task) : ID(id), TaskProgress(taskprogress), Task(task) {}
        };
        struct TaskKill {
            const Int64 ID; //杀死野怪数量id
            const char * Des; //备注
            const std::vector<Int64> MonsterKill; //资源id


            TaskKill (const Int64 id, const char * des, const std::vector<Int64> monsterkill) : ID(id), Des(des), MonsterKill(monsterkill) {}
        };
        struct TaskLevel {
            const Int64 ID; //关卡任务id
            const char * Des; //备注
            const Int64 Scene; //通关场景id


            TaskLevel (const Int64 id, const char * des, const Int64 scene) : ID(id), Des(des), Scene(scene) {}
        };
        struct TaskMaterail {
            const Int64 ID; //收集材料形任务id
            const char * Des; //备注
            const std::vector<Int64> Resource; //资源id


            TaskMaterail (const Int64 id, const char * des, const std::vector<Int64> resource) : ID(id), Des(des), Resource(resource) {}
        };
        struct TaskSystem {
            const Int64 ID; //任务系列id
            const Enum::eTaskVariety TaskVariety; //任务系列的种类
            const char * Name; //任务系列名字
            const Int64 Prerequisite; //解锁前提条件id
            const Int64 CompletePrequisite; //系列完成条件
            const std::vector<Int64> TaskSon; //子任务


            TaskSystem (const Int64 id, const Enum::eTaskVariety taskvariety, const char * name, const Int64 prerequisite, const Int64 completeprequisite, const std::vector<Int64> taskson) : ID(id), TaskVariety(taskvariety), Name(name), Prerequisite(prerequisite), CompletePrequisite(completeprequisite), TaskSon(taskson) {}
        };
        struct TaskUse {
            const Int64 ID; //使用物品id
            const char * Des; //备注
            const Int64 Resource; //使用的资源ID


            TaskUse (const Int64 id, const char * des, const Int64 resource) : ID(id), Des(des), Resource(resource) {}
        };
        struct Tests {
            const Int64 ID; //ID
            const Int64 TestInt; //TestInt
            const float TestFloat; //TestFloat
            const char * TestString; //TestString
            const bool TestBool; //TestBool
            const Enum::eTransactionType TestEnum; //TestEnum
            const std::vector<Int64> TestIntArray; //TestIntArray
            const std::vector<float> TestFloatArray; //TestFloatArray
            const std::vector<const char *> TestStringArray; //TestStringArray
            const std::vector<bool> TestBoolArray; //TestBoolArray
            const std::vector<Enum::eTransactionType> TestEnumArray; //TestEnumArray


            Tests (const Int64 id, const Int64 testint, const float testfloat, const char * teststring, const bool testbool, const Enum::eTransactionType testenum, const std::vector<Int64> testintarray, const std::vector<float> testfloatarray, const std::vector<const char *> teststringarray, const std::vector<bool> testboolarray, const std::vector<Enum::eTransactionType> testenumarray) : ID(id), TestInt(testint), TestFloat(testfloat), TestString(teststring), TestBool(testbool), TestEnum(testenum), TestIntArray(testintarray), TestFloatArray(testfloatarray), TestStringArray(teststringarray), TestBoolArray(testboolarray), TestEnumArray(testenumarray) {}
        };
        struct TransPort {
            const Int64 ID; //传送的位置id
            const char * Des; //备注
            const Int64 Scene; //传送回哪个场景id
            const std::vector<float> Position; //传送点所处的位置
            const Enum::eTransPortType TransPortType; //传送点类型
            const Int64 WhenEffect; //在玩家进入该场景后传送点多久开始生效
            const Int64 EffectNumber; //一场游戏最多生效几次（0就是无限次）生效次数达到后把他销毁，
            const Int64 EffecTime; //在传送点范围内站多久后开始传送
            const Int64 TransPortRange; //传送点范围半径大小
            const char * Effect; //传送点特效
            const bool IsLoop; //传送点特效是否循环
            const char * WhereLoop; //传送点特效循环哪一段


            TransPort (const Int64 id, const char * des, const Int64 scene, const std::vector<float> position, const Enum::eTransPortType transporttype, const Int64 wheneffect, const Int64 effectnumber, const Int64 effectime, const Int64 transportrange, const char * effect, const bool isloop, const char * whereloop) : ID(id), Des(des), Scene(scene), Position(position), TransPortType(transporttype), WhenEffect(wheneffect), EffectNumber(effectnumber), EffecTime(effectime), TransPortRange(transportrange), Effect(effect), IsLoop(isloop), WhereLoop(whereloop) {}
        };

    }

    
    class IAutoConfig {
    public:
        virtual ~IAutoConfig() {}

        virtual const Struct::Buff* QueryBuffConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Buff*> & GetBuffConfigMap() const = 0;        virtual const Struct::Bullet* QueryBulletConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Bullet*> & GetBulletConfigMap() const = 0;        virtual const Struct::Character* QueryCharacterConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Character*> & GetCharacterConfigMap() const = 0;        virtual const Struct::CharacterLevel* QueryCharacterLevelConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::CharacterLevel*> & GetCharacterLevelConfigMap() const = 0;        virtual const Struct::CharacterStrengthen* QueryCharacterStrengthenConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::CharacterStrengthen*> & GetCharacterStrengthenConfigMap() const = 0;        virtual const Struct::Consume* QueryConsumeConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Consume*> & GetConsumeConfigMap() const = 0;        virtual const Struct::DropResource* QueryDropResourceConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::DropResource*> & GetDropResourceConfigMap() const = 0;        virtual const Struct::Dynamic* QueryDynamicConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Dynamic*> & GetDynamicConfigMap() const = 0;        virtual const Struct::Equipment* QueryEquipmentConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Equipment*> & GetEquipmentConfigMap() const = 0;        virtual const Struct::EquipmentStrengthen* QueryEquipmentStrengthenConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::EquipmentStrengthen*> & GetEquipmentStrengthenConfigMap() const = 0;        virtual const Struct::Launcher* QueryLauncherConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Launcher*> & GetLauncherConfigMap() const = 0;        virtual const Struct::Material* QueryMaterialConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Material*> & GetMaterialConfigMap() const = 0;        virtual const Struct::Monster* QueryMonsterConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Monster*> & GetMonsterConfigMap() const = 0;        virtual const Struct::MonsterBoss* QueryMonsterBossConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::MonsterBoss*> & GetMonsterBossConfigMap() const = 0;        virtual const Struct::MonsterQuality* QueryMonsterQualityConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::MonsterQuality*> & GetMonsterQualityConfigMap() const = 0;        virtual const Struct::MonsterSkill* QueryMonsterSkillConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::MonsterSkill*> & GetMonsterSkillConfigMap() const = 0;        virtual const Struct::Monsterkill* QueryMonsterkillConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Monsterkill*> & GetMonsterkillConfigMap() const = 0;        virtual const Struct::Move* QueryMoveConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Move*> & GetMoveConfigMap() const = 0;        virtual const Struct::NonePerfom* QueryNonePerfomConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::NonePerfom*> & GetNonePerfomConfigMap() const = 0;        virtual const Struct::Npc* QueryNpcConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Npc*> & GetNpcConfigMap() const = 0;        virtual const Struct::NpcPositionChange* QueryNpcPositionChangeConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::NpcPositionChange*> & GetNpcPositionChangeConfigMap() const = 0;        virtual const Struct::NpcTalk* QueryNpcTalkConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::NpcTalk*> & GetNpcTalkConfigMap() const = 0;        virtual const Struct::OrganType* QueryOrganTypeConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::OrganType*> & GetOrganTypeConfigMap() const = 0;        virtual const Struct::Prerequisite* QueryPrerequisiteConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Prerequisite*> & GetPrerequisiteConfigMap() const = 0;        virtual const Struct::Quality* QueryQualityConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Quality*> & GetQualityConfigMap() const = 0;        virtual const Struct::RefreshRule* QueryRefreshRuleConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::RefreshRule*> & GetRefreshRuleConfigMap() const = 0;        virtual const Struct::Resource* QueryResourceConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Resource*> & GetResourceConfigMap() const = 0;        virtual const Struct::ResourceQuality* QueryResourceQualityConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::ResourceQuality*> & GetResourceQualityConfigMap() const = 0;        virtual const Struct::Scene* QuerySceneConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Scene*> & GetSceneConfigMap() const = 0;        virtual const Struct::Skill* QuerySkillConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Skill*> & GetSkillConfigMap() const = 0;        virtual const Struct::Sound* QuerySoundConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Sound*> & GetSoundConfigMap() const = 0;        virtual const Struct::Special* QuerySpecialConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Special*> & GetSpecialConfigMap() const = 0;        virtual const Struct::SpecialEffects* QuerySpecialEffectsConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::SpecialEffects*> & GetSpecialEffectsConfigMap() const = 0;        virtual const Struct::TalkContent* QueryTalkContentConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::TalkContent*> & GetTalkContentConfigMap() const = 0;        virtual const Struct::Task* QueryTaskConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Task*> & GetTaskConfigMap() const = 0;        virtual const Struct::TaskCompleteCondition* QueryTaskCompleteConditionConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::TaskCompleteCondition*> & GetTaskCompleteConditionConfigMap() const = 0;        virtual const Struct::TaskContact* QueryTaskContactConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::TaskContact*> & GetTaskContactConfigMap() const = 0;        virtual const Struct::TaskEvent* QueryTaskEventConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::TaskEvent*> & GetTaskEventConfigMap() const = 0;        virtual const Struct::TaskKill* QueryTaskKillConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::TaskKill*> & GetTaskKillConfigMap() const = 0;        virtual const Struct::TaskLevel* QueryTaskLevelConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::TaskLevel*> & GetTaskLevelConfigMap() const = 0;        virtual const Struct::TaskMaterail* QueryTaskMaterailConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::TaskMaterail*> & GetTaskMaterailConfigMap() const = 0;        virtual const Struct::TaskSystem* QueryTaskSystemConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::TaskSystem*> & GetTaskSystemConfigMap() const = 0;        virtual const Struct::TaskUse* QueryTaskUseConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::TaskUse*> & GetTaskUseConfigMap() const = 0;        virtual const Struct::Tests* QueryTestsConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::Tests*> & GetTestsConfigMap() const = 0;        virtual const Struct::TransPort* QueryTransPortConfig(const Int64 id) const = 0;
        virtual const std::unordered_map<Int64, Struct::TransPort*> & GetTransPortConfigMap() const = 0;
    };
}


