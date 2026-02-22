using System;
using UnityEngine;
using XGame;
using UnityEngine.UI;
using GameUnit;
using XEngine;
using XUtils;
using static GameUnitApi;
using Spine.Unity;
using AutoConfig;
using Spine;
using DG.Tweening;


namespace XGameClient
{
    public interface IBossSkillController : IModule
    {
        enum eEvent
        {

        }
        public void BossSkillPreprocessing(oSkillCastPush body);

    }

    class BossSkillController : IBossSkillController
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<IBossSkillController, BossSkillController>();
        }

        ICharacterManager _CharacterManager;
        IShowFXManager _ShowFXManager; // 添加特效管理器
        IAudioManager _AudioManager; // 添加音频管理器
        IGameObjectSync _GameObjectSync;
        XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {
            _CharacterManager = Launcher.GetModule<ICharacterManager>();
            _ShowFXManager = Launcher.GetModule<IShowFXManager>();
            _AudioManager = Launcher.GetModule<IAudioManager>();
            _GameObjectSync = Launcher.GetModule<IGameObjectSync>();
            _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncCreate, OnBossSyncCreate);
            return true;
        }

        private void OnBossSyncCreate(IUnit unit)//可视化
        {
            if (unit.Type == eUnitType.Boss)
            {
                float bodyhalf = AutoConfig.MonsterBoss.Get(unit.ConfigID)._MonsterBody;
#if UNITY_EDITOR
                var visualization = unit.gameObject.GetComponent<BossRadiusVisualization>();
                if (visualization == null)
                {
                    visualization = unit.gameObject.AddComponent<BossRadiusVisualization>();
                }
                visualization.bossConfigId = (int)unit.ConfigID;
#endif //UNITY_EDITOR
            }
        }

        public void Release()
        {

        }

        public void BossSkillPreprocessing(oSkillCastPush body)//处理boss技能分类boss
        {
            IUnit bosscheck = GameUnitApi.QueryUnit(body.CastGuid);
            AutoEnum.eBoss bossType = AutoConfig.MonsterBoss.Get(bosscheck.ConfigID)._BossType;
            switch (bossType)
            {
                case AutoEnum.eBoss.HydraCypress:
                    Debug.LogWarning("Boss九头蛇柏触发技能");
                    StartHydraCypressSkill(body);
                    break;
            }
        }
        void StartHydraCypressSkill(oSkillCastPush body)
        {
            IUnit bosscheck = GameUnitApi.QueryUnit(body.CastGuid);
            Int64 skillConfigId = body.SkillConfigId;
            bool isChildUse = AutoConfig.MonsterSkill.Get(skillConfigId)._IsSonPart;
            SkeletonAnimation skeletonAnimation = null;
            GameObject childPart = null;
            if (isChildUse == true)
            {
                childPart = bosscheck.transform.Find(AutoConfig.MonsterSkill.Get(skillConfigId)._SonPartName).gameObject;
                skeletonAnimation = bosscheck.transform.Find(AutoConfig.MonsterSkill.Get(skillConfigId)._SonPartName).GetComponent<SkeletonAnimation>();
            }
            else
            {
                skeletonAnimation = bosscheck.gameObject.GetComponent<SkeletonAnimation>();
            }

            float Angle = body.Angle;
            Vector2 targetPos = new Vector2(body.TargetPos.X, body.TargetPos.Y);
            if (bosscheck == null)
            {
                return;
            }
            _AudioManager.PlaySound(AutoConfig.Sound.Get(AutoConfig.MonsterSkill.Get(skillConfigId)._Sound)._AudioMaterial);
            //Debug.LogError($"播放技能{skillConfigId}的音效名为{AutoConfig.Sound.Get(AutoConfig.MonsterSkill.Get(skillConfigId)._Sound)._AudioMaterial}");
            bool isVibe = AutoConfig.MonsterSkill.Get(skillConfigId)._IsVibe;
            if (isVibe == true)//屏幕震动
            {
                //Camera.main.transform.DOShakePosition(0.1f, 0.3f, 20, 90, false, true);
                XApi.TimerApi.StartTimer(OnStartVibe, bosscheck, skillConfigId, (int)AutoConfig.MonsterSkill.Get(skillConfigId)._VibeStartTime, 1, 0);
            }

            if (AutoConfig.MonsterSkill.Get(skillConfigId)._IsMultiple == true)
            {
                if (skeletonAnimation == null)
                {
                    Debug.LogWarning("缺少Spine动画组件");
                    return;
                }
                string previousLoopAnim = AutoConfig.MonsterSkill.Get(skillConfigId)._PreviousLoop;
                string whereLoopAnim = AutoConfig.MonsterSkill.Get(skillConfigId)._WhereLoop;
                if (string.IsNullOrEmpty(previousLoopAnim))
                {
                    Debug.LogWarning("第一段动画名称为空或未设置: " + bosscheck.ConfigID);
                    return;
                }
                // 检查动画是否存在
                if (skeletonAnimation.state.Data.SkeletonData.FindAnimation(previousLoopAnim) == null)
                {
                    Debug.LogWarning("未找到第一段动画: " + previousLoopAnim);
                    return;
                }
                TrackEntry entry1 = skeletonAnimation.state.SetAnimation(0, previousLoopAnim, false);
                entry1.Complete += (trackEntry) =>
                {
                    try
                    {
                        if (!string.IsNullOrEmpty(whereLoopAnim))
                        {
                            if (skeletonAnimation.state.Data.SkeletonData.FindAnimation(whereLoopAnim) != null)
                            {
                                skeletonAnimation.state.SetAnimation(0, whereLoopAnim, true);
                                if (childPart != null)
                                {
                                    XApi.TimerApi.StartTimer(OnAnimationComplete, childPart, skillConfigId, (int)AutoConfig.MonsterSkill.Get(skillConfigId)._LoopTime, 1, 0);
                                }
                                else
                                {
                                    XApi.TimerApi.StartTimer(OnAnimationComplete, bosscheck, skillConfigId, (int)AutoConfig.MonsterSkill.Get(skillConfigId)._LoopTime, 1, 0);
                                }

                                Debug.LogWarning($"播放技能{skillConfigId}的第二段动画{whereLoopAnim}，开第三段计时器");
                            }
                            else
                            {
                                Debug.LogWarning("未找到第二段动画: " + whereLoopAnim);
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        XApi.Error("播放第二段动画时发生异常: " + ex.Message);
                    }
                };
            }
            else//非多段技能动画
            {
                if (string.IsNullOrEmpty(AutoConfig.MonsterSkill.Get(skillConfigId)._ActPrefab))
                {
                    return;
                }
                TrackEntry entry = skeletonAnimation.state.SetAnimation(0, AutoConfig.MonsterSkill.Get(skillConfigId)._ActPrefab, false);
                entry.Complete += (trackEntry) =>
                {
                    skeletonAnimation.state.SetAnimation(0, AutoConfig.MonsterSkill.Get(skillConfigId)._IdleAct, true);
                };
            }
        }

        private void OnStartVibe(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
        {

            XApi.TimerApi.StopTimer(OnStartVibe, obj);
            Int64 skillconfigID = (Int64)context;
            //Debug.LogError($"播放技能{skillconfigID}的屏幕震动");

            float duration = AutoConfig.MonsterSkill.Get(skillconfigID)._VibeDuration ;
            duration = duration*0.001f;
            float strength = AutoConfig.MonsterSkill.Get(skillconfigID)._VibeStrength;
            int vibrato = (int)AutoConfig.MonsterSkill.Get(skillconfigID)._Vibrato;
            float Randomness = AutoConfig.MonsterSkill.Get(skillconfigID)._Randomness;
            bool fadeOut = AutoConfig.MonsterSkill.Get(skillconfigID)._IsFadeOut;

            //Camera.main.transform.DOShakePosition(0.1f, 0.3f, 20, 90, false, true);

            Camera.main.transform.DOShakePosition(duration, strength, vibrato, Randomness, false, fadeOut);
        }

        private void OnAnimationComplete(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
        {
            if (state == XApi.ITimer.eState.END)
            {
                XApi.TimerApi.StopTimer(OnAnimationComplete, obj);
                GameObject bulletUnit = obj as GameObject;
                int skillConfigId = Convert.ToInt32(context);
                AutoConfig.MonsterSkill skill = AutoConfig.MonsterSkill.Get(skillConfigId);
                SkeletonAnimation skeletonAnimation = bulletUnit.GetComponent<SkeletonAnimation>();
                if (skeletonAnimation == null)
                {
                    Debug.LogWarning("缺少Spine动画组件");
                    return;
                }
                TrackEntry entry = skeletonAnimation.state.SetAnimation(0, skill._LaterLoop, false);
                entry.Complete += (trackEntry) =>
                {
                    skeletonAnimation.state.SetAnimation(0, AutoConfig.MonsterSkill.Get(skillConfigId)._IdleAct, false);
                    //Debug.LogError($"播放{skill._LaterLoop},第三段结束");
                };
            }
            ;

        }
    }
}
