using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BaseObject : MonoBehaviour
{
    public BaseObjectAttr attr;

    protected long gid = 0;

    public long Gid
    {
        get { return gid; }
    }

    public virtual void Initialize(long _gid)
    {
        gid = _gid;
    }
}
