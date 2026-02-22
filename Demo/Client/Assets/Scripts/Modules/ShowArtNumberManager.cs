using System;
using AutoConfig;
using GameUnit;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using XUtils;
using static GameUnitApi;
using DG.Tweening;
using TMPro;
using System.Collections;
using System.Collections.Generic;
using static AutoConfig.AutoEnum;


public interface IShowArtNumberManager : IModule
{
    public void ShowNum(UInt64 Guid, float num, string parentName, UInt64 castGuid, bool isCritical);
}
public class ShowArtNumberManager : IShowArtNumberManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IShowArtNumberManager, ShowArtNumberManager>();
    }
    IClient _Client;
    ICharacterManager _CharacterManager;
    private GameObject _NumberPre;
    private Queue<GameObject> _NumberPool = new Queue<GameObject>();//数字对象池
    private Sprite[] _NormalNumberSprites = new Sprite[10];
    private Sprite[] _CriticalNumberSprites = new Sprite[10];
    private Sprite[] _SelfNormalNumberSprites = new Sprite[10];
    private Sprite[] _SelfCriticalNumberSprites = new Sprite[10];

    float _Spacing = AutoConfig.AutoConst.DamageNumber.Spacing;                   //数字之间的间隔
    float _MoveDistance = AutoConfig.AutoConst.DamageNumber.moveDistance;          // 移动距离
    float _MoveDuration = AutoConfig.AutoConst.DamageNumber.moveDuration;        // 移动持续时间
    float _FadeDuration = AutoConfig.AutoConst.DamageNumber.fadeDuration;        // 淡出持续时间
    float _DelayBeforeRecycle = AutoConfig.AutoConst.DamageNumber.delayBeforeRecycle;  // 回收前延迟时间
    float _NormalScale = AutoConfig.AutoConst.DamageNumber.NormalScale;               // 正常数字缩放
    float _CriticalScale = AutoConfig.AutoConst.DamageNumber.CriticalScale;            // 暴击数字缩放
    float _SelfCriticalScale = AutoConfig.AutoConst.DamageNumber.SelfCriticalScale;     // 自己受伤暴击数字缩放
    float _SelfNormalScale = AutoConfig.AutoConst.DamageNumber.SelfNormalScale;      // 自己受伤正常数字缩放
    float _SelfDelayBeforeRecycle = AutoConfig.AutoConst.DamageNumber.SelfdelayBeforeRecycle;  // 自己受伤回收前延迟时间
    float _SelffadeDuration = AutoConfig.AutoConst.DamageNumber.SelffadeDuration;       // 自己受伤淡出持续时间
    float _SelfmoveDistance = AutoConfig.AutoConst.DamageNumber.SelfmoveDistance;       // 自己受伤伤害数字移动距离
    float _SelfmoveDuration = AutoConfig.AutoConst.DamageNumber.SelfmoveDuration;       // 伤害数字移动时间

    /// <summary>
    /// 暴击动画配置
    /// </summary>
    float _CritRiseDuration = AutoConfig.AutoConst.DamageNumber.CritRiseDuration;   // 暴击数字上升时间
    float _CritRiseHeight = AutoConfig.AutoConst.DamageNumber.CritRiseHeight;   // 暴击数字上升高度
    float _CritRiseScaleStart = AutoConfig.AutoConst.DamageNumber.CritRiseScaleStart;   // 暴击数字上升开始缩放
    float _CritRiseScaleEnd = AutoConfig.AutoConst.DamageNumber.CritRiseScaleEnd;   // 暴击数字上升结束缩放
    float _CritBurstDuration = AutoConfig.AutoConst.DamageNumber.CritBurstDuration;   // 暴击数字爆发时间
    float _CritBurstScale = AutoConfig.AutoConst.DamageNumber.CritBurstScale;   // 暴击数字爆发缩放
    float _CritRecoilDuration = AutoConfig.AutoConst.DamageNumber.CritRecoilDuration;   // 暴击数字击退时间
    float _CritRecoilScale = AutoConfig.AutoConst.DamageNumber.CritRecoilScale;   // 暴击数字击退缩放
    float _CritPauseDuration = AutoConfig.AutoConst.DamageNumber.CritPauseDuration;   // 暴击数字暂停时间
    float _CritFadeDelay = AutoConfig.AutoConst.DamageNumber.CritFadeDelay;   // 暴击数字淡出延迟时间
    /// <summary>
    /// 普通动画配置
    /// </summary>
    /// <returns></returns>
    float _NormalRiseDuration = AutoConfig.AutoConst.DamageNumber.NormalRiseDuration;   // 普通数字上升时间
    float _NormalRiseHeight = AutoConfig.AutoConst.DamageNumber.NormalRiseHeight;   // 普通数字上升高度
    float _NormalPauseDuration = AutoConfig.AutoConst.DamageNumber.NormalPauseDuration;   // 普通数字暂停时间
    float _NormalFadeDelay = AutoConfig.AutoConst.DamageNumber.NormalFadeDelay;   // 普通数字淡出延迟时间
    /// <summary>
    /// 自己受伤动画配置
    /// </summary>
    float _SelfRiseDuration = AutoConfig.AutoConst.DamageNumber.SelfRiseDuration;   // 自己受伤数字上升时间
    float _SelfRiseHeight = AutoConfig.AutoConst.DamageNumber.SelfRiseHeight;   // 自己受伤数字上升高度
    float _SelfPauseDuration = AutoConfig.AutoConst.DamageNumber.SelfPauseDuration;   // 自己受伤数字暂停时间
    float _SelfFadeDelay = AutoConfig.AutoConst.DamageNumber.SelfFadeDelay;   // 自己受伤数字淡出延迟时间



    public bool Initialize()
    {
        for (int i = 0; i < 10; i++)
        {
            _NormalNumberSprites[i] = Resources.Load<Sprite>(AutoConfig.AutoConst.DamageNumber.NormalDamage + "/" + i);
            //Debug.LogWarning($"NormalNumberSprite {i} {_NormalNumberSprites[i]},路径{AutoConfig.AutoConst.DamageNumber.NormalDamage + "/" + i}");
        }
        for (int i = 0; i < 10; i++)
        {
            _CriticalNumberSprites[i] = Resources.Load<Sprite>(AutoConfig.AutoConst.DamageNumber.CriticalDamage + "/" + i);
            //Debug.LogWarning($"CriticalNumberSprite {i} {_CriticalNumberSprites[i]},路径{AutoConfig.AutoConst.DamageNumber.CriticalDamage + "/" + i}");
        }
        for (int i = 0; i < 10; i++)
        {
            _SelfNormalNumberSprites[i] = Resources.Load<Sprite>(AutoConfig.AutoConst.DamageNumber.SelfNormalDamage + "/" + i);
            //Debug.LogWarning($"SelfNormalNumberSprite {i} {_SelfNormalNumberSprites[i]},路径{AutoConfig.AutoConst.DamageNumber.SelfNormalDamage + "/" + i}");
        }
        for (int i = 0; i < 10; i++)
        {
            _SelfCriticalNumberSprites[i] = Resources.Load<Sprite>(AutoConfig.AutoConst.DamageNumber.SelfCriticalDamage + "/" + i);
            //Debug.LogWarning($"CriticalNumberSprite {i} {_SelfCriticalNumberSprites[i]},路径{AutoConfig.AutoConst.DamageNumber.SelfCriticalDamage + "/" + i}");
        }
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        return true;
    }


    public void Release()
    {
        DOTween.KillAll();
    }

    public void ShowNum(ulong Guid, float num, string parentName, ulong castGuid, bool isCritical = false)
    {
        IUnit unit = GameUnitApi.QueryUnit(Guid);
        IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
        IUnit castUnit = GameUnitApi.QueryUnit(castGuid);
        if (unit.Guid == mainCharacter.Guid)//自己受伤
        {
            if (null == unit)
            {
                XApi.Debug($"ShowNum {Guid} {num} {parentName} 单位不存在");
                return;
            }

            Transform parentNode = unit.transform.Find(parentName);
            if (null == parentNode)
            {
                XApi.Debug($"ShowNum {Guid} {num} {parentName} 父节点不存在");
                return;
            }

            GameObject numberObject = SelfCreateNumberObject(num.ToString(), parentNode, isCritical);
            numberObject.transform.position = parentNode.position;

            SelfAnimateNumber(numberObject, isCritical);
        }
        else//其他单位受伤
        {
            if (castGuid == 0)
            {
                XApi.Debug($"ShowNum {Guid} {num} {parentName} 施法者不存在");
                return;
            }
            if (castGuid != mainCharacter.Guid)
            {
                XApi.Debug($"ShowNum {Guid} {num} {parentName} 施法者不是主角色");
                return;
            }
            if (null == unit)
            {
                XApi.Debug($"ShowNum {Guid} {num} {parentName} 单位不存在");
                return;
            }

            Transform parentNode = unit.transform.Find(parentName);
            if (null == parentNode || unit.gameObject == null)
            {
                XApi.Debug($"ShowNum {Guid} {num} {parentName} 父节点不存在");
                return;
            }
            if (isCritical && unit.Guid != _CharacterManager.GetMainCharacterUnit().Guid)
            {
                // 屏幕微震 (0.1秒, 强度0.3)
                Camera.main.transform.DOShakePosition(0.1f, 0.3f, 20, 90, false, true);
            }

            GameObject numberObject = CreateNumberObject(num.ToString(), parentNode, isCritical);
            numberObject.transform.position = parentNode.position;

            AnimateNumber(numberObject, isCritical);
        }

    }


    GameObject CreateNumberObject(string numStr, Transform parent, bool isCritical)
    {
        GameObject numberObject;

        if (_NumberPool.Count > 0)
        {
            numberObject = _NumberPool.Dequeue();
            if (numberObject != null) // 检查对象是否为空
            {
                numberObject.SetActive(true);

                foreach (Transform child in numberObject.transform)
                {
                    GameObject.Destroy(child.gameObject);
                }
            }
        }
        else
        {
            numberObject = new GameObject("ArtNumber");
        }
        if (numberObject != null && parent != null && parent.gameObject != null)
        {
            numberObject.transform.SetParent(parent, false);
        }
        //numberObject.transform.SetParent(parent, false);
        CreateNumberSprites(numStr, numberObject.transform, isCritical);

        return numberObject;
    }
    void CreateNumberSprites(string numStr, Transform parent, bool isCritical)
    {
        for (int i = 0; i < numStr.Length; i++)
        {
            char c = numStr[i];
            if (char.IsDigit(c))
            {
                int digit = c - '0';

                GameObject digitObject = new GameObject($"Digit_{digit}");
                digitObject.transform.SetParent(parent, false);

                SpriteRenderer digitRenderer = digitObject.AddComponent<SpriteRenderer>();
                digitRenderer.sortingOrder = 20;

                float scale = isCritical ? _CriticalScale : _NormalScale;
                digitObject.transform.localScale = new Vector3(scale, scale, scale);
                if (isCritical == true)
                {
                    Sprite criticalSprite = _CriticalNumberSprites[digit];
                    if (criticalSprite == null)
                    {
                        XApi.Error($"CriticalNumberSprite {digit} 不存在");
                        continue;
                    }
                    digitRenderer.sprite = criticalSprite;
                }
                else
                {
                    Sprite sprite = _NormalNumberSprites[digit];
                    if (sprite == null)
                    {
                        XApi.Error($"NumberSprite {digit} 不存在");
                        continue;
                    }
                    digitRenderer.sprite = sprite;
                }

                Vector3 localPosition = new Vector3((i - numStr.Length / 2.0f) * _Spacing, 0, 0);
                digitObject.transform.localPosition = localPosition;
            }
        }
    }

    void AnimateNumber(GameObject numberObject, bool isCritical)
    {
        if (numberObject == null) return;
        numberObject.transform.DOKill();

        Vector3 startPos = numberObject.transform.position;
        Vector3 endPos = startPos + new Vector3(0, _MoveDistance, 0);
        Sequence seq = DOTween.Sequence();

        if (isCritical)
        {
            // ========== 暴击：烟花特效（全部参数化） ==========
            float burstHeight = _MoveDistance * _CritRiseHeight;
            Vector3 burstPoint = startPos + new Vector3(0, burstHeight, 0);

            numberObject.transform.position = startPos;
            numberObject.transform.localScale = Vector3.one * _CritRiseScaleStart;

            // 上升阶段
            seq.Append(numberObject.transform.DOMove(burstPoint, _CritRiseDuration).SetEase(Ease.OutQuad));
            seq.Join(numberObject.transform.DOScale(_CritRiseScaleEnd, _CritRiseDuration).SetEase(Ease.Linear));

            // 爆发阶段
            float burstScale = _CriticalScale * _CritBurstScale;
            seq.Append(numberObject.transform.DOScale(burstScale, _CritBurstDuration).SetEase(Ease.OutBack));

            // 回弹阶段
            float recoilScale = _CriticalScale * _CritRecoilScale;
            seq.Append(numberObject.transform.DOScale(recoilScale, _CritRecoilDuration).SetEase(Ease.OutQuad));

            // 停顿阶段
            seq.AppendInterval(_CritPauseDuration);

            // 上飘阶段
            float floatDuration = _MoveDuration - (_CritRiseDuration + _CritBurstDuration + _CritRecoilDuration + _CritPauseDuration);
            seq.Append(numberObject.transform.DOMove(endPos, Mathf.Max(floatDuration, 0.5f)).SetEase(Ease.Linear));

            // 淡出阶段
            SpriteRenderer[] renderers = numberObject.GetComponentsInChildren<SpriteRenderer>();
            if (renderers.Length > 0)
            {
                foreach (var r in renderers) if (r != null) r.color = Color.white;
                seq.Join(
                    DOTween.To(
                        () => Color.white,
                        color => { foreach (var r in renderers) if (r != null) r.color = color; },
                        new Color(1, 1, 1, 0),
                        _FadeDuration
                    ).SetDelay(_CritFadeDelay).SetEase(Ease.OutQuad)
                );
            }
        }
        else
        {
            // ========== 普通伤害：简洁动画 ==========
            numberObject.transform.localScale = Vector3.zero;
            seq.Append(numberObject.transform.DOScale(_NormalScale, _NormalRiseDuration).SetEase(Ease.OutCubic));

            float jumpHeight = _MoveDistance * _NormalRiseHeight;
            seq.Append(numberObject.transform.DOMoveY(startPos.y + jumpHeight, _NormalRiseDuration).SetEase(Ease.OutQuad));

            seq.AppendInterval(_NormalPauseDuration);

            float floatDuration = _MoveDuration - (_NormalRiseDuration * 2 + _NormalPauseDuration);
            seq.Append(numberObject.transform.DOMove(endPos, Mathf.Max(floatDuration, 0.5f)).SetEase(Ease.Linear));

            SpriteRenderer[] renderers = numberObject.GetComponentsInChildren<SpriteRenderer>();
            if (renderers.Length > 0)
            {
                foreach (var r in renderers) if (r != null) r.color = Color.white;
                seq.Join(
                    DOTween.To(
                        () => Color.white,
                        color => { foreach (var r in renderers) if (r != null) r.color = color; },
                        new Color(1, 1, 1, 0),
                        _FadeDuration
                    ).SetDelay(_NormalFadeDelay).SetEase(Ease.Linear)
                );
            }
        }

        seq.AppendInterval(_DelayBeforeRecycle);
        seq.OnComplete(() => RecycleNumberObject(numberObject));
        seq.Play();

    }


    GameObject SelfCreateNumberObject(string numStr, Transform parent, bool isCritical)
    {
        GameObject numberObject;

        if (_NumberPool.Count > 0)
        {
            numberObject = _NumberPool.Dequeue();
            if (numberObject != null) // 检查对象是否为空
            {
                numberObject.SetActive(true);

                foreach (Transform child in numberObject.transform)
                {
                    GameObject.Destroy(child.gameObject);
                }
            }
        }
        else
        {
            numberObject = new GameObject("ArtNumber");
        }
        if (numberObject != null && parent != null && parent.gameObject != null)
        {
            numberObject.transform.SetParent(parent, false);
        }
        SelfCreateNumberSprites(numStr, numberObject.transform, isCritical);

        return numberObject;
    }
    void SelfCreateNumberSprites(string numStr, Transform parent, bool isCritical)
    {
        for (int i = 0; i < numStr.Length; i++)
        {
            char c = numStr[i];
            if (char.IsDigit(c))
            {
                int digit = c - '0';

                GameObject digitObject = new GameObject($"Digit_{digit}");
                digitObject.transform.SetParent(parent, false);

                SpriteRenderer digitRenderer = digitObject.AddComponent<SpriteRenderer>();
                digitRenderer.sortingOrder = 15;

                float scale = isCritical ? _SelfCriticalScale : _SelfNormalScale;
                digitObject.transform.localScale = new Vector3(scale, scale, scale);
                if (isCritical == true)
                {
                    Sprite criticalSprite = _SelfCriticalNumberSprites[digit];
                    if (criticalSprite == null)
                    {
                        XApi.Error($"CriticalNumberSprite {digit} 不存在");
                        continue;
                    }
                    digitRenderer.sprite = criticalSprite;
                }
                else
                {
                    Sprite sprite = _SelfNormalNumberSprites[digit];
                    if (sprite == null)
                    {
                        XApi.Error($"NumberSprite {digit} 不存在");
                        continue;
                    }
                    digitRenderer.sprite = sprite;
                }

                Vector3 localPosition = new Vector3((i - numStr.Length / 2.0f) * _Spacing, 0, 0);
                digitObject.transform.localPosition = localPosition;
            }
        }
    }

    void SelfAnimateNumber(GameObject numberObject, bool isCritical)
    {
        // if (numberObject == null) return;
        // numberObject.transform.DOKill();

        // Vector3 startPos = numberObject.transform.position;
        // Vector3 endPos = startPos + new Vector3(0, _SelfmoveDistance, 0);

        // Sequence seq = DOTween.Sequence();

        // // 1. **瞬间出现**
        // numberObject.transform.localScale = Vector3.zero;
        // seq.Append(numberObject.transform.DOScale(
        //     isCritical ? _SelfCriticalScale : _SelfNormalScale,
        //     0.05f
        // ).SetEase(Ease.OutCubic));

        // // 2. **上飘到顶**（自己受伤跳得更低更快）
        // float jumpHeight = isCritical ? _SelfmoveDistance * 0.15f : _SelfmoveDistance * 0.1f;
        // seq.Append(numberObject.transform.DOMoveY(
        //     startPos.y + jumpHeight,
        //     0.2f
        // ).SetEase(Ease.OutQuad));

        // // 3. **强制停顿**（自己受伤停顿更短，避免干扰）
        // seq.AppendInterval(0.08f);

        // // 4. **匀速上飘**
        // float floatDuration = _SelfmoveDuration - 0.33f;
        // seq.Append(numberObject.transform.DOMove(
        //     endPos,
        //     Mathf.Max(floatDuration, 0.4f)
        // ).SetEase(Ease.Linear)); // **匀速**

        // // 5. **同时淡出**
        // SpriteRenderer[] renderers = numberObject.GetComponentsInChildren<SpriteRenderer>();
        // if (renderers.Length > 0)
        // {
        //     foreach (var r in renderers) if (r != null) r.color = Color.yellow; // 自己受伤用黄色

        //     seq.Join(
        //         DOTween.To(
        //             () => Color.yellow,
        //             color =>
        //             {
        //                 foreach (var r in renderers) if (r != null) r.color = color;
        //             },
        //             new Color(1, 1, 0, 0),
        //             _SelffadeDuration
        //         ).SetDelay(0.33f).SetEase(Ease.Linear)
        //     );
        // }

        // seq.AppendInterval(_SelfDelayBeforeRecycle);
        // seq.OnComplete(() => RecycleNumberObject(numberObject));

        // seq.Play();
        if (numberObject == null) return;
        numberObject.transform.DOKill();

        Vector3 startPos = numberObject.transform.position;
        Vector3 endPos = startPos + new Vector3(0, _SelfmoveDistance, 0);
        Sequence seq = DOTween.Sequence();

        numberObject.transform.localScale = Vector3.zero;
        seq.Append(numberObject.transform.DOScale(
            isCritical ? _SelfCriticalScale : _SelfNormalScale,
            _SelfRiseDuration
        ).SetEase(Ease.OutCubic));

        float jumpHeight = _SelfmoveDistance * _SelfRiseHeight;
        seq.Append(numberObject.transform.DOMoveY(startPos.y + jumpHeight, _SelfRiseDuration).SetEase(Ease.OutQuad));

        seq.AppendInterval(_SelfPauseDuration);

        float floatDuration = _SelfmoveDuration - (_SelfRiseDuration * 2 + _SelfPauseDuration);
        seq.Append(numberObject.transform.DOMove(endPos, Mathf.Max(floatDuration, 0.4f)).SetEase(Ease.Linear));

        SpriteRenderer[] renderers = numberObject.GetComponentsInChildren<SpriteRenderer>();
        if (renderers.Length > 0)
        {
            Color startColor = isCritical ? new Color(1, 0.5f, 0) : Color.yellow; // 暴击橙色，普通黄色
            foreach (var r in renderers) if (r != null) r.color = startColor;

            seq.Join(
                DOTween.To(
                    () => startColor,
                    color => { foreach (var r in renderers) if (r != null) r.color = color; },
                    new Color(startColor.r, startColor.g, startColor.b, 0),
                    _SelffadeDuration
                ).SetDelay(_SelfFadeDelay).SetEase(Ease.Linear)
            );
        }

        seq.AppendInterval(_SelfDelayBeforeRecycle);
        seq.OnComplete(() => RecycleNumberObject(numberObject));
        seq.Play();
    }



    void RecycleNumberObject(GameObject numberObject)//回收数字对象
    {
        if (numberObject == null) return;
        numberObject.transform.DOKill();
        SpriteRenderer[] renderers = numberObject.GetComponentsInChildren<SpriteRenderer>();
        if (renderers != null)
        {
            foreach (SpriteRenderer renderer in renderers)
            {
                if (renderer != null)
                {
                    renderer.DOKill();
                    renderer.color = Color.white;
                }
            }
        }
        numberObject.transform.SetParent(null, false);
        numberObject.transform.position = Vector3.zero;
        numberObject.transform.localScale = Vector3.one;
        //隐藏并回收
        numberObject.SetActive(false);
        _NumberPool.Enqueue(numberObject);
    }

    Ease GetEaseType(eMoveEaseType easeType)
    {
        switch (easeType)
        {
            case eMoveEaseType.Linear: return Ease.Linear;
            case eMoveEaseType.InQuad: return Ease.InQuad;
            case eMoveEaseType.OutQuad: return Ease.OutQuad;
            case eMoveEaseType.InOutQuad: return Ease.InOutQuad;
            case eMoveEaseType.InCubic: return Ease.InCubic;
            case eMoveEaseType.OutCubic: return Ease.OutCubic;
            case eMoveEaseType.InOutCubic: return Ease.InOutCubic;
            case eMoveEaseType.InQuart: return Ease.InQuart;
            case eMoveEaseType.OutQuart: return Ease.OutQuart;
            case eMoveEaseType.InOutQuart: return Ease.InOutQuart;
            case eMoveEaseType.InSine: return Ease.InSine;
            case eMoveEaseType.OutSine: return Ease.OutSine;
            case eMoveEaseType.InOutSine: return Ease.InOutSine;
            case eMoveEaseType.InExpo: return Ease.InExpo;
            case eMoveEaseType.OutExpo: return Ease.OutExpo;
            case eMoveEaseType.InOutExpo: return Ease.InOutExpo;
            case eMoveEaseType.InCirc: return Ease.InCirc;
            case eMoveEaseType.OutCirc: return Ease.OutCirc;
            case eMoveEaseType.InOutCirc: return Ease.InOutCirc;
            case eMoveEaseType.InBounce: return Ease.InBounce;
            case eMoveEaseType.OutBounce: return Ease.OutBounce;
            case eMoveEaseType.InOutBounce: return Ease.InOutBounce;
            case eMoveEaseType.InElastic: return Ease.InElastic;
            case eMoveEaseType.OutElastic: return Ease.OutElastic;
            case eMoveEaseType.InOutElastic: return Ease.InOutElastic;
            case eMoveEaseType.InBack: return Ease.InBack;
            case eMoveEaseType.OutBack: return Ease.OutBack;
            case eMoveEaseType.InOutBack: return Ease.InOutBack;
            default: return Ease.OutQuad; // 默认值
        }
    }
}


