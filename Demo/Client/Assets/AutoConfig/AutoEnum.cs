using System;

namespace AutoConfig
{
    public static class AutoEnum
    {
        public enum ConditionalLogic
        {
            And = 0, //和
            Or = 1, //或
        }

        public enum eActTrack
        {
            Five = 4, //第5
            Four = 3, //第四轨
            One = 0, //第一轨
            Three = 2, //第三轨
            Two = 1, //第二轨
        }

        public enum eAction
        {
            Casting = 2, //
            Charge = 1, //
            None = 0, //
        }

        public enum eAddType
        {
            Current = 1, //当前值加成
            Max = 0, //最大值加成
            None = 2, //非数值加成类型
        }

        public enum eAttribute
        {
            AddDefend = 14, //增加物理防御
            AddMagicDefend = 15, //增加魔法防御
            AtkSpeed = 18, //攻击速度
            Attack = 2, //攻击力
            Blood = 9, //吸血
            CantController = 6, //无法被操作控制
            Critical = 8, //暴击率
            CriticalAtk = 11, //暴击伤害
            DamageReduction = 4, //减伤
            Defend = 10, //防御
            ExpAdd = 13, //加经验
            ExpandVield = 16, //扩大可看视范围
            Hp = 3, //持续改变血量
            Invincible = 5, //无敌
            Mockery = 12, //嘲讽
            MoveSpeed = 1, //移动速度
            Mp = 19, //改变蓝量
            None = 0, //不改变任何属性
            Restraint = 7, //束缚(免除物理伤害)
            Rewind = 21, //击退
            Silent = 20, //沉默
            SuperArmor = 17, //免控霸体（不会受到击退。击飞等buff状态的影响）
        }

        public enum eBallistics
        {
            Line = 0, //直线弹道（在技能行进过程中会产生效果）
            Parabolic = 1, //抛物线弹道（只在技能目的点造成效果）
            Range = 2, //范围性伤害（立即造成伤害）
            Track = 3, //跟踪弹
        }

        public enum eBattleType
        {
            PvE = 1, //不允许pvp的副本只允许pve
            PvP = 0, //允许pvp的副本
        }

        public enum eBoss
        {
            HydraCypress = 0, //九头蛇柏
        }

        public enum eCalculationMethod
        {
            Normal = 1, //实际数值计算
            Percent = 0, //百分比计算
        }

        public enum eConsumeType
        {
            Angry = 2, //怒气值
            Hp = 0, //血
            Mp = 1, //蓝
        }

        public enum eDamagePriority
        {
            Fifth = 4, //第五
            First = 0, //第一
            Fourth = 3, //第四
            None = 5, //无优先级
            Second = 1, //第二
            Third = 2, //第三
        }

        public enum eDamageType
        {
            Fire = 1, //火属性
            Gold = 3, //金属性
            Physic = 0, //物理伤害
            Truth = 4, //真实伤害
            Water = 2, //水属性
        }

        public enum eEquipmentType
        {
            Belt = 4, //腰带
            Cloths = 3, //衣服
            Jewelry = 5, //饰品
            Pants = 2, //裤子
            Shose = 0, //鞋子
            Weapon = 1, //武器
        }

        public enum eFollowType
        {
            Follow = 0, //追随角色
            NoFollow = 1, //不追随角色
        }

        public enum eInfluenceOrganType
        {
            Drop = 1, //掉落
            Prediction = 0, //预测
            Recyle = 2, //回收机关
        }

        public enum eInteractType
        {
            Accpet = 0, //Npc发放任务
            Complete = 2, //Npc完成玩家任务
            Wait = 1, //等待玩家完成任务
        }

        public enum eLifePanType
        {
            HurtLife = 0, //在造成伤害后才能变成正常生命周期，未造成伤害灰提前结束
            Normal = 1, //正常周期
        }

        public enum eMonsterSkillTrack
        {
            EffectAll = 0, //全能听
            EffectSelf = 1, //个人能听到
        }

