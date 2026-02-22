using System;
using AutoConfig;
using GameUnit;
using Unity.Mathematics;
using UnityEngine;
using UnityEngine.Rendering.Universal;
using XEngine;
using XGame;
using XGameClient;
using XUtils;
using static GameUnitApi;

public interface IMoveSkillManager : IModule
{
    public void OnMoveSkillCreate(UInt64 CastGuid, float angle, Int64 skillConfigID, UInt64 TargetGuid);//移动技能创建
}

public class TrackMoveContext
{
    public UInt64 TargetGuid { get; set; }
    public Int64 SkillId { get; set; }

    public TrackMoveContext(UInt64 targetGuid, Int64 skillId)
    {
        TargetGuid = targetGuid;
        SkillId = skillId;
    }
}
class MoveSkillManager : IMoveSkillManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IMoveSkillManager, MoveSkillManager>();
    }

    IShowFXManager _ShowFXManager;
    IClient _Client;
    IGameSceneManager _GameScene;
    ISpineAnimationController _SpineAnimationController;
    ITilemapManage _TilemapManage;
    IFiniteStateMachine _FSM;

    IGameObjectSync _GameObjectSync;

    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _SpineAnimationController = Launcher.GetModule<ISpineAnimationController>();
        _GameObjectSync = Launcher.GetModule<IGameObjectSync>();
        _ShowFXManager = Launcher.GetModule<IShowFXManager>();
        _GameScene = Launcher.GetModule<IGameSceneManager>();
        _TilemapManage = Launcher.GetModule<ITilemapManage>();
        _FSM = Launcher.GetModule<IFiniteStateMachine>();
        _FSM.RegistExitJudge(AutoConfig.AutoEnum.eState.Shift, OnExitMoveSkillJudge);

        _Client.RegistProtobuf<oSkillEndPush>(eServerID.SkillEndPush, OnMoveSkillRelease);
        _FSM.RegistStateEnter(AutoConfig.AutoEnum.eState.Shift, OnMoveSkillEnter);
        _FSM.RegistStateExit(AutoConfig.AutoEnum.eState.Shift, OnMoveSkillExit);

        _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncRelease, (IUnit goUnit) =>
        {
            if (goUnit.Type == GameUnit.eUnitType.Character)
            {
                OnSyncRelease(goUnit);
            }
        });


        return true;
    }

    public void Release()
    {

    }

    public void OnMoveSkillCreate(UInt64 CastGuid, float Angle, Int64 skillConfigID, UInt64 TargetGuid)//移动技能创建
    {
        UInt64 guid = CastGuid;
        float angle = Angle;
        //Debug.LogError("角度" + angle + "技能" + skillConfigID);
        IUnit skillUnit = GameUnitApi.QueryUnit(guid);
        //AutoConfig.Skill skillConfig = null;
        object skillConfigObject = null;
        AutoConfig.Skill skillConfigAsSkill = null;
        AutoConfig.MonsterSkill skillConfigAsMonsterSkill = null;
        if (skillUnit.Type == eUnitType.Character)
        {
            skillConfigAsSkill = AutoConfig.Skill.Get(skillConfigID);
            skillConfigObject = skillConfigAsSkill;
        }
        else if (skillUnit.Type == eUnitType.Monster)
        {
            skillConfigAsMonsterSkill = AutoConfig.MonsterSkill.Get(skillConfigID);
            skillConfigObject = skillConfigAsMonsterSkill;
        }

        AutoEnum.eSkillEffect[] skillEffects = null;
        Int64[] skillEffectConfigIDs = null;
        if (skillConfigAsSkill != null)
        {
            skillEffects = skillConfigAsSkill._SkillEffect;
            skillEffectConfigIDs = skillConfigAsSkill._SkillEffectConfigID;
        }
        else if (skillConfigAsMonsterSkill != null)
        {
            skillEffects = skillConfigAsMonsterSkill._SkillEffect;
            skillEffectConfigIDs = skillConfigAsMonsterSkill._SkillEffectConfigID;
        }


        Int64 moveId = AutoConfig.Move.INVALID_CONFIG_ID;
        for (int i = 0; i < skillEffects.Length; i++)
        {
            // 检查是否是位移类型的技能效果
            if (skillEffects[i] == AutoEnum.eSkillEffect.Move && i < skillEffectConfigIDs.Length)
            {
                moveId = skillEffectConfigIDs[i];
                if (moveId == AutoConfig.Move.INVALID_CONFIG_ID)
                {
                    Debug.LogWarning("非移动技能效果" + skillEffects[i]);
                    return;
                }
                if (moveId == 0)
                {
                    XApi.Error($"移动技能配置ID为0,请检查技能配置是否正确！" + skillConfigID + ":" + skillEffectConfigIDs.Length);
                }
                break;
            }
            else
            {
                Debug.LogWarning("技能" + skillConfigID + "没有位移技能效果");
            }
        }
        if (moveId == AutoConfig.Move.INVALID_CONFIG_ID)
        {
            return;
        }

        if (skillUnit != null)
        {
            skillUnit.SetFloat(GameUnit.eEntityAttr.SkillAngle_Float, angle);
            skillUnit.SetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64, moveId);
            skillUnit.SetUInt64(GameUnit.eEntityAttr.MoveSkillTargetGuid_UInt64, TargetGuid);
            if (skillUnit.GetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64) == 0)
            {
                XApi.Error("技能ID为0,请检查技能配置是否正确！");
            }
            // skillUnit.Angle = angle;
            // if (angle != 0)
            // {
            //     _SpineAnimationController.FlipCharacterToDirection(skillUnit, angle);//翻转人物，注意角度为0的时候不会翻转，而是按照人物当前方向放技能
            // }

            AutoEnum.eMoveType type = AutoConfig.Move.Get(moveId)._MoveType;
            switch (type)
            {
                case AutoConfig.AutoEnum.eMoveType.LineMove://直线移动技能 
                    skillUnit.SetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64, moveId);

                    XApi.TimerApi.StartTimer(StartLineMoveCharge, skillUnit, null, (int)AutoConfig.Move.Get(moveId)._DelayTime, 1, 0);//启动蓄力动作计时器
                     Int64 actualSkillID = skillConfigID;  
                     _ShowFXManager.ShowSkillChargeFX(guid, actualSkillID);                                                                                                      
                    //_ShowFXManager.ShowSkillChargeFX(guid, skillConfig._ID);//蓄力特效
                    //_SpineAnimationController.HandleAnimation(skillUnit, "Move", false);//动画
                    break;
                // case AutoConfig.AutoEnum.eMoveType.ParabolaMove://抛物线移动技能
                //     Debug.LogWarning($"抛物线技能起点坐标{skillUnit.Position}，目标坐标X{skillUnit.GetFloat(GameUnit.eBulletAttr.TargetPosX_Float)}，目标坐标Y{skillUnit.GetFloat(GameUnit.eBulletAttr.TargetPosY_Float)}");
                //     skillUnit.SetInt64(GameUnit.eMoveSkillAttr.MoveSkillConfigID_Int64, skillid);
                //     XApi.TimerApi.StartTimer(StartParabolaMoveCharge, skillUnit, null, 0, 0, (int)AutoConfig.Move.Get(skillid)._DelayTime);//启动抛物线蓄力动作计时器
                //     _ShowFXManager.ShowSkillMoveFX(guid, skillid);//特效
                //     _SpineAnimationController.HandleAnimation(skillUnit, "Move", false);//动画
                //     break;
                case AutoConfig.AutoEnum.eMoveType.TrackMove://追踪移动技能
                    skillUnit.SetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64, moveId);
                    var context = new TrackMoveContext(TargetGuid, moveId);
                    XApi.TimerApi.StartTimer(StartTrackMoveCharge, skillUnit, context, (int)AutoConfig.Move.Get(moveId)._DelayTime, 1, 0);//转向
                    _ShowFXManager.ShowSkillMoveFX(guid, moveId);//特效
                    //_SpineAnimationController.HandleAnimation(skillUnit, "Move", false);//动画
                    break;
            }
        }
    }

    void OnMoveSkillRelease(oSkillEndPush body)//移动技能释放
    {
        UInt64 guid = body.CastGuid;
        IUnit skillUnit = GameUnitApi.QueryUnit(guid);
        if (skillUnit == null)
        {
            XApi.Debug($"OnMoveSkillRelease {guid} ,GUID单位不存在");
            return;
        }
        skillUnit.CancelRegistUpdate(UpdateLineMove);
        //skillUnit.CancelRegistUpdate(UpdateParabolicMove);
        skillUnit.CancelRegistUpdate(UpdateTrackMove);
        XApi.TimerApi.StopTimer(StartLineMoveCharge, skillUnit);
        //XApi.TimerApi.StopTimer(StartParabolaMoveCharge, skillUnit);
        XApi.TimerApi.StopTimer(StartTrackMoveCharge, skillUnit);
        XApi.TimerApi.StopTimer(TurnTrackMove, skillUnit);
        Int64 moveId = skillUnit.GetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64);
        if (moveId == 0)
        {
            XApi.Debug($"moveId为0");
            return;
        }

        var skillConfig = AutoConfig.Move.Get(moveId);
        if (skillConfig == null)
        {
            XApi.Debug($"OnMoveSkillRelease {guid} ,移动技能配置不存在");
            return;
        }
        if (skillConfig != null)
        {
            switch (skillConfig._MoveType)
            {
                case AutoConfig.AutoEnum.eMoveType.LineMove://直线
                    Debug.LogWarning("直线最终坐标" + skillUnit.Position);
                    StopMoveSkill(skillUnit);
                    break;
                // case AutoConfig.AutoEnum.eMoveType.ParabolaMove://抛物线
                //     //Debug.LogWarning($"抛物线技能{skillUnit.ConfigID}到达终点" + skillUnit.Position);
                //     StopMoveSkill(skillUnit);
                //     break;
                case AutoConfig.AutoEnum.eMoveType.TrackMove://追踪
                    //Debug.LogWarning($"追踪技能{skillUnit.ConfigID}到达终点" + skillUnit.Position);
                    StopMoveSkill(skillUnit);
                    break;
            }
        }
    }

    void StartLineMoveCharge(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)//直线移动技能开始蓄力
    {
        if (!(obj is IUnit skillUnit)) return;
        if (state == XApi.ITimer.eState.END) //&& murder == false)
        {
            _ShowFXManager.DeletSkillFX(skillUnit.Guid);//清理特效
            if (AutoConfig.Move.Get(skillUnit.GetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64))._MoveEffect != 0)
            {
                _ShowFXManager.ShowSkillMoveFX(skillUnit.Guid, skillUnit.GetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64));
            }
            LineMoveCheck(skillUnit);//检测是否会碰撞
            skillUnit.RegistUpdate(UpdateLineMove);//注册位移事件update
            Debug.LogWarning($"UpdateLineMove registered for skillUnit GUID: {skillUnit.Guid}");
        }
    }
    void UpdateLineMove(IUnit skillUnit)//直线移动位置
    {
        Vector2 startpos = skillUnit.Position;//起点
        float speed = skillUnit.GetFloat(GameUnit.eBulletAttr.MoveSpeed_Float);//基本速度
        float addspeed = AutoConfig.Move.Get(skillUnit.GetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64))._AddSpeed;//加速度
        float angle = skillUnit.GetFloat(GameUnit.eEntityAttr.SkillAngle_Float);

        speed = speed + addspeed * Time.deltaTime;//速度
        skillUnit.SetFloat(GameUnit.eEntityAttr.MoveSpeed_Float, speed);
        Vector2 direction = new Vector2(Mathf.Cos(angle * Mathf.Deg2Rad), Mathf.Sin(angle * Mathf.Deg2Rad));//方向
        Vector2 pos = skillUnit.Position + direction * speed * Time.deltaTime;

        //Vector2 hitPos = Vector2.zero;
        //float distance = Vector2.Distance(skillUnit.Position, pos);
        //hitPos = skillUnit.GetVector2(GameUnit.eEntityAttr.SkillTargetPos_Vector2);
        // float distanceToTarget = Vector2.Distance(skillUnit.Position, hitPos);
        // if (distanceToTarget < 0.01f)
        // {
        //     Debug.Log("[追踪技能] 检测到碰撞，停止追踪");

        //     skillUnit.CancelRegistUpdate(UpdateLineMove);
        //     _ShowFXManager.DeletSkillFX(skillUnit.Guid);//清理特效
        //     return;
        // }

        skillUnit.Position = pos;

        //XApi.Error($"End Move:: {pos.x} {pos.y}");
        //Debug.LogWarning($"移动到" + skillUnit.Position);
    }

    // void StartParabolaMoveCharge(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)//抛物线移动技能开始蓄力
    // {
    //     if (!(obj is IUnit skillUnit)) return;
    //     if (state == XApi.ITimer.eState.END && murder == false)
    //     {
    //         skillUnit.RegistUpdate(UpdateParabolicMove);//注册位移事件update
    //     }
    // }

    // void UpdateParabolicMove(IUnit skillUnit)
    // {
    //     //Vector2 startpos = skillUnit.GetVector2(GameUnit.eBulletAttr.StartPos_Vector2);//起点
    //     Vector2 startpos = skillUnit.Position;//起点
    //     Vector2 endpos = new Vector2(skillUnit.GetFloat(GameUnit.eBulletAttr.TargetPosX_Float), skillUnit.GetFloat(GameUnit.eBulletAttr.TargetPosY_Float));//终点
    //     float lifetime = AutoConfig.Move.Get(skillUnit.ConfigID)._Time / 1000f;//生命周期

    //     if (lifetime <= 0)
    //     {
    //         skillUnit.Position = endpos;
    //         return;
    //     }
    //     float directDistance = Vector2.Distance(endpos, startpos);//直接距离
    //     float deltaTime = skillUnit.GetFloat(GameUnit.eBulletAttr.DeltaTime_Float);//已经使用的时间
    //     float progress = deltaTime / lifetime;
    //     progress = Mathf.Clamp01(progress);//获得时间比例0-1

    //     deltaTime = deltaTime + Time.deltaTime;//增加时间
    //     skillUnit.SetFloat(GameUnit.eBulletAttr.DeltaTime_Float, deltaTime); //存储已用时间

    //     Vector2 midPoint = (startpos + endpos) * 0.5f;//中点

    //     float heightOffset = directDistance * AutoConfig.Move.Get(skillUnit.ConfigID)._HeightParameter;//高度偏移
    //     float bulletAngle = skillUnit.Angle;
    //     Vector2 bulletDirection = new Vector2(Mathf.Cos(bulletAngle * Mathf.Deg2Rad), Mathf.Sin(bulletAngle * Mathf.Deg2Rad));
    //     Vector2 perpendicular = new Vector2(-bulletDirection.y, bulletDirection.x);
    //     Vector2 controlPoint = midPoint + perpendicular * heightOffset * 0.5f;

    //     if (progress >= 1.0f)
    //     {
    //         skillUnit.Position = endpos;
    //         return;
    //     }

    //     if (skillUnit.Position.x == skillUnit.GetFloat(GameUnit.eBulletAttr.TargetPosX_Float) && skillUnit.Position.y == skillUnit.GetFloat(GameUnit.eBulletAttr.TargetPosY_Float))
    //     {
    //         return;
    //     }

    //     Vector2 newPosition = CalculateQuadraticBezierPoint(progress, startpos, controlPoint, endpos);
    //     skillUnit.Position = newPosition;
    // }
    // Vector2 CalculateQuadraticBezierPoint(float t, Vector2 p0, Vector2 p1, Vector2 p2)
    // {
    //     float u = 1 - t;
    //     float tt = t * t;
    //     float uu = u * u;
    //     Vector2 p = uu * p0;
    //     p += 2 * u * t * p1;
    //     p += tt * p2;
    //     return p;
    // }


    void StartTrackMoveCharge(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)//跟踪移动技能开始蓄力
    {
        if (!(obj is IUnit skillUnit)) return;
        if (skillUnit == null || skillUnit.gameObject == null)
        {
            XApi.TimerApi.StopTimer(StartTrackMoveCharge, obj);
            return;
        }
        if (state == XApi.ITimer.eState.END)//&& murder == false)
        {
            TrackMoveContext moveContext = context as TrackMoveContext;
            Int64 moveId = moveContext.SkillId;
            _ShowFXManager.DeletSkillFX(skillUnit.Guid);//清理特效
            _ShowFXManager.ShowSkillMoveFX(skillUnit.Guid, moveId);
            XApi.TimerApi.StartTimer(TurnTrackMove, skillUnit, moveContext, 0, 0, (int)AutoConfig.Move.Get(moveId)._UpdateInterval);
            skillUnit.RegistUpdate(UpdateTrackMove);//注册位移事件update
        }
    }
    void UpdateTrackMove(IUnit skillUnit)
    {
        if (skillUnit == null || skillUnit.gameObject == null)
        {
            return;
        }

        UInt64 targetGuid = skillUnit.GetUInt64(GameUnit.eEntityAttr.MoveSkillTargetGuid_UInt64);
        IUnit target = GameUnitApi.QueryUnit(targetGuid);
        if (target == null)
        {
            XApi.Debug($"追踪目标不存在");
            return;
        }
        Vector2 currentPos = skillUnit.Position;
        Vector2 targetPos = target.Position;
        Vector2 directionToTarget = (targetPos - currentPos).normalized;
        float targetAngle = Mathf.Atan2(directionToTarget.y, directionToTarget.x) * Mathf.Rad2Deg;
        skillUnit.Angle = targetAngle;

        _SpineAnimationController.FlipCharacterToDirection(skillUnit, skillUnit.Angle);

        float speed = skillUnit.GetFloat(GameUnit.eEntityAttr.MoveSpeed_Float);
        float angleRad = skillUnit.Angle * Mathf.Deg2Rad;//角度转弧度
        Vector2 direction = new Vector2(Mathf.Cos(angleRad), Mathf.Sin(angleRad));//方向
        Vector2 newPos = currentPos + direction * speed * Time.deltaTime;//新的位置

        Vector2 hitPos = Vector2.zero;
        float distance = Vector2.Distance(skillUnit.Position, newPos);
        if (_GameScene.GetCurrentTilemap().CanMove(newPos) == false)
        {
            skillUnit.CancelRegistUpdate(UpdateTrackMove);
            XApi.TimerApi.StopTimer(TurnTrackMove, skillUnit);
            _ShowFXManager.DeletSkillFX(skillUnit.Guid);//清理特效
            return;
        }

        skillUnit.Position = newPos;
        Debug.Log($"TurnTrackMove called for unit GUID: {skillUnit.Guid} at position: {newPos}");
    }

    void TurnTrackMove(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
    {
        if (!(obj is IUnit skillUnit)) return;
        if (skillUnit == null || skillUnit.gameObject == null)
        {
            XApi.TimerApi.StopTimer(TurnTrackMove, obj);
            return;
        }
        TrackMoveContext moveContext = context as TrackMoveContext;
        if (moveContext == null) return;
        UInt64 targetGuid = moveContext.TargetGuid;//目标GUID
        Int64 skillId = moveContext.SkillId;//技能ID

        float lateangle = skillUnit.Angle;
        Vector2 currentPos = skillUnit.Position;//当前位置
        IUnit target = GameUnitApi.QueryUnit(targetGuid);//目标
        if (target == null)
        {
            XApi.Debug($"追踪目标不存在");
            return;
        }
        Vector2 targetPos = target.Position;
        // 当前方向和目标方向
        Vector2 currentDir = new Vector2(Mathf.Cos(skillUnit.Angle * Mathf.Deg2Rad), Mathf.Sin(skillUnit.Angle * Mathf.Deg2Rad));//当前方向
        Vector2 targetDir = (targetPos - currentPos).normalized;//目标方向
                                                                // 需要转动的角度
        float targetAngle = Mathf.Atan2(targetDir.y, targetDir.x) * Mathf.Rad2Deg;

        // 计算角度差并规范到 [-180, 180] 范围内
        float angleDiff = targetAngle - skillUnit.Angle;
        angleDiff = Mathf.DeltaAngle(skillUnit.Angle, targetAngle);

        float maxTurnAngle = AutoConfig.Move.Get(skillId)._MaxTurnAngle;

        // 如果角度差大于最大转向角度，则限制转向角度
        if (Mathf.Abs(angleDiff) > maxTurnAngle)
        {
            angleDiff = Mathf.Sign(angleDiff) * maxTurnAngle;
        }

        // 更新角度
        skillUnit.Angle += angleDiff;

    }

    void StopMoveSkill(IUnit skillUnit)
    {
        UInt64 guid = skillUnit.Guid;
        //销毁特效
        _ShowFXManager.DeletSkillFX(guid);
    }

    void OnMoveSkillEnter(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        GameUnitApi.IUnit unit = GameUnitApi.QueryUnit(guid);
        if (null == unit)
        {
            XApi.Error($"Where is unit {guid}");
            throw new Exception($"Where is unit {guid}");
        }

    }

    void OnMoveSkillExit(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        GameUnitApi.IUnit unit = GameUnitApi.QueryUnit(guid);
        if (null == unit)
        {
            XApi.Error($"Where is unit {guid}");
            throw new Exception($"Where is unit {guid}");
        }
        StopMoveSkill(unit);
    }

    bool OnExitMoveSkillJudge(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        IUnit unit = GameUnitApi.QueryUnit(guid);
        if (null == unit)
        {
            return false;
        }
        int shiftref = unit.GetInt32(eEntityAttr.ShiftRef_Int32);

        return shiftref == 0;
    }

    void OnSyncRelease(IUnit unit)
    {
        XApi.TimerApi.StopTimer(StartLineMoveCharge, unit);
        //XApi.TimerApi.StopTimer(StartParabolaMoveCharge, skillUnit);
        XApi.TimerApi.StopTimer(StartTrackMoveCharge, unit);
        XApi.TimerApi.StopTimer(TurnTrackMove, unit);
        XApi.TimerApi.StopTimer(StopLineMove, unit);
    }


    void LineMoveCheck(IUnit unit)
    {
        Debug.LogWarning($"LineMoveCheck called for unit GUID: {unit.Guid} at position: {unit.Position}");

        Vector2 startpos = unit.Position;//起点
        float speed = unit.GetFloat(GameUnit.eBulletAttr.MoveSpeed_Float);//初始速度
        float addspeed = AutoConfig.Move.Get(unit.GetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64))._AddSpeed;//加速度
        float angle = unit.GetFloat(GameUnit.eEntityAttr.SkillAngle_Float);//角度
        Vector2 direction = new Vector2(Mathf.Cos(angle * Mathf.Deg2Rad), Mathf.Sin(angle * Mathf.Deg2Rad));//方向
        Int64 moveId = unit.GetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64);
        AutoConfig.Move moveConfig = AutoConfig.Move.Get(moveId);
        Int64 movetimeMs = moveConfig._Time;//移动时间

        float movetimeSec = movetimeMs / 1000.0f;//移动时间（秒）

        float theoreticalDistance = speed * movetimeSec + 0.5f * addspeed * movetimeSec * movetimeSec;//理论距离

        Debug.Log($"移动参数 - 起点: {startpos}, 角度: {angle}, 方向: {direction}, 速度: {speed}, 加速度: {addspeed}, 时间: {movetimeMs}ms ({movetimeSec}s), 理论距离: {theoreticalDistance}");

        Vector2 hitPos = Vector2.zero;//预计碰撞位置
        bool hasHit = _GameScene.GetCurrentTilemap().Raycast(startpos, angle, theoreticalDistance, ref hitPos);
        float distanceToHit = Vector2.Distance(startpos, hitPos);

        Debug.Log($"射线检测结果 - 是否碰撞: {hasHit}, 碰撞点: {hitPos}, 到碰撞点距离: {distanceToHit}");

        float arrivalTimeSec = movetimeSec;

        if (hasHit && distanceToHit < theoreticalDistance - 0.01f)
        {
            if (addspeed == 0f)
            {
                // 匀速运动
                if (speed > 0)
                {
                    arrivalTimeSec = distanceToHit / speed;
                }
            }
            else
            {
                float discriminant = speed * speed + 2 * addspeed * distanceToHit;
                if (discriminant >= 0)
                {
                    float sqrtDiscriminant = Mathf.Sqrt(discriminant);
                    float t1 = (-speed + sqrtDiscriminant) / addspeed;
                    float t2 = (-speed - sqrtDiscriminant) / addspeed;
                    if (t1 >= 0 && t2 >= 0)
                    {
                        arrivalTimeSec = Mathf.Min(t1, t2);
                    }
                    else if (t1 >= 0)
                    {
                        arrivalTimeSec = t1;
                    }
                    else if (t2 >= 0)
                    {
                        arrivalTimeSec = t2;
                    }
                }
            }
            unit.SetVector2(GameUnit.eEntityAttr.SkillTargetPos_Vector2, hitPos);
            Debug.Log($"检测到碰撞，设置目标点: {hitPos}，调整到达时间: {arrivalTimeSec}s");
        }
        else
        {
            unit.SetVector2(GameUnit.eEntityAttr.SkillTargetPos_Vector2, Vector2.zero);
            Debug.Log("未检测到有效碰撞或碰撞点超出范围，清除目标点，使用默认飞行时间");
        }

        int arrivalTimeMs = Mathf.RoundToInt(arrivalTimeSec * 1000);

        XApi.TimerApi.StartTimer(StopLineMove, unit, null, arrivalTimeMs, 1, 0);
        //Debug.LogError($"开始移动 {arrivalTimeSec}s ({arrivalTimeMs}ms)，目的坐标：{hitPos}，起点坐标：{startpos}，理论距离：{theoreticalDistance}，实际距离：{distanceToHit}");
        return;
    }
    void StopLineMove(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
    {
        if (!(obj is IUnit skillUnit))
            return;

        skillUnit.CancelRegistUpdate(UpdateLineMove);
        skillUnit.CancelRegistUpdate(UpdateTrackMove);
        XApi.TimerApi.StopTimer(StopLineMove, skillUnit);
        XApi.TimerApi.StopTimer(StartLineMoveCharge, skillUnit);
        XApi.TimerApi.StopTimer(StartTrackMoveCharge, skillUnit);
        XApi.TimerApi.StopTimer(TurnTrackMove, skillUnit);

        Int64 moveId = skillUnit.GetInt64(GameUnit.eEntityAttr.MoveSkillConfigID_Int64);
        if (moveId == 0)
        {
            XApi.Debug($"moveId为0");
            return;
        }
        StopMoveSkill(skillUnit);
    }
}



