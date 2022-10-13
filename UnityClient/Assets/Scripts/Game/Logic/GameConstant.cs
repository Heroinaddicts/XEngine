using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum OBJ_TYPE
{
    ZOMBIE = 0,
    ITEM = 1,
}

public enum ZOMBIE
{
    ZOMBIE_1 = 0,
    ZOMBIE_2 = 1,
}

public enum REFRESH_TYPE
{
    RANDOM = 0,
    SEQ = 1,
}

public enum VALUE_TYPE
{
    INTEGER = 0,
    FLOAT = 1,
    STRING = 2,
}

public class LAYERS
{
    public static int PLAYER = 1 << 6;
    public static int MONSTER = 1 << 7;
    public static int STATICS = 1 << 8;
}

public enum WEAPON_CATEGORY
{
    NONE = 0,
    HANDGUN = 1,
    AUTO = 2,
    AUTO1 = 3,
    SHOTGUN = 4,
    RIFLE = 5,
    RIFLE1 = 6,
    MACHINEGUN = 7,
    RPG = 8,
    MINIGUN = 9,
}
