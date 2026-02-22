using System;
using System.Collections.Generic;
using System.Linq;
using AutoConfig;
using GameUnit;
using Spine;
using Spine.Unity;
using TMPro;
using UnityEngine;
using XEngine;
using XGame;
using XGameClient;
using XUtils;
using static GameUnitApi;


public interface ISkillManager : IModule
{
    Int64 GetLastCastTick(in Int64 skillConfigID);

    public void UseSkill(UInt64 skillGuid, Int64 skillConfigId, float angle, oVector2 targetpos, UInt64 targetGuid, oVector2 castPos);//技能使用
    public UInt64 CheckClickOnPlayerAdvanced(eUnitType[] clickType);
    public Int64 GetLastCastTickForCharacter(in UInt64 characterGuid, in long skillConfigId);
}

class SkillManager : ISkillManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]

    static void Regist()
    {
        Launcher.RegistModule<ISkillManager, SkillManager>();
    }
    ICharacterManager _CharacterManager;
    IFiniteStateMachine _FSM;
    IClient _Client;
    IMoveSkillManager _MoveSkillManager;
    IMonsterSkillController _MonsterSkillController;
    IBossSkillController _BossSkillController;
    IUISkillManager _UISkillManager;
    ISpineAnimationController _SpineAnimationController;
    IOptimizationOfOperationalFeel _OptimizationOfOperationalFeel;
    IShowFXManager _ShowFXManager;
    IAudioManager _AudioManager;
    private System.Collections.Generic.Dictionary<Int64, Int64> _SkillLastCastTicks = new System.Collections.Generic.Dictionary<Int64, Int64>();

    private System.Collections.Generic.Dictionary<System.UInt64, System.Collections.Generic.Dictionary<System.Int64, System.Int64>> _CharacterSkillLastCastTicks
       = new System.Collections.Generic.Dictionary<System.UInt64, System.Collections.Generic.Dictionary<System.Int64, System.Int64>>();

    private List<KeyValuePair<UInt64, GameUnitApi.IUnit>> _CachedUnits = new List<KeyValuePair<UInt64, GameUnitApi.IUnit>>();



    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _Client = Launcher.GetModule<IClient>();
        _UISkillManager = Launcher.GetModule<IUISkillManager>();
        _FSM = Launcher.GetModule<IFiniteStateMachine>();
        _ShowFXManager = Launcher.GetModule<IShowFXManager>();
        _OptimizationOfOperationalFeel = Launcher.GetModule<IOptimizationOfOperationalFeel>();
        _MoveSkillManager = Launcher.GetModule<IMoveSkillManager>();
        _MonsterSkillController = Launcher.GetModule<IMonsterSkillController>();
        _SpineAnimationController = Launcher.GetModule<ISpineAnimationController>();
        _Client.RegistProtobuf<oSkillCastPush>(eServerID.SkillCastPush, OnSkillCreate);
        _FSM.RegistEnterJudge(AutoConfig.AutoEnum.eState.Moving, OnMovingEnterJudge);
        _FSM.RegistAction(AutoConfig.AutoEnum.eAction.Casting, OnSkillCastingAction);
        _FSM.RegistActionJudge(AutoConfig.AutoEnum.eAction.Casting, OnSkillCastingJudge);
        _BossSkillController = Launcher.GetModule<IBossSkillController>();

        _AudioManager = Launcher.GetModule<IAudioManager>();
        return true;
    }


    private bool OnMovingEnterJudge(in ulong guid, in AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        IUnit unit = GameUnitApi.QueryUnit(guid);
        if (null == unit)
        {
            Debug.Log($"OnMovingEnterJudge {guid} 不存在");
            return false;
        }

        Int64 lastSkillConfigID = unit.GetInt64(GameUnit.eCharacterAttr.LastSkillConfigID_Int64);
        UInt64 lastSkillUseTime = unit.GetUInt64(GameUnit.eCharacterAttr.LastSkillUseTime_UInt64);

        // 添加调试日志来追踪问题
        if (lastSkillConfigID != 0 && lastSkillUseTime != 0)
        {
            AutoConfig.Skill skillConfig = AutoConfig.Skill.Get(lastSkillConfigID);
            if (skillConfig != null)
            {
                Int64 tick = SafeSystem.Time.GetMillisecondTick() - (Int64)lastSkillUseTime;

                Debug.Log($"[移动判断] GUID:{guid}, 技能ID:{lastSkillConfigID}, 已过时间:{tick}ms, " +
                         $"禁止移动时间:{skillConfig._CantMoveTime}ms, 技能间隔时间:{skillConfig._SkillInterval}ms");

                if (tick <= skillConfig._CantMoveTime)
                {
                    Debug.LogWarning($"角色 {guid} 正在施法冷却中，禁止移动: {tick} <= {skillConfig._CantMoveTime}");
                    return false;
                }

                if (tick <= skillConfig._SkillInterval)
                {
                    Debug.LogWarning($"角色 {guid} 正在施法后摇中，禁止移动: {tick} <= {skillConfig._SkillInterval}");
                    return false;
                }

                // 时间已超过技能间隔，允许移动
                //Debug.LogError($"[移动判断] 时间已超过技能间隔，允许移动: {tick} > {skillConfig._SkillInterval}，时间差：{tick - skillConfig._SkillInterval}");
                return true;
            }
            else
            {
                //打错误日志
                return false;
            }
        }
        else
        {
            //Debug.LogError($"[移动判断] 无技能限制，允许移动: lastSkillConfigID={lastSkillConfigID}, lastSkillUseTime={lastSkillUseTime}，时间差：{SafeSystem.Time.GetMillisecondTick() - (Int64)lastSkillUseTime}");
        }

        return true;
    }


    public void Release()
    {

    }
    private void OnSkillCastingAction(in ulong guid, in AutoEnum.eAction action, in long skillConfigId, in float angle, in Vector2 pos)
    {
        IUnit unit = GameUnitApi.QueryUnit(guid);
        if (unit != null && unit.Type == eUnitType.Character)
        {
            //Debug.LogError($"单位{guid} 开始施法技能{skillConfigId} 角度{angle} 位置{pos}");
            HandleSkillCasting(unit, skillConfigId, angle, pos);
        }
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

    private void HandleSkillCasting(IUnit unit, long skillConfigId, float angle, Vector2 pos)//处理技能施法
    {
        if (AutoConfig.Skill.Get(skillConfigId)._IsNromalAtk == true)
        {
            if (unit.gameObject.GetComponent<SkeletonAnimation>() != null)
            {
                SkeletonAnimation skeletonAnimation = unit.gameObject.GetComponent<SkeletonAnimation>();
                skeletonAnimation.state.ClearTracks(); // 清除所有轨道上的动画
            }
            HandheldNormalAttack(unit, skillConfigId);
        }
        else
        {
            if (unit.Type == eUnitType.Character)
            {
                _MoveSkillManager.OnMoveSkillCreate(unit.Guid, angle, skillConfigId, unit.GetUInt64(GameUnit.eCharacterAttr.MoveTargetGuid_UInt64));
                _SkillLastCastTicks[skillConfigId] = SafeSystem.Time.GetMillisecondTick();
                Debug.LogWarning($"技能创建: {unit.Guid}，{_SkillLastCastTicks[skillConfigId]}");
            }

            string animationName = AutoConfig.Skill.Get(skillConfigId)._ActPrefab;
            bool isloop = AutoConfig.Skill.Get(skillConfigId)._ActIsLoop;
            if (unit.gameObject.GetComponent<SkeletonAnimation>() != null)
            {
                SkeletonAnimation skeletonAnimation = unit.gameObject.GetComponent<SkeletonAnimation>();
                skeletonAnimation.state.ClearTracks(); // 清除所有轨道上的动画
            }

            _SpineAnimationController.HandleAnimation(unit, animationName, isloop);
            if (AutoConfig.Skill.Get(skillConfigId)._CanSeparated == true)
            {
                unit.Angle = angle;
                if (angle != 0)
                {
                    _SpineAnimationController.FlipCharacterToDirection(unit, angle);//翻转人物，注意角度为0的时候不会翻转，而是按照人物当前方向放技能
                }
            }
            else
            {

                unit.Angle = angle;
                if (angle != 0)
                {
                    _SpineAnimationController.FlipCharacterToDirection(unit, angle);
                }
            }
            XApi.TimerApi.StartTimer(BackIdle, unit.Guid, null, (int)AutoConfig.Skill.Get(skillConfigId)._ActTime, 1, 0);
        }
    }

    private void HandheldNormalAttack(IUnit unit, long skillConfigId)//处理普通攻击
    {
        GameObject attackBtn = GameObjectCommon.GetGameObjectByPath("Canvas/UIGameController/UIJoyButton/UIAttack");
        if (attackBtn != null)
        {
            attackBtn.GetComponent<UINormalAttack>().ChangeAtkInfo(unit.Guid, skillConfigId, SafeSystem.Time.GetMillisecondTick());
        }

        SkeletonAnimation skeletonAnimation = unit.gameObject.GetComponent<SkeletonAnimation>();
        //Debug.LogError("收到平A攻击，ID是" + skillConfigId);
        if (unit.Angle != 0)
        {
            _SpineAnimationController.FlipCharacterToDirection(unit, unit.Angle);
        }

        //播动画
        if (skillConfigId == AutoConfig.Character.Get(unit.ConfigID)._Atk1)//第一段普通攻击
        {
            if (AutoConfig.Skill.Get(skillConfigId)._IsPost == true)//是否后摇
            {
                TrackEntry entry1 = skeletonAnimation.state.SetAnimation(0, AutoConfig.Skill.Get(skillConfigId)._ActPrefab, false);

                entry1.Complete += (trackEntry) =>//播完播第二段
              {
                  //Debug.LogError("播放第一段动画");
                  TrackEntry postEntry = skeletonAnimation.state.SetAnimation(0, AutoConfig.Skill.Get(skillConfigId)._PostName, false);
                  postEntry.Complete += (postTrackEntry) =>
                  {
                      if (unit.GetInt32(eEntityAttr.CantControllerRef_Int32) <= 0)
                      {
                          _SpineAnimationController.HandleAnimation(unit, "idle", true);
                      }

                      //Debug.LogError("播放技能" + body.SkillConfigId + "后摇完成，回到idle");
                  };
              };
            }
            else
            {
                TrackEntry entry = skeletonAnimation.state.SetAnimation(0, AutoConfig.Skill.Get(skillConfigId)._ActPrefab, false);
                entry.Complete += (trackEntry) =>
                {
                    //_SpineAnimationController.HandleAnimation(unit, "idle", true);
                    //Debug.LogError("播放技能" + body.SkillConfigId + "完成，回到idle");
                    if (unit.GetInt32(eEntityAttr.CantControllerRef_Int32) <= 0)
                    {
                        _SpineAnimationController.HandleAnimation(unit, "idle", true);
                    }
                };
            }
        }
        else if (skillConfigId == AutoConfig.Character.Get(unit.ConfigID)._Atk2)
        {
            if (unit.Angle != 0)
            {
                _SpineAnimationController.FlipCharacterToDirection(unit, unit.Angle);
            }
            if (AutoConfig.Skill.Get(skillConfigId)._IsPost == true)//是否后摇
            {
                TrackEntry entry1 = skeletonAnimation.state.SetAnimation(0, AutoConfig.Skill.Get(skillConfigId)._ActPrefab, false);
                entry1.Complete += (trackEntry) =>//播完播第二段
                {
                    // Debug.LogError("播放第二段动画");
                    TrackEntry postEntry = skeletonAnimation.state.SetAnimation(0, AutoConfig.Skill.Get(skillConfigId)._PostName, false);
                    postEntry.Complete += (postTrackEntry) =>
                    {
                        //_SpineAnimationController.HandleAnimation(unit, "idle", true);
                        //Debug.LogError("播放技能" + skillConfigId + "后摇完成，回到idle");
                        if (unit.GetInt32(eEntityAttr.CantControllerRef_Int32) <= 0)
                        {
                            _SpineAnimationController.HandleAnimation(unit, "idle", true);
                        }
                    };
                };
            }
            else
            {
                TrackEntry entry = skeletonAnimation.state.SetAnimation(0, AutoConfig.Skill.Get(skillConfigId)._ActPrefab, false);
                entry.Complete += (trackEntry) =>
                {
                    //_SpineAnimationController.HandleAnimation(unit, "idle", true);
                    //Debug.LogError("播放技能" + body.SkillConfigId + "完成，回到idle");
                    if (unit.GetInt32(eEntityAttr.CantControllerRef_Int32) <= 0)
                    {
                        _SpineAnimationController.HandleAnimation(unit, "idle", true);
                    }
                };
            }
        }
        else if (skillConfigId == AutoConfig.Character.Get(unit.ConfigID)._Atk3)
        {
            if (unit.Angle != 0)
            {
                _SpineAnimationController.FlipCharacterToDirection(unit, unit.Angle);
            }
            //Debug.LogError("播放第三段动画");
            TrackEntry entry = skeletonAnimation.state.SetAnimation(0, AutoConfig.Skill.Get(skillConfigId)._ActPrefab, false);
            entry.Complete += (trackEntry) =>
            {
                //_SpineAnimationController.HandleAnimation(unit, "idle", true);
                //Debug.LogError("播放技能" + body.SkillConfigId + "完成，回到idle");
                if (unit.GetInt32(eEntityAttr.CantControllerRef_Int32) <= 0)
                {
                    _SpineAnimationController.HandleAnimation(unit, "idle", true);
                }
            };
        }
    }

    public Int64 GetLastCastTick(in long skillConfigId)
    {
        if (_SkillLastCastTicks.ContainsKey(skillConfigId))
        {
            return _SkillLastCastTicks[skillConfigId];
        }
        return 0;
    }

    public UInt64 CheckClickOnPlayerAdvanced(eUnitType[] clickType)
    {
        // var players = GameUnitApi.GetUnitsByType(eUnitType.Monster);
        if (Camera.main == null)
        {
            Debug.LogWarning("未找到主摄像机");
            return 0;
        }
        _CachedUnits.Clear();
        foreach (eUnitType type in clickType)
        {
            var units = GameUnitApi.GetUnitsByType(type);
            if (units != null)
            {
                _CachedUnits.AddRange(units);
            }
        }
        if (_CachedUnits.Count == 0)
        {
            Debug.LogWarning($"未找到点击类型为{string.Join(",", clickType)}的单位");
            return 0;
        }
        Vector2 mouseWorldPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        foreach (var unit in _CachedUnits.OrderByDescending(p => p.Value.Position.y))
        {
            if (unit.Value == null) continue;
            Bounds bounds = GetSpineBounds(unit.Value.gameObject);
            //Debug.LogWarning($"单位: {unit.Value.Guid}，类型: {unit.Value.Type}");
            if (bounds.Contains(new Vector3(mouseWorldPos.x, mouseWorldPos.y, bounds.center.z)))
            {
                GameUnitApi.IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
                Debug.LogWarning($"精确点击到单位: {unit.Value.Guid}，主角色GUID: {mainCharacter.Guid}");
                return unit.Value.Guid;
            }
        }
        return 0;
    }

    Bounds GetSpineBounds(GameObject spineObject)
    {
        Bounds bounds = new Bounds(spineObject.transform.position, Vector3.zero);

        SkeletonAnimation skeletonAnimation = spineObject.GetComponent<SkeletonAnimation>();
        if (skeletonAnimation != null && skeletonAnimation.skeleton != null)
        {
            Spine.Skeleton skeleton = skeletonAnimation.skeleton;

            float minX = float.MaxValue, minY = float.MaxValue;
            float maxX = float.MinValue, maxY = float.MinValue;

            foreach (Spine.Bone bone in skeleton.Bones)
            {
                if (bone.Active)
                {
                    float x = bone.WorldX;
                    float y = bone.WorldY;

                    minX = Math.Min(minX, x);
                    minY = Math.Min(minY, y);
                    maxX = Math.Max(maxX, x);
                    maxY = Math.Max(maxY, y);
                }
            }
            if (minX != float.MaxValue && minY != float.MaxValue &&
                maxX != float.MinValue && maxY != float.MinValue)
            {
                // 计算中心点和尺寸
                bounds.center = new Vector3((minX + maxX) / 2, (minY + maxY) / 2, 0);
                bounds.size = new Vector3(maxX - minX, maxY - minY, 0.1f);

                // 转换为中心点到世界坐标
                bounds.center = spineObject.transform.TransformPoint(bounds.center);
            }
            else
            {
                bounds.center = spineObject.transform.position;
                bounds.size = new Vector3(1.5f, 2f, 0.1f);
            }
        }
        else
        {
            bounds.center = spineObject.transform.position;
            bounds.size = new Vector3(1.5f, 2f, 0.1f);
        }
        return bounds;
    }

    public void UseSkill(UInt64 skillGuid, Int64 skillConfigId, float angle, oVector2 targetpos, UInt64 targetGuid, oVector2 castPos)//技能使用
    {
        oSkillCastReq skillCastReq = new oSkillCastReq();
        skillCastReq.SkillGuid = skillGuid;
        skillCastReq.SkillConfigId = skillConfigId;
        skillCastReq.Angle = angle;
        //skillCastReq.TargetPos = targetpos;
        skillCastReq.PlacePos = targetpos;
        skillCastReq.TargetGuid = targetGuid;
        skillCastReq.CastPosition = castPos;
        //Debug.LogError("发送技能请求" + skillConfigId);
        IUnit self = _CharacterManager.GetMainCharacterUnit();
        UInt64 lastSkillUseTime = self.GetUInt64(GameUnit.eCharacterAttr.LastSkillUseTime_UInt64);
        Int64 lastSkillConfigID = self.GetInt64(GameUnit.eCharacterAttr.LastSkillConfigID_Int64);
        if (self.GetInt64(GameUnit.eCharacterAttr.LastSkillConfigID_Int64) == 0)
        {
            _Client.SendProtobuf(eClientID.SkillCastReq, skillCastReq);
            return;
        }
        else if (SafeSystem.Time.GetMillisecondTick() - (long)lastSkillUseTime >= AutoConfig.Skill.Get(lastSkillConfigID)._SkillInterval)
        {
            _Client.SendProtobuf(eClientID.SkillCastReq, skillCastReq);
        }
        else
        {
            // XApi.Debug("技能无法释放，上个技能后摇时间未到");
            // Debug.LogWarning("技能无法释放，上个技能后摇时间未到");
            onSkillLastUse(skillGuid, skillConfigId, angle, targetpos, targetGuid, castPos);
        }
    }

    void onSkillLastUse(UInt64 skillGuid, Int64 skillConfigId, float angle, oVector2 targetpos, UInt64 targetGuid, oVector2 castPos)
    {
        IUnit self = _CharacterManager.GetMainCharacterUnit();
        UInt64 lastSkillUseTime = self.GetUInt64(GameUnit.eCharacterAttr.LastSkillUseTime_UInt64);
        //Debug.LogError($"技能进入优化释放，技能ID:{skillConfigId}");
        _OptimizationOfOperationalFeel.OptimizationCastSkill((UInt64)SafeSystem.Time.GetMillisecondTick() - lastSkillUseTime, new IOptimizationOfOperationalFeel.OptimizationSkillInfo()
        {
            SkillID = skillGuid,
            skillConfigid = skillConfigId,
            angle = angle,
            targetpos = targetpos,
            targetGuid = targetGuid,
            castPos = castPos,
        }, (info) => skillLastCallBack(info)//UseSkill(info.SkillID, info.skillConfigid, info.angle, info.targetpos, info.targetGuid, info.castPos)
);
    }

    void skillLastCallBack(IOptimizationOfOperationalFeel.OptimizationSkillInfo info)
    {

        UseSkill(info.SkillID, info.skillConfigid, info.angle, info.targetpos, info.targetGuid, info.castPos);
    }

    void OnSkillCreate(oSkillCastPush body)//技能创建
    {
        IUnit bosscheck = GameUnitApi.QueryUnit(body.CastGuid);
        {
            if (bosscheck.Type == eUnitType.Boss)
            {
                _BossSkillController.BossSkillPreprocessing(body);
                return;
            }
            if (bosscheck.Type == eUnitType.Monster)
            {
                _MonsterSkillController.MonsterSkillPreprocessing(body);
                //Debug.LogError("怪物技能处理");
                return;
            }
        }

        if (AutoConfig.Skill.Get(body.SkillConfigId)._SpecialEffects != 0)
        {
            XApi.TimerApi.StartTimer(StartSkillEffect, body.CastGuid, body.SkillConfigId, (int)AutoConfig.Skill.Get(body.SkillConfigId)._SpecialEffectsDelay, 1, 0);
        }
        //播放音效
        _AudioManager.PlaySound(AutoConfig.Sound.Get(AutoConfig.Skill.Get(body.SkillConfigId)._Sound)._AudioMaterial);

        UInt64 castGuid = body.CastGuid;
        UInt64 onerGuid = body.CastGuid;
        IUnit onerUnit = GameUnitApi.QueryUnit(onerGuid);
        if (onerUnit.Type == eUnitType.Character)
        {
            onerUnit.SetUInt64(GameUnit.eCharacterAttr.MoveTargetGuid_UInt64, body.TargetGuid);
            onerUnit.SetUInt64(GameUnit.eCharacterAttr.LastSkillUseTime_UInt64, (UInt64)SafeSystem.Time.GetMillisecondTick());
            onerUnit.SetInt64(GameUnit.eCharacterAttr.LastSkillConfigID_Int64, body.SkillConfigId);
        }

        if (!AutoConfig.Skill.Get(body.SkillConfigId)._CanMove)
        {
            _FSM.EnterState(castGuid, AutoEnum.eState.Idle, body.Angle, onerUnit.Position, true);
            //Debug.LogError("技能不能移动");
        }

        _FSM.RunAction(castGuid, AutoConfig.AutoEnum.eAction.Casting, body.SkillConfigId, body.Angle, new Vector2(body.PlacePos.X, body.PlacePos.Y));

        if (AutoConfig.Skill.Get(body.SkillConfigId)._IsNromalAtk == false)
        {
            UpdateCharacterSkillCooldown(onerGuid, body.SkillConfigId);
        }
    }
    private void StartSkillEffect(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
    {
        if (state == XApi.ITimer.eState.END)
        {
            XApi.TimerApi.StopTimer(StartSkillEffect, obj);
            ulong castGuid = (ulong)obj;
            long skillConfigId = (long)context;
            _ShowFXManager.ShowSkillChargeFX(castGuid, skillConfigId, AutoConfig.Skill.Get(skillConfigId)._IsEffectLoop);
            //Debug.LogError($"技能{skillConfigId}开始蓄力效果，销毁延迟{(int)AutoConfig.Skill.Get(skillConfigId)._EffectCycleLife}");
            IUnit castUnit = GameUnitApi.QueryUnit(castGuid);
            XApi.TimerApi.StartTimer(EndOfSkillFX, castUnit, null, (int)AutoConfig.Skill.Get(skillConfigId)._EffectCycleLife, 1, 0);
        }
    }

    private void EndOfSkillFX(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
    {
        if (state == XApi.ITimer.eState.END)
        {
            XApi.TimerApi.StopTimer(EndOfSkillFX, obj);
            IUnit castUnit = (IUnit)obj;
            _ShowFXManager.DeletSkillFX(castUnit.Guid);
            // Debug.LogError($"技能结束蓄力效果");
        }
    }

    public Int64 GetLastCastTickForCharacter(in UInt64 characterGuid, in long skillConfigId)
    {
        if (_CharacterSkillLastCastTicks.ContainsKey(characterGuid))
        {
            var characterSkills = _CharacterSkillLastCastTicks[characterGuid];
            if (characterSkills.ContainsKey(skillConfigId))
            {
                return characterSkills[skillConfigId];
            }
        }
        return 0;
    }
    private void UpdateCharacterSkillCooldown(UInt64 characterGuid, Int64 skillConfigId)
    {
        if (!_CharacterSkillLastCastTicks.ContainsKey(characterGuid))
        {
            _CharacterSkillLastCastTicks[characterGuid] = new System.Collections.Generic.Dictionary<System.Int64, System.Int64>();
        }

        _CharacterSkillLastCastTicks[characterGuid][skillConfigId] = SafeSystem.Time.GetMillisecondTick();
    }

    void BackIdle(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
    {
        XApi.TimerApi.StopTimer(BackIdle, obj);
        UInt64 castGuid = (UInt64)obj;
        IUnit castUnit = GameUnitApi.QueryUnit(castGuid);
        if (_FSM.GetState(castGuid) == AutoEnum.eState.Death)
        {
            // 单位已死亡，直接返回
            return;
        }
        if (castUnit == null)
        {
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