        public enum eMoveEaseType
        {
            InBack = 25, //
            InBounce = 19, //
            InCirc = 16, //
            InCubic = 4, //
            InElastic = 22, //
            InExpo = 13, //
            InOutBack = 27, //
            InOutBounce = 21, //
            InOutCirc = 18, //
            InOutCubic = 6, //
            InOutElastic = 24, //
            InOutExpo = 15, //
            InOutQuad = 3, //
            InOutQuart = 9, //
            InOutSine = 12, //
            InQuad = 1, //
            InQuart = 7, //
            InSine = 10, //
            Linear = 0, //
            OutBack = 26, //
            OutBounce = 20, //
            OutCirc = 17, //
            OutCubic = 5, //
            OutElastic = 23, //
            OutExpo = 14, //
            OutQuad = 2, //
            OutQuart = 8, //
            OutSine = 11, //
        }

        public enum eMoveType
        {
            EverLineMove = 4, //自由选择目的地直线位移
            Immediate = 2, //瞬间移动
            LineMove = 0, //直线位移
            ParabolaMove = 1, //抛物线位移
            TrackMove = 3, //跟踪位移
        }

        public enum eNpcCareer
        {
            Blacksmith = 0, //铁匠
            Driver = 2, //巴士司机
            Shop = 1, //商店
        }

        public enum eNpcEventType
        {
            SwitchEvent = 3, //打开界面事件
            TalkContent = 1, //谈话事件
            TaskInteract = 0, //任务事件
            TransPort = 2, //传送事件
        }

        public enum eNpcType
        {
            Normal = 0, //正常任务类Npc
            TransPort = 1, //传送类Npc
        }

        public enum ePhysicType
        {
            EnemyCharacter = 1, //敌对角色
            Monster = 0, //怪物
            None = 6, //无
            Npc = 5, //Npc人物
            Organ = 4, //机关类型物品
            ParterCharacter = 3, //友军角色
            StrengthWall = 7, //人和子弹都无法穿过的墙
            Wall = 2, //人无法穿过但子弹可以穿的墙体
        }

        public enum ePosition
        {
            MountBone = 2, //挂在骨骼上
            None = 3, //用不到位置
            TraceHost = 1, //跟随宿主
            WorldPosition = 0, //摆放在世界中
        }

        public enum eRangeType
        {
            Circle = 1, //圆形
            Cube = 2, //长方形
            Ellipse = 4, //椭圆
            None = 3, //
            Sector = 0, //扇形
        }

        public enum eRefreshThing
        {
            Monster = 0, //野怪
            MonsterBoss = 3, //boss
            OrganType = 1, //机关
            Resource = 2, //奖励物品1
        }

        public enum eResources
        {
            Brass = 1, //陨铜币
            Coin = 0, //现金
            Consume = 3, //消耗品
            Equipment = 6, //装备
            FatigueValue = 5, //疲劳值
            Material = 2, //普通材料
            Special = 4, //特殊
        }

        public enum eSceneType
        {
            BattleCopy = 1, //战斗副本
            Public = 0, //公共场景
            PublicBattle = 2, //野外战斗场景
        }

        public enum eSelectedType
        {
            Aim = 0, //指向性（技能点击后必须选择了目标后才能释放）
            Liberty = 2, //没有选择贴花等选择区间只有范围区间。
            UnAim = 1, //非指向性（没有目标也可以释放玩家可以选择技能释放的位置）
        }

        public enum eSheetsType
        {
            Circle = 0, //圆形
            None = 3, //无图形
            Rectangle = 1, //矩形
            sector = 2, //扇形
        }

        public enum eSkillChargeType
        {
            EndEarly = 0, //可以提前释放技能的蓄力（提前结束蓄力一样可以释放出来）
            NoEndEarly = 1, //不可以提前释放技能的蓄力（提前结束蓄力会导致技能无法释放出来）
            None = 4, //不可以蓄力
        }

        public enum eSkillEffect
        {
            Launcher = 0, //发射器
            Move = 2, //位移
            None = 1, //纯用来加buff的技能事件。
        }

