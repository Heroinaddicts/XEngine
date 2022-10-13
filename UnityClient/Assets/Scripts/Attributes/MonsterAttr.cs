using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class MonsterAttr : CharactorAttr
{
    public enum eMonsterAttrIndex
    {
        Type = 9,
        Damage = 10,
        Speed_attack = 11,
        Range_alert = 12,
        Range_track = 13,
        Dead_item_group = 14,
        Effect_attack = 15,
        Effect_accept = 16,
        Sound_attack = 17,
        Sound_accept = 18,
        Sound_dead = 19,
    }

    void Awake()
    {
        Attributes = new AttrInfo[20];

        Attributes[0] = new AttrInfo("gid", 0, eAttributeType.INT, "no desc");
        Attributes[1] = new AttrInfo("id", 1, eAttributeType.INT, "no desc");
        Attributes[2] = new AttrInfo("move_speed", 2, eAttributeType.FLOAT, "no desc");
        Attributes[3] = new AttrInfo("hp", 3, eAttributeType.FLOAT, "no desc");
        Attributes[4] = new AttrInfo("cfg_id", 4, eAttributeType.INT, "no desc");
        Attributes[5] = new AttrInfo("resources", 5, eAttributeType.STRING, "no desc");
        Attributes[6] = new AttrInfo("name", 6, eAttributeType.STRING, "no desc");
        Attributes[7] = new AttrInfo("desc", 7, eAttributeType.STRING, "no desc");
        Attributes[8] = new AttrInfo("icon", 8, eAttributeType.STRING, "no desc");
        Attributes[9] = new AttrInfo("type", 9, eAttributeType.INT, "no desc");
        Attributes[10] = new AttrInfo("damage", 10, eAttributeType.FLOAT, "no desc");
        Attributes[11] = new AttrInfo("speed_attack", 11, eAttributeType.FLOAT, "no desc");
        Attributes[12] = new AttrInfo("range_alert", 12, eAttributeType.FLOAT, "no desc");
        Attributes[13] = new AttrInfo("range_track", 13, eAttributeType.FLOAT, "no desc");
        Attributes[14] = new AttrInfo("dead_item_group", 14, eAttributeType.INT, "no desc");
        Attributes[15] = new AttrInfo("effect_attack", 15, eAttributeType.STRING, "no desc");
        Attributes[16] = new AttrInfo("effect_accept", 16, eAttributeType.STRING, "no desc");
        Attributes[17] = new AttrInfo("sound_attack", 17, eAttributeType.STRING, "no desc");
        Attributes[18] = new AttrInfo("sound_accept", 18, eAttributeType.STRING, "no desc");
        Attributes[19] = new AttrInfo("sound_dead", 19, eAttributeType.STRING, "no desc");
    }
    public long GetType()
    {
        return Attributes[9].GetInt();
    }

    public void SetType(in long value, in object context = null, in bool trigger = true)
    {
        Attributes[9].SetInt(value, context, trigger);
    }

    public float GetDamage()
    {
        return Attributes[10].GetFloat();
    }

    public void SetDamage(in float value, in object context = null, in bool trigger = true)
    {
        Attributes[10].SetFloat(value, context, trigger);
    }

    public float GetSpeed_attack()
    {
        return Attributes[11].GetFloat();
    }

    public void SetSpeed_attack(in float value, in object context = null, in bool trigger = true)
    {
        Attributes[11].SetFloat(value, context, trigger);
    }

    public float GetRange_alert()
    {
        return Attributes[12].GetFloat();
    }

    public void SetRange_alert(in float value, in object context = null, in bool trigger = true)
    {
        Attributes[12].SetFloat(value, context, trigger);
    }

    public float GetRange_track()
    {
        return Attributes[13].GetFloat();
    }

    public void SetRange_track(in float value, in object context = null, in bool trigger = true)
    {
        Attributes[13].SetFloat(value, context, trigger);
    }

    public long GetDead_item_group()
    {
        return Attributes[14].GetInt();
    }

    public void SetDead_item_group(in long value, in object context = null, in bool trigger = true)
    {
        Attributes[14].SetInt(value, context, trigger);
    }

    public string GetEffect_attack()
    {
        return Attributes[15].GetString();
    }

    public void SetEffect_attack(in string value, in object context = null, in bool trigger = true)
    {
        Attributes[15].SetString(value, context, trigger);
    }

    public string GetEffect_accept()
    {
        return Attributes[16].GetString();
    }

    public void SetEffect_accept(in string value, in object context = null, in bool trigger = true)
    {
        Attributes[16].SetString(value, context, trigger);
    }

    public string GetSound_attack()
    {
        return Attributes[17].GetString();
    }

    public void SetSound_attack(in string value, in object context = null, in bool trigger = true)
    {
        Attributes[17].SetString(value, context, trigger);
    }

    public string GetSound_accept()
    {
        return Attributes[18].GetString();
    }

    public void SetSound_accept(in string value, in object context = null, in bool trigger = true)
    {
        Attributes[18].SetString(value, context, trigger);
    }

    public string GetSound_dead()
    {
        return Attributes[19].GetString();
    }

    public void SetSound_dead(in string value, in object context = null, in bool trigger = true)
    {
        Attributes[19].SetString(value, context, trigger);
    }

}

