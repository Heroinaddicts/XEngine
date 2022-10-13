using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class BaseObjectAttr : IAttribute
{
    public enum eBaseObjectAttrIndex
    {
        Gid = 0,
    }

    void Awake()
    {
        Attributes = new AttrInfo[1];

        Attributes[0] = new AttrInfo("gid", 0, eAttributeType.INT, "no desc");
    }
    public long GetGid()
    {
        return Attributes[0].GetInt();
    }

    public void SetGid(in long value, in object context = null, in bool trigger = true)
    {
        Attributes[0].SetInt(value, context, trigger);
    }

}

