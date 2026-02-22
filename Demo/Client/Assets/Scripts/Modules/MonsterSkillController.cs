using System;
using UnityEngine;
using XGame;
using GameUnit;
using XEngine;
using XUtils;
using static GameUnitApi;
using AutoConfig;

namespace XGameClient
{
    public interface IMonsterSkillController : IModule
    {
        public void MonsterSkillPreprocessing(oSkillCastPush body);
    }

    class MonsterSkillController : IMonsterSkillController
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
        static void Regist()
        {
            Launcher.RegistModule<IMonsterSkillController, MonsterSkillController>();
        }
        ICharacterManager _CharacterManager;
        IFiniteStateMachine _FSM;
        IAudioManager _AudioManager;
        IMoveSkillManager _MoveSkillManager;
        ISpineAnimationController _SpineAnimationController;
        IFiniteStateMachine _FiniteStateMachine;
        IShowFXManager _ShowFXManager;
        XEventPool<IUIMain.eEvent> _Events = new XEventPool<IUIMain.eEvent>();

        public bool Initialize()
        {
            return true;
        }

        public bool Launch()
        {
            _CharacterManager = Launcher.GetModule<ICharacterManager>();
            _AudioManager = Launcher.GetModule<IAudioManager>();
            _MoveSkillManager = Launcher.GetModule<IMoveSkillManager>();
            _SpineAnimationController = Launcher.GetModule<ISpineAnimationController>();
            _FiniteStateMachine = Launcher.GetModule<IFiniteStateMachine>();
            _ShowFXManager = Launcher.GetModule<IShowFXManager>();
            _FSM = Launcher.GetModule<IFiniteStateMachine>();
            _FSM.RegistActionJudge(AutoConfig.AutoEnum.eAction.Casting, OnSkillCastingJudge);
            _FSM.RegistAction(AutoConfig.AutoEnum.eAction.Casting, OnSkillCastingAction);

            return true;
        }

        public void Release()
        {

        }

        public void MonsterSkillPreprocessing(oSkillCastPush body)
        {
            //Debug.LogError("怪物技能处理");
            IUnit unit = GameUnitApi.QueryUnit(body.CastGuid);
            if (unit == null)
            {
                XApi.Debug($"找不到单位: {body.CastGuid}");
                return;
            }
            if (unit.Type != eUnitType.Monster)
            {
                XApi.Debug($"单位不是怪物: {body.CastGuid}");
                return;
            }
            UInt64 castGuid = body.CastGuid;
            IUnit onerUnit = GameUnitApi.QueryUnit(castGuid);

            _AudioManager.PlaySound(AutoConfig.Sound.Get(AutoConfig.MonsterSkill.Get(body.SkillConfigId)._Sound)._AudioMaterial);

            _FSM.RunAction(castGuid, AutoConfig.AutoEnum.eAction.Casting, body.SkillConfigId, body.Angle, new Vector2(body.PlacePos.X, body.PlacePos.Y));
        }

        private bool OnSkillCastingJudge(in ulong guid, in AutoEnum.eAction action, in long skillConfigId, in float angle, in Vector2 pos)
        {
            IUnit unit = GameUnitApi.QueryUnit(guid);
            if (unit != null)
            {
                //判断单位是否可以施法
                //Debug.LogError($"单位{guid} 可以施法");
                return true;
            }
            return false;
        }

        private void OnSkillCastingAction(in ulong guid, in AutoEnum.eAction action, in long skillConfigId, in float angle, in Vector2 pos)
        {
            IUnit unit = GameUnitApi.QueryUnit(guid);
            if (unit != null && unit.Type == eUnitType.Monster)
            {
                //Debug.LogError($"单位{guid} 开始施法技能{skillConfigId} 角度{angle} 位置{pos}");
                HandleSkillCasting(unit, skillConfigId, angle, pos);
            }
        }

