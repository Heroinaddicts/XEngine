using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class PlayerAttr : CharactorAttr
{
    public enum ePlayerAttrIndex
    {
        Weapon_melee = 9,
        Weapon_gun = 10,
    }

    void Awake()
    {
        Attributes = new AttrInfo[11];

        Attributes[0] = new AttrInfo("gid", 0, eAttributeType.INT, "no desc");
        Attributes[1] = new AttrInfo("id", 1, eAttributeType.INT, "no desc");
        Attributes[2] = new AttrInfo("move_speed", 2, eAttributeType.FLOAT, "no desc");
        Attributes[3] = new AttrInfo("hp", 3, eAttributeType.FLOAT, "no desc");
        Attributes[4] = new AttrInfo("cfg_id", 4, eAttributeType.INT, "no desc");
        Attributes[5] = new AttrInfo("resources", 5, eAttributeType.STRING, "no desc");
        Attributes[6] = new AttrInfo("name", 6, eAttributeType.STRING, "no desc");
        Attributes[7] = new AttrInfo("desc", 7, eAttributeType.STRING, "no desc");
        Attributes[8] = new AttrInfo("icon", 8, eAttributeType.STRING, "no desc");
        Attributes[9] = new AttrInfo("weapon_melee", 9, eAttributeType.INT, "no desc");
        Attributes[10] = new AttrInfo("weapon_gun", 10, eAttributeType.INT, "no desc");
    }
    public long GetWeapon_melee()
    {
        return Attributes[9].GetInt();
    }

    public void SetWeapon_melee(in long value, in object context = null, in bool trigger = true)
    {
        Attributes[9].SetInt(value, context, trigger);
    }

    public long GetWeapon_gun()
    {
        return Attributes[10].GetInt();
    }

    public void SetWeapon_gun(in long value, in object context = null, in bool trigger = true)
    {
        Attributes[10].SetInt(value, context, trigger);
    }

}

