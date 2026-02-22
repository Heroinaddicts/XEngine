using AutoConfig;
using DG.Tweening;
using GameUnit;
using System;
using System.Collections.Generic;
using System.Linq;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using static GameUnitApi;

public interface IDamageManager : IModule
{
    public IUnit GetLastDamageUnit();//获取最后伤害单位中优先级最高的Unit
}

class DamageManager : IDamageManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IDamageManager, DamageManager>();
    }

    IClient _Client;
    IGameSceneManager _GameSceneManager;
    ICharacterManager _CharacterManager;
    IShowArtNumberManager _ShowArtNumberManager;
    IGameObjectSync _GameObjectSync;
    IAudioManager _AudioManager;
    GameObject _CanvasUILab;
    IUnit _LastTargetUnit;
    private Dictionary<UInt64, IUnit> _LastDamageUnit = new Dictionary<UInt64, IUnit>();
    private Dictionary<Spine.Unity.SkeletonAnimation, Color> _originalColors = new Dictionary<Spine.Unity.SkeletonAnimation, Color>();


    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();
        _GameObjectSync = Launcher.GetModule<IGameObjectSync>();
        _AudioManager = Launcher.GetModule<IAudioManager>();
        _ShowArtNumberManager = Launcher.GetModule<IShowArtNumberManager>();
        _Client.RegistProtobuf<oSkillDamagePush>(eServerID.SkillDamagePush, OnSkillDamageChange);
        _GameSceneManager.RegistEvent(IGameSceneManager.eEvent.GameMapLoaded, OnSceneChange);
        _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncRelease, (IUnit goUnit) =>
          {
              DeletUnitInDictionary(goUnit.Guid);
              //   if (goUnit.Type == GameUnit.eUnitType.Monster || goUnit.Type == GameUnit.eUnitType.Character || goUnit.Type == GameUnit.eUnitType.Boss)
              //   {
              //       DeletUnitInDictionary(goUnit.Guid);
              //   }
              if (goUnit.Type == GameUnit.eUnitType.Boss)
              {
                  if (goUnit != null && goUnit.gameObject != null)
                  {
                      var skeletonAnimations = goUnit.gameObject.GetComponentsInChildren<Spine.Unity.SkeletonAnimation>();
                      foreach (var skeletonAnimation in skeletonAnimations)
                      {
                          if (skeletonAnimation != null)
                          {
                              _originalColors.Remove(skeletonAnimation);
                          }
                      }
                  }
              }
          });
           _GameObjectSync.RegistEvent(IGameObjectSync.eEvents.SyncCreate, (IUnit goUnit) =>
 {
     if (goUnit.Type == GameUnit.eUnitType.Boss)
     {
         goUnit.RegistFloatAttributeChange(eEntityAttr.Hp_Float, eUnitAttributeEvent.Changed,
         (in eUnitAttributeEvent ev, in IUnit unit, in int index, in float oldValue, in float newValue) =>
         {
             try
             {
                 if (goUnit == null || goUnit.Guid == 0)
                 {
                     XApi.Error("Boss unit is null or invalid, skipping HP slider update");
                     return true;
                 }

                 Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
                 if (canvas == null)
                 {
                     XApi.Error("Canvas not found, skipping HP slider update");
                     return true;
                 } 

                 Transform bossTopUILabTransform = canvas.transform.Find("BossTopUILab");
                 if (bossTopUILabTransform == null)
                 {
                     XApi.Error("BossTopUILab not found, skipping HP slider update");
                     return true;
                 }

                 _CanvasUILab = bossTopUILabTransform.gameObject;

                 Transform bossTopUITransform = _CanvasUILab.transform.Find("BossTopUI");
                 if (bossTopUITransform == null)
                 {
                     XApi.Error("BossTopUI not found, skipping HP slider update");
                     return true;
                 }

                 GameObject targetTopUI = bossTopUITransform.gameObject;

                 Slider targetHpSlider = targetTopUI.transform.Find("Slider").GetComponent<Slider>();
                 if (targetHpSlider == null)
                 {
                     XApi.Error("Target HP Slider not found, skipping HP slider update");
                     return true;
                 }

                 Slider bgSlider = targetTopUI.transform.Find("BGSlider").GetComponent<Slider>(); // 背景缓降血条
                 if (bgSlider == null)
                 {
                     XApi.Error("Background HP Slider not found, skipping HP slider update");
                     return true;
                 }

                 targetHpSlider.value = goUnit.GetFloat(eEntityAttr.Hp_Float);
                 if (targetHpSlider.value <= 0)
                 {
                     targetHpSlider.value = 0;
                 }


                 if (bgSlider != null)
                 {
                     //DOTween.Kill(bgSlider);

                     bgSlider.value = newValue;
                     if (bgSlider.value < newValue)
                     {
                         bgSlider.value = newValue;
                     }

                     StartSliderTween(bgSlider, newValue, AutoConst.HpGradient.BossHpGradient);
                 }
                 return true;
             }
             catch (Exception ex)
             {
                 XApi.Error($"Exception in Boss HP change event handler for unit {goUnit?.Guid}: {ex.Message}\n{ex.StackTrace}");
                 return true; // 返回true但不更新UI，避免中断其他处理
             }
         });

         if (goUnit != null && goUnit.gameObject != null)
         {
             try
             {
                 var skeletonAnimations = goUnit.gameObject.GetComponentsInChildren<Spine.Unity.SkeletonAnimation>();
                 foreach (var skeletonAnimation in skeletonAnimations)
                 {
                     if (skeletonAnimation != null && skeletonAnimation.Skeleton != null)
                     {
                         var originalColor = new Color(skeletonAnimation.Skeleton.R, skeletonAnimation.Skeleton.G, skeletonAnimation.Skeleton.B, skeletonAnimation.Skeleton.A);
                         if (!_originalColors.ContainsKey(skeletonAnimation))
                         {
                             _originalColors[skeletonAnimation] = originalColor;
                         }
                     }
                 }
             }
             catch (Exception ex)
             {
                 XApi.Error($"Exception when processing skeleton animations for boss {goUnit.Guid}: {ex.Message}\n{ex.StackTrace}");
             }
         }
     }
 });

        return true;
    }

    private void StartSliderTween(Slider bgSlider, float newValue, float v)
    {
        //Debug.LogError($"开始缓降血条,目标值:{newValue},缓动时间:{v}");
        if (bgSlider != null)
        {
            float currentValue = bgSlider.value;

            DG.Tweening.Tween tween = DG.Tweening.DOTween.To(() => bgSlider.value, x => bgSlider.value = x, newValue, v);
            tween.SetId(bgSlider);
        }
    }

    private void OnSceneChange(UnityEngine.SceneManagement.Scene scene)
    {
        // 清空最后伤害单位字典
        _LastDamageUnit.Clear();
        _LastTargetUnit = null;
    }

    public void Release()
    {

    }

    void OnSkillDamageChange(oSkillDamagePush body)
    {
        UInt64 targetGuid = body.TargetGuid;
        IUnit targetUnit = GameUnitApi.QueryUnit(targetGuid);
        AutoConfig.Scene currentSceneConfig = _GameSceneManager.GetCurrentSceneConfig();
        IUnit mainCharacterUnit = _CharacterManager.GetMainCharacterUnit();
        float damageNum = body.DamageNum;
        if (currentSceneConfig._SceneType == AutoEnum.eSceneType.Public)
        {
            XApi.Debug($"目标单位{targetGuid}在公共场景,不处理伤害");
            Debug.LogError($"目标单位{targetGuid}在公共场景,不处理伤害");
            return;
        }
        if (body.CastGuid == mainCharacterUnit.Guid)
        {
            if (targetUnit != null)
            {
                // 更新或添加到字典中
                if (_LastDamageUnit.ContainsKey(targetGuid))
                {
                    _LastDamageUnit[targetGuid] = targetUnit;
                   // Debug.LogError($"目标单位{targetGuid}已存在,更新为{targetUnit.Guid}");
                }
                else
                {
                    _LastDamageUnit.Add(targetGuid, targetUnit);
                    //Debug.LogError($"目标单位{targetGuid}不存在,添加{targetUnit.Guid}");
                }
            }
        }


        if (body.IsCritical == true)//暴击判断
        {
            XApi.Debug($"目标单位{targetGuid}触发暴击");
            if (targetUnit != null && targetUnit.gameObject != null) // 添加检查
            {
                _ShowArtNumberManager.ShowNum(targetGuid, damageNum, "NumLab", body.CastGuid, true);
            }
        }
        else
        {
            //Debug.LogError($"目标单位{body.TargetGuid}受到伤害{body.DamageNum}");
            if (targetUnit != null && targetUnit.gameObject != null) // 添加检查
            {
                _ShowArtNumberManager.ShowNum(targetGuid, damageNum, "NumLab", body.CastGuid, false);
            }
        }
        if (targetUnit.Type == eUnitType.Monster)
        {
            //Debug.LogError($"目标单位{body.TargetGuid}受到伤害{body.DamageNum}");
            _CanvasUILab = GameObject.FindFirstObjectByType<Canvas>().transform.Find("TopUILab").gameObject;
            GameObject targetTopUI = _CanvasUILab.transform.Find("MonsterTopUI" + targetGuid).gameObject;
            Slider targetHpSlider = targetTopUI.transform.Find("HpBar").GetComponent<Slider>();
            //targetHpSlider.value -= damageNum;
            if (targetHpSlider.value <= 0)
            {
                targetHpSlider.value = 0;
            }

            //播放受伤影响
            _AudioManager.PlaySound(AutoConfig.Sound.Get(AutoConfig.Monster.Get(targetUnit.ConfigID)._SoundHurt)._AudioMaterial);
            Int64 hurtEffectID = AutoConfig.Monster.Get(targetUnit.ConfigID)._HurtEffect;
            if (hurtEffectID != 0)
            {
                string hurtEffectPath = AutoConfig.SpecialEffects.Get(hurtEffectID)._Prefab;
                if (hurtEffectPath != null)
                {
                    GameObject hurtEffect = Resources.Load<GameObject>(hurtEffectPath);
                    if (hurtEffect != null)
                    {
                        GameObject parent = targetUnit.transform.Find("BeAttackFx").gameObject;
                        GameObject hurtEffectGO = GameObject.Instantiate(hurtEffect, parent.transform);
                        hurtEffectGO.transform.localPosition = new Vector3(0, 0, hurtEffectGO.transform.localPosition.z);
                        var skeletonAnimation = hurtEffectGO.GetComponent<Spine.Unity.SkeletonAnimation>();
                        if (skeletonAnimation != null)
                        {
                            // 注册动画完成事件
                            skeletonAnimation.AnimationState.Complete += (trackEntry) =>
                            {
                                if(hurtEffectGO != null)
                                GameObject.Destroy(hurtEffectGO);
                            };
                        }
                    }


                }
            }
        }
        else if (targetUnit.Type == eUnitType.Character)
        {
            //Debug.LogError($"玩家单位{targetGuid}受到伤害{body.DamageNum}");
            _CanvasUILab = GameObject.FindFirstObjectByType<Canvas>().transform.Find("TopUILab").gameObject;
            GameObject targetTopUI = _CanvasUILab.transform.Find("CharacterTopUI" + targetGuid).gameObject;
            Slider targetHpSlider = targetTopUI.transform.Find("BattleTopUI/HpBar").GetComponent<Slider>();
            // targetHpSlider.value -= damageNum;
            if (targetHpSlider.value <= 0)
            {
                targetHpSlider.value = 0;
            }

            if (targetGuid == _CharacterManager.GetMainCharacterUnit().Guid)//如果是主角受伤，播放红光
            {
                Canvas canvas = GameObject.FindFirstObjectByType<Canvas>();
                if (canvas != null)
                {
                   Transform redlight = canvas.transform.Find("BeAttack");
                   if(redlight != null)
                   {
                       redlight.gameObject.SetActive(true);
                   }
                   XApi.TimerApi.StartTimer(OffRedLight,redlight,null,(int)AutoConst.BattleSetting.RedScreenTime,1,XApi.ITimer.Unlimited);
                }
            }
        }
        else if (targetUnit.Type == eUnitType.Boss)
        {
            //Debug.LogError($"目标单位{body.TargetGuid}受到伤害{body.DamageNum}");
            if (targetUnit != null && targetUnit.gameObject != null)
            {
                var skeletonAnimations = targetUnit.gameObject.GetComponentsInChildren<Spine.Unity.SkeletonAnimation>();
                _AudioManager.PlaySound(AutoConfig.Sound.Get(AutoConfig.MonsterBoss.Get(targetUnit.ConfigID)._SoundHurt)._AudioMaterial);
                foreach (var skeletonAnimation in skeletonAnimations)
                {
                    if (!_originalColors.ContainsKey(skeletonAnimation))
                    {
                        var initialColor = new Color(skeletonAnimation.Skeleton.R, skeletonAnimation.Skeleton.G, skeletonAnimation.Skeleton.B, skeletonAnimation.Skeleton.A);
                        _originalColors[skeletonAnimation] = initialColor;
                    }
                    DG.Tweening.DOTween.Kill(skeletonAnimation);

                    var originalColor = _originalColors[skeletonAnimation];

                    if (body.IsCritical)
                    {
                        // 暴击时闪红
                        var flashColor = new Color(1f, 0.3f, 0.3f, 0.93f);

                        var sequence = DG.Tweening.DOTween.Sequence();
                        sequence.Append(DG.Tweening.DOTween.To(() => new Color(skeletonAnimation.Skeleton.R, skeletonAnimation.Skeleton.G, skeletonAnimation.Skeleton.B, skeletonAnimation.Skeleton.A),
                                (Color c) =>
                                {
                                    skeletonAnimation.Skeleton.R = c.r;
                                    skeletonAnimation.Skeleton.G = c.g;
                                    skeletonAnimation.Skeleton.B = c.b;
                                    skeletonAnimation.Skeleton.A = c.a;
                                }, flashColor, 0.3f))
                               .Append(DG.Tweening.DOTween.To(() => new Color(skeletonAnimation.Skeleton.R, skeletonAnimation.Skeleton.G, skeletonAnimation.Skeleton.B, skeletonAnimation.Skeleton.A),
                                (Color c) =>
                                {
                                    skeletonAnimation.Skeleton.R = c.r;
                                    skeletonAnimation.Skeleton.G = c.g;
                                    skeletonAnimation.Skeleton.B = c.b;
                                    skeletonAnimation.Skeleton.A = c.a;
                                }, originalColor, 0.22f));
                        sequence.OnComplete(() =>
                        {

                            var finalColor = _originalColors[skeletonAnimation];
                            skeletonAnimation.Skeleton.R = finalColor.r;
                            skeletonAnimation.Skeleton.G = finalColor.g;
                            skeletonAnimation.Skeleton.B = finalColor.b;
                            skeletonAnimation.Skeleton.A = finalColor.a;
                        });
                        sequence.SetId(skeletonAnimation);
                    }
                    else
                    {
                        var whiteColor = new Color(0.65f, 0.65f, 0.65f, 0.93f); // 亮白色（超过1.0会更亮）
                        var sequence = DG.Tweening.DOTween.Sequence();
                        sequence.Append(DG.Tweening.DOTween.To(() => new Color(skeletonAnimation.Skeleton.R, skeletonAnimation.Skeleton.G, skeletonAnimation.Skeleton.B, skeletonAnimation.Skeleton.A),
                         (Color c) =>
                         {
                             skeletonAnimation.Skeleton.R = c.r;
                             skeletonAnimation.Skeleton.G = c.g;
                             skeletonAnimation.Skeleton.B = c.b;
                             skeletonAnimation.Skeleton.A = c.a;
                         }, whiteColor, 0.38f))
                        .Append(DG.Tweening.DOTween.To(() => new Color(skeletonAnimation.Skeleton.R, skeletonAnimation.Skeleton.G, skeletonAnimation.Skeleton.B, skeletonAnimation.Skeleton.A),
                         (Color c) =>
                         {
                             skeletonAnimation.Skeleton.R = c.r;
                             skeletonAnimation.Skeleton.G = c.g;
                             skeletonAnimation.Skeleton.B = c.b;
                             skeletonAnimation.Skeleton.A = c.a;
                         }, originalColor, 0.25f)); // 恢复原始颜色
                        sequence.OnComplete(() =>
                        {
                            // 确保动画完成后颜色恢复到原始状态
                            var finalColor = _originalColors[skeletonAnimation];
                            skeletonAnimation.Skeleton.R = finalColor.r;
                            skeletonAnimation.Skeleton.G = finalColor.g;
                            skeletonAnimation.Skeleton.B = finalColor.b;
                            skeletonAnimation.Skeleton.A = finalColor.a;
                        });
                        sequence.SetId(skeletonAnimation);
                    }
                }
            }
            Int64 hurtEffectID = AutoConfig.MonsterBoss.Get(targetUnit.ConfigID)._HurtEffect;
            if (hurtEffectID != 0)
            {
                string hurtEffectPath = AutoConfig.SpecialEffects.Get(hurtEffectID)._Prefab;
                if (hurtEffectPath != null)
                {
                    GameObject hurtEffect = Resources.Load<GameObject>(hurtEffectPath);
                    if (hurtEffect != null)
                    {
                        GameObject parent = targetUnit.transform.Find("BeAttackFx").gameObject;
                        GameObject hurtEffectGO = GameObject.Instantiate(hurtEffect, parent.transform);
                        hurtEffectGO.transform.localPosition = new Vector3(0, 0, hurtEffectGO.transform.localPosition.z);
                        var skeletonAnimation = hurtEffectGO.GetComponent<Spine.Unity.SkeletonAnimation>();
                        if (skeletonAnimation != null)
                        {
                            // 注册动画完成事件
                            skeletonAnimation.AnimationState.Complete += (trackEntry) =>
                            {
                                GameObject.Destroy(hurtEffectGO);
                            };
                        }
                    }


                }
            }

        }
    }

    private void OffRedLight(in XApi.ITimer.eState state, in int count, in long tick, in object obj, in object context, in bool murder)
    {
        XApi.TimerApi.StopTimer(OffRedLight, obj);
        Transform redlight = obj as Transform;
        if (redlight != null)
        {
            redlight.gameObject.SetActive(false);
        }
    }

    void DeletUnitInDictionary(UInt64 guid)//从最后伤害单位字典中删除Unit（脱离视野或者死了）
    {
        if (_LastDamageUnit.ContainsKey(guid))
        {
            _LastDamageUnit.Remove(guid);
        }
        _LastTargetUnit = GetHighestPriorityDamageUnit();
    }

    IUnit GetHighestPriorityDamageUnit()//获取最后伤害单位中优先级最高的Unit
    {
        if (_LastDamageUnit.Count == 0)
        {
            return null;
        }
        IUnit highestPriorityUnit = null;
        int minPriority = int.MaxValue;

        foreach (var pair in _LastDamageUnit)
        {
            if (pair.Value != null)
            {
                if(pair.Key == _CharacterManager.GetMainCharacterUnit().Guid)
                {
                    Debug.LogError($"目标单位{pair.Key}为主角色,跳过");
                    continue;
                }
                int priority = pair.Value.GetInt32(GameUnit.eEntityAttr.DamagePriority_Int32);
                if (priority < minPriority)
                {
                    minPriority = priority;
                    highestPriorityUnit = pair.Value;
                }
            }
        }
        return highestPriorityUnit;
    }

    List<IUnit> GetDamageUnitsSortedByPriority()//获取最后伤害单位列表并按优先级排序
    {
        var sortedUnits = new List<IUnit>();

        if (_LastDamageUnit.Count > 0)
        {
            // 使用 LINQ 按 DamagePriority_Int32 排序
            var sortedPairs = _LastDamageUnit
                .Where(pair => pair.Value != null) // 过滤掉空单位
                .OrderBy(pair => pair.Value.GetInt32(GameUnit.eEntityAttr.DamagePriority_Int32))
                .Select(pair => pair.Value);

            sortedUnits.AddRange(sortedPairs);
        }

        return sortedUnits;
    }

    public IUnit GetLastDamageUnit()//获取最后伤害单位中优先级最高的Unit
    {
        _LastTargetUnit = GetHighestPriorityDamageUnit();
        return _LastTargetUnit;
    }

}

