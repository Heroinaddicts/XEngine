using NUnit.Framework;
using System;
using System.Collections.Generic;
using UnityEngine;
using XGame;
using XUtils;

public interface IGameTilemap
{
    bool CanMove(in Vector2 pos, in string ground = null);

    
    void FindPath(in Vector2 start, in Vector2 end, XAStar.fXStarPathResult callback);
    void ReleaseTilemap();

    bool Raycast(in Vector2 origin, in float angle, in float distance, ref Vector2 hit);
}

public interface ITilemapManage : IModule
{
    void LoadTilemap(in string res, Action<IGameTilemap> action);
    void ReleaseTilemap(IGameTilemap map);
}

class TilemapManage : ITilemapManage
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<ITilemapManage, TilemapManage>();
    }

    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        return true;
    }

    public void Release()
    {

    }

    public void LoadTilemap(in string res, Action<IGameTilemap> action)
    {
        GameObject ResTileMap = Resources.Load<GameObject>(res);
        if (ResTileMap == null)
        {
            //Debug.LogError("路径错误: " + res);
            return;
        }
        GameObject instance = GameObject.Instantiate(ResTileMap);

        instance.transform.position = Vector3.zero;

        IGameTilemap tilemap = instance.GetComponent<GameTilemap>();

        if (tilemap == null)
        {
            tilemap = instance.AddComponent<GameTilemap>();
        }
        action?.Invoke(tilemap);
    }

    public void ReleaseTilemap(IGameTilemap map)
    {
        if (map != null)
        {
            map.ReleaseTilemap();
            map = null;
        }

    }
}