        public enum eSkillReleaseType
        {
            Normal = 1, //正常释放的技能（不需要通过吟唱和蓄力来释放的技能）
            Sing = 0, //吟唱性技能（使用后不可以自行决定时长使用后就会一直使用）
        }

        public enum eSkillTriggleType
        {
            Auto = 0, //被动触发
            Proactive = 1, //主动释放触发
        }

        public enum eSkillType
        {
            Launch = 2, //弹道类技能
            Move = 1, //位移性技能
            NonePerfom = 4, //无表现形式的技能
            Organ = 3, //机关型技能
            Range = 0, //范围性技能
        }

        public enum eSoldType
        {
            None = 1, //不可售卖
            Sold = 0, //可售卖
        }

        public enum eSoundType
        {
            EffectAll = 0, //全能听
            EffectSelf = 1, //个人能听到
        }

        public enum eState
        {
            Death = 5, //死亡
            Idle = 0, //站立
            Jumping = 2, //
            Moving = 1, //移动
            Shift = 4, //位移
            Stuned = 3, //
        }

        public enum eSustainabilityType
        {
            AddHp = 2, //在加血后结束
            Again = 1, //需要再次点击同按钮后结束
            None = 0, //非持续性技能在释放后自动结束该技能
        }

        public enum eTargetType
        {
            All = 1, //全部可伤害敌对目标
            Boss = 6, //Bosss
            Character = 5, //全部玩家
            EnemyCharacter = 2, //敌方人物
            Monster = 0, //野怪
            PlayerCharacter = 3, //除我以外的我方队友目标
            PlayerSelf = 4, //玩家自己
        }

        public enum eTaskEvent
        {
            Complete = 2, //玩家已完成该任务
            Completed = 3, //玩家完成并交了
            None = 4, //没有任何状态纯对话
            PreRecive = 0, //玩家待接受任务
            Received = 1, //玩家已接受但未完成
        }

        public enum eTaskReceiveType
        {
            NoTriggle = 1, //达到触发任务条件下后直接触发任务环节
            TriggleNpc = 0, //达到触发任务条件下触碰任务Npc后触发任务环节
        }

        public enum eTaskType
        {
            TaskCollect = 1, //收集型任务
            TaskContact = 4, //在触碰了某个npc时间后完成
            TaskKill = 2, //击杀野怪数量累积型任务
            TaskLevel = 0, //关卡型任务
            TaskMaterail = 5, //收集资料型任务
            TaskUse = 3, //在使用了某件物品后
        }

        public enum eTaskVariety
        {
            DayMission = 3, //日常任务
            MainMission = 0, //主线任务
            SideMission = 1, //支线任务
            SpecialMission = 2, //特殊任务
        }

        public enum eTransPortType
        {
            Evacuate = 1, //撤离点
            Normal = 0, //正常传送点
        }

        public enum eTransactionType
        {
            None = 1, //不可交易
            Tradable = 0, //可交易
        }

        public enum eTriggleCondition
        {
            Angry = 4, //有怒气值
            HpDecrease = 1, //损失血量后可以有条件触发
            Monster = 5, //击杀野怪
            None = 2, //无条件
            Organ = 3, //在视野范围内看到机关
            ReleaseSkill = 0, //释放任意技能后可以有条件触发
        }

        public enum eTriggleType
        {
            Atk = 0, //击打
            Near = 1, //靠近
        }

        public enum eTriggleWay
        {
            None = 1, //正常触发（被动技能就是自动触发的主动技能就是点击一个按钮触发）
            Skill = 0, //释放某个技能后触发
        }

        public enum eValueChangeType
        {
            Add = 0, //加
            Divide = 3, //除
            Multiply = 2, //乘
            Sub = 1, //减
        }

        public enum eVisualSceneType
        {
            Entity = 1, //角色个人视野
            Public = 0, //公共地图视野
        }

        public enum eWeaponType
        {
            Arrow = 0, //弓箭
            BluntWeapon = 1, //钝器
            Dagger = 3, //匕首
            Detonator = 2, //雷管
            None = 5, //非武器
            Sword = 4, //剑
        }

    }

}
