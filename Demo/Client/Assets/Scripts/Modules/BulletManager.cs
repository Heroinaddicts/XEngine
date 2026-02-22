using System;
using AutoConfig;
using GameUnit;
using Spine;
using Spine.Unity;
using UnityEngine;
using XEngine;
using XGame;
using XUtils;
using static GameUnitApi;

public interface IBulletManager : IModule
{

}
class BulletManager : IBulletManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IBulletManager, BulletManager>();
    }
    IGameObjectSync _GameObjectSync;
    IShowFXManager _ShowFXManager;
    IClient _Client;
    IGameSceneManager _GameSceneManager;

    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();
        _GameObjectSync = Launcher.GetModule<IGameObjectSync>();
        _Client = Launcher.GetModule<IClient>();
        _ShowFXManager = Launcher.GetModule<IShowFXManager>();
        _Client.RegistProtobuf<oBulletReleasePush>(eServerID.BulletReleasePush, OnBulletReleasePush);

        _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncCreate, (IUnit goUnit) =>
        {
            if (goUnit.Type == GameUnit.eUnitType.Bullet)
            {
                OnBulletCreate(goUnit);
                if (AutoConfig.Bullet.Get(goUnit.ConfigID)._ChangeZ != 0)
                {
                    goUnit.RegistUpdate(ChangebulletZ);
                }
            }
        });

        _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncRelease, (IUnit goUnit) =>
        {
            if (goUnit.Type == GameUnit.eUnitType.Bullet)
            {
                OnBulletRelease(goUnit);
            }
        });
        return true;
    }

    private void ChangebulletZ(IUnit unit)
    {
        var bulletConfig = AutoConfig.Bullet.Get(unit.ConfigID);
        Int64 bulletZ = bulletConfig._ChangeZ;
        if (bulletZ != 0 && unit.transform.position.z != bulletZ)
        {
            unit.transform.position = new Vector3(unit.transform.position.x, unit.transform.position.y, bulletZ);
        }
    }

    public void Release()
    {

    }

    // ... existing code ...
    void OnBulletCreate(IUnit bulletUnit)
    {
        var bulletConfig = AutoConfig.Bullet.Get(bulletUnit.ConfigID);

        // Int64 bulletZ = AutoConfig.Bullet.Get(bulletUnit.ConfigID)._ChangeZ;
        // Debug.LogError($"单位{bulletUnit.Guid} 施法ID{bulletUnit.ConfigID},Z轴调整{bulletZ}");
        // if (bulletZ != 0)
        // {
        //     bulletUnit.transform.position = new Vector3(bulletUnit.transform.position.x, bulletUnit.transform.position.y, bulletZ);
        //     Debug.LogError($"Z轴调整{bulletZ},调整后坐标{bulletUnit.transform.position}");
        // }

        if (bulletConfig != null)
        {
#if UNITY_EDITOR
            if (bulletConfig._Ballistics == AutoConfig.AutoEnum.eBallistics.Line)
            {
                var visualization = bulletUnit.gameObject.GetComponent<BulletVisualization>();
                if (visualization == null)
                {
                    visualization = bulletUnit.gameObject.AddComponent<BulletVisualization>();
                }
                visualization.bulletConfigId = (int)bulletUnit.ConfigID;
            }
#endif //UNITY_EDITOR
            switch (bulletConfig._Ballistics)
            {

                case AutoConfig.AutoEnum.eBallistics.Line://直线
                                                          //Debug.LogWarning("直线起点坐标" + bulletUnit.transform.position);
                    if (AutoConfig.Bullet.Get(bulletUnit.ConfigID)._IsMultiple == true)
                    {
                        PlayFormationAnimation(bulletUnit);
                        //Debug.LogError($"生成坐标{bulletUnit.transform.position}");
                    }
                    else
                    {
                        bulletUnit.RegistUpdate(UpdateLineBullet);
                        //Debug.LogError($"直线子弹生成坐标{bulletUnit.transform.position}");
                    }

                    break;
                case AutoConfig.AutoEnum.eBallistics.Parabolic://抛物线
                                                               // Debug.LogWarning($"抛物线子弹起点坐标{bulletUnit.Position}，目标坐标X{bulletUnit.GetFloat(GameUnit.eBulletAttr.TargetPosX_Float)}，目标坐标Y{bulletUnit.GetFloat(GameUnit.eBulletAttr.TargetPosY_Float)}");
                    bulletUnit.RegistUpdate(UpdateParabolicBullet);
                    break;
                case AutoConfig.AutoEnum.eBallistics.Range://范围
                    bulletUnit.RegistUpdate(UpdateRangeBullet);
                    break;
                case AutoConfig.AutoEnum.eBallistics.Track://追踪
                    bulletUnit.RegistUpdate(UpdateTrackBullet);
                    XApi.TimerApi.StartTimer(TurnTrackBullet, bulletUnit, null, 0, 0, (int)AutoConfig.Bullet.Get(bulletUnit.ConfigID)._UpdateInterval);
                    break;
            }
        }
    }
    // ... existing code ...
    void OnBulletRelease(IUnit bulletUnit)
    {
        bulletUnit.CancelRegistUpdate(UpdateLineBullet);
        bulletUnit.CancelRegistUpdate(UpdateParabolicBullet);
        bulletUnit.CancelRegistUpdate(UpdateRangeBullet);
        bulletUnit.CancelRegistUpdate(UpdateTrackBullet);
        XApi.TimerApi.StopTimer(TurnTrackBullet, bulletUnit);

        var bulletConfig = AutoConfig.Bullet.Get(bulletUnit.ConfigID);
        if (bulletConfig != null)
        {
            switch (bulletConfig._Ballistics)
            {
                case AutoConfig.AutoEnum.eBallistics.Line://直线
                                                          // Debug.LogWarning("直线最终坐标" + bulletUnit.Position);

                    DestoryBullet(bulletUnit);
                    break;
                case AutoConfig.AutoEnum.eBallistics.Parabolic://抛物线
                    //Debug.LogWarning($"抛物线子弹{bulletUnit.ConfigID}到达终点" + bulletUnit.Position);
                    DestoryBullet(bulletUnit);
                    break;
                case AutoConfig.AutoEnum.eBallistics.Range://范围
                    DestoryBullet(bulletUnit);
                    break;
                case AutoConfig.AutoEnum.eBallistics.Track://追踪
                    // Debug.LogWarning($"追踪子弹{bulletUnit.ConfigID}到达终点" + bulletUnit.Position);
                    DestoryBullet(bulletUnit);
                    break;
            }
        }
    }

    void UpdateLineBullet(IUnit bulletUnit)
    {
        AutoConfig.Bullet config = AutoConfig.Bullet.Get(bulletUnit.ConfigID);
        if (null != config)
        {
            Vector2 startpos = bulletUnit.Position;//起点
            float speed = bulletUnit.GetFloat(GameUnit.eBulletAttr.Speed_Float);//基本速度
            float addspeed = AutoConfig.Bullet.Get(bulletUnit.ConfigID)._AddSpeed;//加速
            float angle = bulletUnit.Angle;//角度
                                           // Debug.LogError("angle:" + angle);

            speed = speed + addspeed * Time.deltaTime;//速度
            bulletUnit.SetFloat(GameUnit.eBulletAttr.Speed_Float, speed);
            Vector2 direction = new Vector2(Mathf.Cos(angle * Mathf.Deg2Rad), Mathf.Sin(angle * Mathf.Deg2Rad));//方向
            Vector2 pos = bulletUnit.Position + direction * speed * Time.deltaTime;//位置
            if (_GameSceneManager.GetCurrentTilemap().CanMove(pos, config._CanMoveLayer))
            {
                bulletUnit.Position = pos;
                SkeletonAnimation skeletonAnimation = bulletUnit.gameObject.GetComponent<SkeletonAnimation>();
                bulletUnit.transform.rotation = Quaternion.Euler(0, 0, angle);
                return;
            }
        }

        bulletUnit.gameObject.SetActive(false);
    }


    void UpdateParabolicBullet(IUnit bulletUnit)
    {
        Vector2 startpos = bulletUnit.GetVector2(GameUnit.eBulletAttr.StartPos_Vector2);//起点
        Vector2 endpos = new Vector2(bulletUnit.GetFloat(GameUnit.eBulletAttr.TargetPosX_Float), bulletUnit.GetFloat(GameUnit.eBulletAttr.TargetPosY_Float));//终点
        float lifetime = AutoConfig.Bullet.Get(bulletUnit.ConfigID)._LifeCycle / 1000f;//生命周期
        float bulletMutiple = AutoConfig.Bullet.Get(bulletUnit.ConfigID)._BulletMutiple;//放大倍率
        if (lifetime <= 0)
        {
            bulletUnit.Position = endpos;
            return;
        }
        float directDistance = Vector2.Distance(endpos, startpos);//直接距离
        float deltaTime = bulletUnit.GetFloat(GameUnit.eBulletAttr.DeltaTime_Float);//已经使用的时间

        float progress = deltaTime / lifetime;
        progress = Mathf.Clamp01(progress);//获得时间比例0-1

        deltaTime = deltaTime + Time.deltaTime;//增加时间
        bulletUnit.SetFloat(GameUnit.eBulletAttr.DeltaTime_Float, deltaTime); //存储已用时间

        Vector2 direction = (endpos - startpos).normalized;//方向
        Vector2 midPoint = (startpos + endpos) * 0.5f;//中点
        // float heightOffset = directDistance * AutoConfig.Bullet.Get(bulletUnit.ConfigID)._HeightParameter;//高度偏移
        // Vector2 perpendicular = new Vector2(-direction.y, direction.x);//垂直于起点到终点的方向
        // Vector2 controlPoint = midPoint + perpendicular * heightOffset * 0.5f;//控制点左偏移
        float heightOffset = directDistance * AutoConfig.Bullet.Get(bulletUnit.ConfigID)._HeightParameter;//高度偏移
        float bulletAngle = bulletUnit.Angle;
        Vector2 bulletDirection = new Vector2(Mathf.Cos(bulletAngle * Mathf.Deg2Rad), Mathf.Sin(bulletAngle * Mathf.Deg2Rad));
        Vector2 perpendicular = new Vector2(-bulletDirection.y, bulletDirection.x);
        Vector2 controlPoint = midPoint + perpendicular * heightOffset * 0.5f;

        if (bulletMutiple > 0)
        {
            float scaleProgress = progress * 2;
            float scale = 1.0f;

            if (scaleProgress <= 1.0f)
            {
                scale = 1.0f + (bulletMutiple - 1.0f) * scaleProgress;
            }
            else
            {
                scale = bulletMutiple - (bulletMutiple - 1.0f) * (scaleProgress - 1.0f);
            }

            // 应用缩放
            bulletUnit.transform.localScale = new Vector2(scale, scale);
        }


        if (progress >= 1.0f)
        {
            //bulletUnit.Position = endpos;
            return;
        }
        if (bulletUnit.Position.x == bulletUnit.GetFloat(GameUnit.eBulletAttr.TargetPosX_Float) && bulletUnit.Position.y == bulletUnit.GetFloat(GameUnit.eBulletAttr.TargetPosY_Float))
        {
            return;
        }

        Vector2 newPosition = CalculateQuadraticBezierPoint(progress, startpos, controlPoint, endpos);
        bulletUnit.Position = newPosition;
    }

    Vector2 CalculateQuadraticBezierPoint(float t, Vector2 p0, Vector2 p1, Vector2 p2)
    {
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;

        Vector2 p = uu * p0;
        p += 2 * u * t * p1;
        p += tt * p2;

        return p;
    }

    void UpdateRangeBullet(IUnit bulletUnit)
    {

    }

    void UpdateTrackBullet(IUnit bulletUnit)
    {
        float speed = bulletUnit.GetFloat(GameUnit.eBulletAttr.Speed_Float);
        Vector2 currentPos = bulletUnit.Position;//子弹当前位置

        float angleRad = bulletUnit.Angle * Mathf.Deg2Rad;//角度转弧度
        Vector2 direction = new Vector2(Mathf.Cos(angleRad), Mathf.Sin(angleRad));//方向
        Vector2 newPos = currentPos + direction * speed * Time.deltaTime;//新的位置
        bulletUnit.Position = newPos;
    }

    void TurnTrackBullet(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
    {
        if (!(obj is IUnit bulletUnit)) return;
        if (bulletUnit == null || bulletUnit.gameObject == null)
        {
            XApi.TimerApi.StopTimer(TurnTrackBullet, obj);
            return;
        }
        float lateangle = bulletUnit.Angle;
        Vector2 currentPos = bulletUnit.Position;//子弹当前位置
        IUnit target = GameUnitApi.QueryUnit(bulletUnit.GetUInt64(GameUnit.eBulletAttr.TargetGuid_UInt64));//目标
        if (target == null)
        {
            XApi.Debug($"追踪目标不存在");
            return;
        }
        Vector2 targetPos = target.Position;
        // 当前方向和目标方向
        Vector2 currentDir = new Vector2(Mathf.Cos(bulletUnit.Angle * Mathf.Deg2Rad), Mathf.Sin(bulletUnit.Angle * Mathf.Deg2Rad));//当前方向
        Vector2 targetDir = (targetPos - currentPos).normalized;//目标方向

        // 需要转动的角度
        float angleToTarget = Mathf.Atan2(targetDir.y, targetDir.x) - Mathf.Atan2(currentDir.y, currentDir.x);//要转多少
        angleToTarget = Mathf.Atan2(Mathf.Sin(angleToTarget), Mathf.Cos(angleToTarget));//归一化

        float angleToTargetDegree = angleToTarget * Mathf.Rad2Deg;//转为角度

        if (currentDir != targetDir) // 方向不同
        {
            if (Mathf.Abs(angleToTargetDegree) > AutoConfig.Bullet.Get(bulletUnit.ConfigID)._MaxTurnAngle)
            {
                angleToTargetDegree = Mathf.Sign(angleToTargetDegree) * AutoConfig.Bullet.Get(bulletUnit.ConfigID)._MaxTurnAngle;
            }


            bulletUnit.Angle += angleToTargetDegree;//旋转更新角度
        }

        // float angleRad = bulletUnit.Angle * Mathf.Deg2Rad;//角度转弧度
        // Vector2 direction = new Vector2(Mathf.Cos(angleRad), Mathf.Sin(angleRad));//方向
        // Vector2 newPos = currentPos + direction * speed * ((int)AutoConfig.Bullet.Get(bulletUnit.ConfigID)._UpdateInterval / 1000f);//新的位置
        // bulletUnit.Position = newPos;
        // while (bulletUnit.Angle >= 360f)
        //     bulletUnit.Angle -= 360f;
        // while (bulletUnit.Angle < 0f)
        //     bulletUnit.Angle += 360f;
        //Debug.LogWarning($"转之前的角度{lateangle:F8}，当前角度{bulletUnit.Angle:F8}，位置X={bulletUnit.Position.x:F8}, Y={bulletUnit.Position.y:F8}，目标角度{target.Angle:F8}，目标位置X={targetPos.x:F8}, Y={targetPos.y:F8}");
    }

    void DestoryBullet(IUnit bulletUnit)
    {
        GameObject.Destroy(bulletUnit.gameObject);
    }

    void OnBulletReleasePush(oBulletReleasePush push)
    {
        IUnit bulletUnit = GameUnitApi.QueryUnit(push.BulletGuid);
        //Debug.LogWarning($"子弹{bulletUnit.ConfigID}爆炸在{bulletUnit.Position}");
        if (AutoConfig.Bullet.Get(bulletUnit.ConfigID)._EndPrefab != 0)
        {
            GameObject bulletFX = Resources.Load<GameObject>(AutoConfig.SpecialEffects.Get(AutoConfig.Bullet.Get(bulletUnit.ConfigID)._EndPrefab)._Prefab);
            bulletFX.GetComponentInChildren<Renderer>().sortingOrder = 10;
            GameObject Fxitem = GameObject.Instantiate(bulletFX, new Vector3(bulletUnit.Position.x, bulletUnit.Position.y, 0), Quaternion.identity);
            XApi.TimerApi.StartTimer(OnFXTimeout, Fxitem, 0, (int)AutoConfig.Bullet.Get(bulletUnit.ConfigID)._DestroyTime, 1, 1, "");
        }
        // if (AutoConfig.Bullet.Get(bulletUnit.ConfigID)._IsHitLoop == true)
        // {
        //     SkeletonAnimation skeletonAnimation = bulletUnit.gameObject.GetComponent<SkeletonAnimation>();
        //     if (skeletonAnimation == null)
        //     {
        //         Debug.LogWarning("缺少Spine动画组件");
        //         return;
        //     }
        //     skeletonAnimation.state.SetAnimation(0, AutoConfig.Bullet.Get(bulletUnit.ConfigID)._LaterLoop, false);
        //     Debug.LogWarning($"播放{AutoConfig.Bullet.Get(bulletUnit.ConfigID)._LaterLoop}");
        // }
    }

    void OnFXTimeout(in XApi.ITimer.eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)
    {
        XApi.TimerApi.StopTimer(OnFXTimeout, obj);
        GameObject.Destroy(obj as GameObject);
    }


    void PlayFormationAnimation(IUnit bulletUnit)
    {
        try
        {
            SkeletonAnimation skeletonAnimation = bulletUnit.gameObject.GetComponent<SkeletonAnimation>();

            if (skeletonAnimation == null)
            {
                Debug.LogWarning("缺少Spine动画组件");
                return;
            }

            var bulletConfig = AutoConfig.Bullet.Get(bulletUnit.ConfigID);
            if (bulletConfig == null)
            {
                Debug.LogWarning("缺少子弹配置: " + bulletUnit.ConfigID);
                return;
            }

            string previousLoopAnim = bulletConfig._PreviousLoop;
            string whereLoopAnim = bulletConfig._WhereLoop;

            if (string.IsNullOrEmpty(previousLoopAnim))
            {
                Debug.LogWarning("第一段动画名称为空或未设置: " + bulletUnit.ConfigID);
                return;
            }

            // 检查动画是否存在
            if (skeletonAnimation.state.Data.SkeletonData.FindAnimation(previousLoopAnim) == null)
            {
                Debug.LogWarning("未找到第一段动画: " + previousLoopAnim);
                return;
            }

            // 播放第一段动画
            TrackEntry entry1 = skeletonAnimation.state.SetAnimation(0, previousLoopAnim, false);
            Int64 bulletZ = AutoConfig.Bullet.Get(bulletUnit.ConfigID)._ChangeZ;

            entry1.Complete += (trackEntry) =>
            {
                try
                {
                    // 检查第二段动画名称
                    if (!string.IsNullOrEmpty(whereLoopAnim))
                    {
                        // 检查动画是否存在
                        if (skeletonAnimation.state.Data.SkeletonData.FindAnimation(whereLoopAnim) != null)
                        {
                            Int64 bulletZ = AutoConfig.Bullet.Get(bulletUnit.ConfigID)._ChangeZ;
                            skeletonAnimation.state.SetAnimation(0, whereLoopAnim, true);
                            if (AutoConfig.Bullet.Get(bulletUnit.ConfigID)._IsHitLoop == true)
                            {
                                //Debug.LogError("播放技能二段，进入循环");
                                return;

                                // UInt64 hitUnitGuid = bulletUnit.GetUInt64(eBulletAttr.HitUnitGuid_UInt64);
                                // if (hitUnitGuid != 0)
                                //{
                                //     return;
                                // }
                                // else
                                // {
                                //     XApi.TimerApi.StartTimer(OnAnimationComplete, bulletUnit, null, (int)AutoConfig.Bullet.Get(bulletUnit.ConfigID)._LoopTime, 1, 0);
                                // }
                            }
                            else
                            {
                                XApi.TimerApi.StartTimer(OnAnimationComplete, bulletUnit, null, (int)AutoConfig.Bullet.Get(bulletUnit.ConfigID)._LoopTime, 1, 0);
                            }
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
        catch (Exception ex)
        {
            XApi.Error("PlayFormationAnimation执行异常: " + ex.Message + "\nStackTrace: " + ex.StackTrace);
        }
    }

    void OnAnimationComplete(in XApi.ITimer.eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)//播放最终动画
    {
        XApi.TimerApi.StopTimer(OnAnimationComplete, obj);
        IUnit bulletUnit = obj as IUnit;
        if (bulletUnit == null || bulletUnit.gameObject == null)
        {
            Debug.LogWarning("子弹单元或其游戏对象已被销毁，跳过动画播放");
            return;
        }
        SkeletonAnimation skeletonAnimation = bulletUnit.gameObject.GetComponent<SkeletonAnimation>();
        if (skeletonAnimation == null)
        {
            Debug.LogWarning("缺少Spine动画组件");
            return;
        }
        skeletonAnimation.state.SetAnimation(0, AutoConfig.Bullet.Get(bulletUnit.ConfigID)._LaterLoop, false);

        Debug.LogWarning($"播放{AutoConfig.Bullet.Get(bulletUnit.ConfigID)._LaterLoop}");
    }
}

