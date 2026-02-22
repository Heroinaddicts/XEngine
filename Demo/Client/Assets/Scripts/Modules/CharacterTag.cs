using System;
using AutoConfig;
using GameUnit;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using XEngine;
using XGame;
using XGameClient;
using XUtils;
using static GameUnitApi;


public interface ICharacterTag : IModule
{

}
public class CharacterTag : ICharacterTag
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<ICharacterTag, CharacterTag>();
    }

    const int TarRendererOrder = 9;

    Sprite _TeammateTag;
    Sprite _EnemyTag;
    Sprite _SelfTag;

    IGameSceneManager _GameSceneManager;
    ICharacterManager _CharacterManager;
    ITeamManager _TeamManager;

    public bool Initialize()
    {
        _TeammateTag = Resources.Load<Sprite>(AutoConfig.AutoConst.Tags.TeammateTag);
        _EnemyTag = Resources.Load<Sprite>(AutoConfig.AutoConst.Tags.EnemyTag);
        _SelfTag = Resources.Load<Sprite>(AutoConfig.AutoConst.Tags.SelfTag);

        GameUnitApi.RegistUnitEvent(eUnitType.Character, OnEntityUnitEvent);
        GameUnitApi.RegistUnitEvent(eUnitType.Monster, OnEntityUnitEvent);
        GameUnitApi.RegistUnitEvent(eUnitType.Boss, OnEntityUnitEvent);
        return true;
    }

    public bool Launch()
    {
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _TeamManager = Launcher.GetModule<ITeamManager>();
        return true;
    }

    public void Release()
    {

    }

    SpriteRenderer GetSpriteRenderer(GameObject go)
    {
        SpriteRenderer spriteRenderer = go.GetComponent<SpriteRenderer>();
        if (null == spriteRenderer)
        {
            spriteRenderer = go.AddComponent<SpriteRenderer>();
        }
        spriteRenderer.sortingOrder = TarRendererOrder;
        return spriteRenderer;
    }

    void OnCharacterTagCheck(in XApi.ITimer.eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder) 
    {
        IUnit unit = obj as IUnit;
        if (_CharacterManager.IsSameAccount(unit))
        {
            GameObject tag = GameObjectCommon.GetChildByPath(unit.gameObject, "TagRing");
            if (tag == null)
            {
                XApi.Debug("没找到TagRing对象");
                Debug.LogWarning("没找到TagRing对象");
                return;
            }
            GetSpriteRenderer(tag).sprite = _SelfTag;
            unit.RegistUInt64AttributeChange(eEntityAttr.TeamGuid_UInt64, eUnitAttributeEvent.Changed, OnTeamGuidChanged);
        }
        else
        {
            float scale = 1f;
            switch (unit.Type)
            {
                case eUnitType.Character:
                    scale = AutoConfig.Character.Get(unit.ConfigID)._CircleArea;
                    goto default;
                case eUnitType.Monster:
                    scale = AutoConfig.Monster.Get(unit.ConfigID)._CircleArea;
                    goto default;
                case eUnitType.Boss:
                    scale = AutoConfig.Monster.Get(unit.ConfigID)._CircleArea;
                    goto default;
                default:
                    {
                        if (_TeamManager.IsSameTeam(unit))
                        {
                            GameObject tag = GameObjectCommon.GetChildByPath(unit.gameObject, "TagRing");
                            tag.transform.localScale = Vector3.one * scale;
                            GetSpriteRenderer(tag).sprite = _TeammateTag;

                        }
                        else
                        {
                            if (_GameSceneManager.GetCurrentSceneConfig()._SceneType == AutoEnum.eSceneType.BattleCopy)
                            {
                                GameObject tag = GameObjectCommon.GetChildByPath(unit.gameObject, "TagRing");
                                tag.transform.localScale = Vector3.one * scale;
                                GetSpriteRenderer(tag).sprite = _EnemyTag;
                                tag.SetActive(!unit.GetBool(eEntityAttr.IsCantDamaged_Bool));
                                unit.RegistBoolAttributeChange(eEntityAttr.IsCantDamaged_Bool, eUnitAttributeEvent.Changed, OnIsCantDamagedChanged);

                            }
                        }

                        unit.RegistUInt64AttributeChange(eEntityAttr.TeamGuid_UInt64, eUnitAttributeEvent.Changed, OnTeamGuidChanged);
                        break;
                    }
            }
        }
    }

    void OnEntityUnitEvent(in eUnitEvent ev, IUnit unit, in bool isParentType, in string file, in int line)
    {
        switch(unit.Type)
        {
            case eUnitType.Character:
            case eUnitType.Monster:
            case eUnitType.Boss:
                {
                    if (ev == eUnitEvent.Create)
                    {
                        XApi.TimerApi.StartTimer(OnCharacterTagCheck, unit, null, SafeSystem.Time.Second, XApi.ITimer.Unlimited, SafeSystem.Time.Second);
                    }
                    else
                    {
                        XApi.TimerApi.StopTimer(OnCharacterTagCheck, unit);
                    }
                    break;
                }
        }
    }

    bool OnIsCantDamagedChanged(in eUnitAttributeEvent ev, in IUnit unit, in int index, in bool oldValue, in bool newValue)
    {
        GameObject tag = GameObjectCommon.GetChildByPath(unit.gameObject, "TagRing");
        tag.SetActive(!newValue);
        return true;
    }

    bool OnTeamGuidChanged(in eUnitAttributeEvent ev, in IUnit unit, in int index, in UInt64 oldValue, in UInt64 newValue)
    {
        if (!_CharacterManager.IsSameAccount(unit))
        {
            if (_TeamManager.IsSameTeam(unit))
            {
                GameObject tag = GameObjectCommon.GetChildByPath(unit.gameObject, "TagRing");
                GetSpriteRenderer(tag).sprite = _TeammateTag;
            }
            else
            {
                GameObject tag = GameObjectCommon.GetChildByPath(unit.gameObject, "TagRing");
                if (_GameSceneManager.GetCurrentSceneConfig()._SceneType == AutoEnum.eSceneType.BattleCopy)
                {
                    GetSpriteRenderer(tag).sprite = _EnemyTag;
                }
                else {
                    GetSpriteRenderer(tag).sprite = null;
                }
            }
        }
        return true;
    }
}