        private void HandleSkillCasting(IUnit unit, long skillConfigId, float angle, Vector2 pos)//处理技能施法
        {
            //_MoveSkillManager.OnMoveSkillCreate(unit.Guid, angle, skillConfigId, unit.GetUInt64(GameUnit.eCharacterAttr.MoveTargetGuid_UInt64));
            _MoveSkillManager.OnMoveSkillCreate(unit.Guid, angle, skillConfigId, unit.GetUInt64(GameUnit.eMonsterAttr.MoveSkillTargetGuid_UInt64));

            Debug.LogWarning($"技能创建: {unit.Guid}");
            if (skillConfigId == 11078)
            {
                string animationName = AutoConfig.MonsterSkill.Get(skillConfigId)._ActPrefab;

                _SpineAnimationController.HandleAnimation(unit, animationName, false);

            }
            else
            {
                string animationName = AutoConfig.MonsterSkill.Get(skillConfigId)._ActPrefab;

                _SpineAnimationController.HandleAnimation(unit, animationName, false);
            }


            unit.Angle = angle;
            if (angle != 0)
            {
                _SpineAnimationController.FlipCharacterToDirection(unit, angle);//翻转人物，注意角度为0的时候不会翻转，而是按照人物当前方向放技能
            }
            // if(AutoConfig.MonsterSkill.Get(skillConfigId)._SpecialEffects != 0)
            // {
            //     XApi.TimerApi.StartTimer(StartMonsterSkillEffect,unit,skillConfigId,(int)AutoConfig.MonsterSkill.Get(skillConfigId)._SpecialEffectsDelay,1,0);
            // }

            XApi.TimerApi.StartTimer(BackIdle, unit.Guid, null, (int)AutoConfig.MonsterSkill.Get(skillConfigId)._ActTime, 1, 0);

        }

        // private void StartMonsterSkillEffect(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)//技能特效显示
        // {
        //     XApi.TimerApi.StopTimer(StartMonsterSkillEffect, obj);
        //     IUnit unit = obj as IUnit;
        //     Int64 skillConfigId = (Int64)context;
        //     _ShowFXManager.ShowFX(unit.Guid,unit.Guid,AutoConfig.MonsterSkill.Get(skillConfigId)._SpecialEffects,"SkillFX");
        //     XApi.TimerApi.StartTimer(StopFx,unit,unit,(int)AutoConfig.MonsterSkill.Get(skillConfigId)._SpecialEffectTime,1,XApi.ITimer.Unlimited);   
        // }

        // private void StopFx(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
        // {
        //     XApi.TimerApi.StopTimer(StopFx, obj);
        //     IUnit unit = obj as IUnit;

        //     if (unit != null)
        //     {
        //         Transform fxTransform = unit.transform.Find("SkillFX");
        //         if (fxTransform != null)
        //         {
        //             GameObject item = fxTransform.gameObject;
        //             for (int i = item.transform.childCount - 1; i >= 0; i--)
        //             {
        //                 Transform child = item.transform.GetChild(i);
        //                 GameObject.Destroy(child.gameObject);
        //             }
        //         }
        //     }
        // }

        void BackIdle(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
        {
            XApi.TimerApi.StopTimer(BackIdle, obj);
            UInt64 castGuid = (UInt64)obj;
            IUnit castUnit = GameUnitApi.QueryUnit(castGuid);
            if (_FiniteStateMachine.GetState(castGuid) == AutoEnum.eState.Death)
            {
                // 单位已死亡，直接返回
                return;
            }
            if (castUnit == null)
            {
                // 单位已不存在，直接返回
                return;
            }
            if (castUnit.GetInt32(eEntityAttr.CantControllerRef_Int32) > 0)
            {
                //Debug.LogError("在被控制中");
                return;
            }
            _SpineAnimationController.HandleAnimation(castUnit, "idle", true);
            //Debug.LogError("播放idle");
        }
    }
}
