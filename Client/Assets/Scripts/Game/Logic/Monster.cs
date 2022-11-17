using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
#if UNITY_EDITOR
using UnityEditor;
using System.Xml;
#endif

enum eMonsterAttr
{
    id = 0,
    name,
    desc,
    icon,
    resources,
    type,
    damage,
    speed_attack,
    speed_move,
    range_alert,
    range_track,
    dead_item_group,
    effect_attack,
    effect_accept,
    sound_attack,
    sound_accept,
    sound_dead,
    life,
    max_size,
}

public class Monster : Charactor
{
    public MonsterAttr monsterAttr;
    public override void Awake()
    {
        base.Awake();
        monsterAttr = GetComponent<MonsterAttr>();
    }

    public override void Initialize(int _cfgId, long _gid)
    {
        gid = _gid;
        cfgId = _cfgId;
        AutoConfig.o_config_monster monsterCfg = CharactorManager.Instance.GetMonsterCfg(cfgId);
        monsterAttr.SetType(monsterCfg.type);
        monsterAttr.SetDamage(monsterCfg.damage);
        monsterAttr.SetSpeed_attack(monsterCfg.speed_attack);
        monsterAttr.SetMove_speed(monsterCfg.speed_move);
        monsterAttr.SetRange_alert(monsterCfg.range_alert);
        monsterAttr.SetRange_track(monsterCfg.range_track);
        monsterAttr.SetDead_item_group(monsterCfg.dead_item_group);
        monsterAttr.SetEffect_attack(monsterCfg.effect_attack);
        monsterAttr.SetEffect_accept(monsterCfg.effect_accept);
        monsterAttr.SetSound_attack(monsterCfg.sound_attack);
        monsterAttr.SetSound_accept(monsterCfg.sound_accept);
        monsterAttr.SetSound_dead(monsterCfg.sound_dead);

        InitData(_cfgId);
    }

    public override void InitData(in int config_id)
    {
        AutoConfig.o_config_monster monsterCfg = CharactorManager.Instance.GetMonsterCfg(cfgId);
        monsterAttr.SetMove_speed(monsterCfg.speed_move);
        monsterAttr.SetHp(monsterCfg.life);
        monsterAttr.SetCfg_id(cfgId);
        monsterAttr.SetResources(monsterCfg.resources);
        monsterAttr.SetName(monsterCfg.name);
        monsterAttr.SetDesc(monsterCfg.desc);
        monsterAttr.SetIcon(monsterCfg.icon);
    }

    public override void OnMove(bool _move)
    {

    }

#if UNITY_EDITOR
    [NaughtyAttributes.Button("导出动画信息")]
    void ExportAnimationInfo()
    {
        AnimationClip[] aniClip = AnimationUtility.GetAnimationClips(gameObject);

        XmlDocument doc = new XmlDocument();
        XmlDeclaration declaration = doc.CreateXmlDeclaration("1.0", "UTF-8", null);
        //2.将头部信息添加到xml文档
        doc.AppendChild(declaration);
        //3. 创建一个根节点
        XmlNode root = doc.CreateNode(XmlNodeType.Element, "Root", null);
        doc.AppendChild(root);

        for(int i = 0; i < aniClip.Length; i++)
        {
            AnimationClip clip = aniClip[i];
            XmlElement ele = doc.CreateElement("AnimationClip");
            ele.SetAttribute("name", clip.name);
            ele.SetAttribute("length", clip.length.ToString());
            for(int j = 0; j < clip.events.Length; j++)
            {
                var evnt = clip.events[j];
                XmlElement ele1 = doc.CreateElement("AnimationEvent");
                ele1.SetAttribute("function", evnt.functionName);
                ele1.SetAttribute("time", evnt.time.ToString());
                ele.AppendChild(ele1);
            }
            root.AppendChild(ele);
        }

        doc.Save(Application.dataPath + "/../../Documents/Config/MonsterAnimation.xml");
    }
#endif
}