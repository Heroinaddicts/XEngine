using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class CharactorAttr : BaseObjectAttr
{
    public enum eCharactorAttrIndex
    {
        Id = 1,
        Move_speed = 2,
        Hp = 3,
        Cfg_id = 4,
        Resources = 5,
        Name = 6,
        Desc = 7,
        Icon = 8,
    }

    void Awake()
    {
        Attributes = new AttrInfo[9];

        Attributes[0] = new AttrInfo("gid", 0, eAttributeType.INT, "no desc");
        Attributes[1] = new AttrInfo("id", 1, eAttributeType.INT, "no desc");
        Attributes[2] = new AttrInfo("move_speed", 2, eAttributeType.FLOAT, "no desc");
        Attributes[3] = new AttrInfo("hp", 3, eAttributeType.FLOAT, "no desc");
        Attributes[4] = new AttrInfo("cfg_id", 4, eAttributeType.INT, "no desc");
        Attributes[5] = new AttrInfo("resources", 5, eAttributeType.STRING, "no desc");
        Attributes[6] = new AttrInfo("name", 6, eAttributeType.STRING, "no desc");
        Attributes[7] = new AttrInfo("desc", 7, eAttributeType.STRING, "no desc");
        Attributes[8] = new AttrInfo("icon", 8, eAttributeType.STRING, "no desc");
    }
    public long GetId()
    {
        return Attributes[1].GetInt();
    }

    public void SetId(in long value, in object context = null, in bool trigger = true)
    {
        Attributes[1].SetInt(value, context, trigger);
    }

    public float GetMove_speed()
    {
        return Attributes[2].GetFloat();
    }

    public void SetMove_speed(in float value, in object context = null, in bool trigger = true)
    {
        Attributes[2].SetFloat(value, context, trigger);
    }

    public float GetHp()
    {
        return Attributes[3].GetFloat();
    }

    public void SetHp(in float value, in object context = null, in bool trigger = true)
    {
        Attributes[3].SetFloat(value, context, trigger);
    }

    public long GetCfg_id()
    {
        return Attributes[4].GetInt();
    }

    public void SetCfg_id(in long value, in object context = null, in bool trigger = true)
    {
        Attributes[4].SetInt(value, context, trigger);
    }

    public string GetResources()
    {
        return Attributes[5].GetString();
    }

    public void SetResources(in string value, in object context = null, in bool trigger = true)
    {
        Attributes[5].SetString(value, context, trigger);
    }

    public string GetName()
    {
        return Attributes[6].GetString();
    }

    public void SetName(in string value, in object context = null, in bool trigger = true)
    {
        Attributes[6].SetString(value, context, trigger);
    }

    public string GetDesc()
    {
        return Attributes[7].GetString();
    }

    public void SetDesc(in string value, in object context = null, in bool trigger = true)
    {
        Attributes[7].SetString(value, context, trigger);
    }

    public string GetIcon()
    {
        return Attributes[8].GetString();
    }

    public void SetIcon(in string value, in object context = null, in bool trigger = true)
    {
        Attributes[8].SetString(value, context, trigger);
    }

}

