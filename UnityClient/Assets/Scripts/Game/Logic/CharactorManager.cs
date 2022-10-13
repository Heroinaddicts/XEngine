using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharactorManager : Singleton<CharactorManager>
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    public static void EnemyManagerOnInit()
    {
        CharactorManager.Instance.Registe();
    }

    protected IDictionary<int, AutoConfig.o_config_monster> monster_cfg;
    protected IDictionary<int, AutoConfig.o_config_role> role_cfg;
    protected Dictionary<long, Charactor> charactors = new Dictionary<long, Charactor>();
    protected Dictionary<long, Player> players = new Dictionary<long, Player>();
    protected event System.Action<Charactor, bool> onLocalPlayerAppear;
    public Transform root;
    public Charactor LocalPlayer { get; set; }
    public long LocalPlayerGID { get; set; }

    public void Registe()
    {
        ConnectionManager.RegisterProto<oOBJECT_APPER>(eSPID.ObjectApper, OnSPIDObjectAppear);
        ConnectionManager.RegisterProto<oOBJECT_DISAPPER>(eSPID.ObjectDisapper, OnSPIDObjectDisAppear);
        ConnectionManager.RegisterProto<oSTATUS_CHANGED>(eSPID.StatusChanged, OnSPIDActorMove);
        ConnectionManager.RegisterProto<oBROADCAST_PLAYER_MOVE>(eSPID.BroadcastPlayerMove, OnSPIDPlayerMove);
        ConnectionManager.RegisterProto<oBROADCAST_PLAYER_STOP>(eSPID.BroadcastPlayerStop, OnSPIDPlayerStop);
        ConnectionManager.RegisterProto<oBROADCAST_PLAYER_HIT>(eSPID.BroadcastPlayerHit, OnSPIDPlayerHit);
    }

    public void OnSPIDObjectAppear(in Connection con, in oOBJECT_APPER info)
    {
        if (monster_cfg == null)
        {
            monster_cfg = Module.GetModule<Config>().GetConfig().o_config_monster_map;
            role_cfg = Module.GetModule<Config>().GetConfig().o_config_role_map;
        }

        for (int i = 0; i < info.Objects.Count; i++)
        {
            oObject enemyInfo = info.Objects[i];
            SpawnCharactor(enemyInfo.ConfigId, enemyInfo.Gid, enemyInfo.Type, new Vector3(enemyInfo.Pos.X, enemyInfo.Pos.Y, enemyInfo.Pos.Z), Quaternion.identity);
        }
    }

    public void OnSPIDObjectDisAppear(in Connection con, in oOBJECT_DISAPPER info)
    {
        for (int i = 0; i < info.ObjectIds.Count; i++)
        {
            Int64 id = info.ObjectIds[i];
            if (charactors.ContainsKey(id))
            {
                Charactor _charactor = charactors[id];
                charactors.Remove(id);
                Destroy(_charactor.gameObject);
            }
        }
    }

    public void OnSPIDActorMove(in Connection con, in oSTATUS_CHANGED info)
    {
        if (charactors.ContainsKey(info.Gid))
        {
            oMoveInfo m = info.MoveInfo;
            charactors[info.Gid].Move(charactors[info.Gid].gameObject.transform.position, new Vector3(m.EndPosition.X, m.EndPosition.Y, m.EndPosition.Z));
        }
    }

    public void OnSPIDPlayerMove(in Connection con, in oBROADCAST_PLAYER_MOVE info)
    {
        if (charactors.ContainsKey(info.Gid))
        {
            charactors[info.Gid].Move(new Vector3(info.StartPos.X, info.StartPos.Y, info.StartPos.Z), info.Angle);
        }
    }

    public void OnSPIDPlayerStop(in Connection con, in oBROADCAST_PLAYER_STOP info)
    {
        if (charactors.ContainsKey(info.Gid))
        {
            charactors[info.Gid].StopMove(new Vector3(info.StartPos.X, info.StartPos.Y, info.StartPos.Z));
            Debug.LogError("OnSPIDPlayerStop " + Time.unscaledTime);
        }
    }

    public void OnSPIDPlayerHit(in Connection con, in oBROADCAST_PLAYER_HIT info)
    {
        //if(charactors.ContainsKey(info.HitInfo.Gid))
        //{
        //    Charactor _charactor = charactors[info.HitInfo.Gid];
        //    charactors.Remove(info.HitInfo.Gid);
        //    Destroy(_charactor.gameObject);
        //}
    }

    public void SpawnCharactor(int cfg_id, long gid, in eObjectType type, Vector3 pos, Quaternion rot)
    {
        string res_path = "";
        if (type == eObjectType.Monster)
            res_path = monster_cfg[cfg_id].resources;
        else
            res_path = role_cfg[cfg_id].resources;

        Charactor tmp = Resources.Load<Charactor>(res_path);
#if UNITY_EDITOR
        if (!tmp)
        {
            Debug.LogError("can not load prefab " + res_path);
        }

        tmp.gameObject.name = "Role_" + gid.ToString();
#endif
        Charactor charactor = Instantiate<Charactor>(tmp, pos, rot);
        charactor.Initialize(cfg_id, gid);
        charactor.transform.parent = GameObject.Find("PlayerRoot").transform;

        if (charactors.ContainsKey(gid))
        {
            Debug.LogError("already has charactor " + gid.ToString());
            charactors.Remove(gid);
        }

        charactors.Add(gid, charactor);
        charactor.BindDeadCallback(OnCharactorDie);

        if (gid == LocalPlayerGID)
        {
#if UNITY_EDITOR
            Debug.LogError("Local GID = " + LocalPlayerGID.ToString());
#endif
            LocalPlayerAppeared(LocalPlayerGID, charactor);
        }
    }

    public void OnCharactorDie(Charactor _montser)
    {
        long gid = _montser.Gid;
        if (charactors.ContainsKey(_montser.Gid))
        {
            Charactor m = charactors[_montser.Gid];
            GameObject.Destroy(m.gameObject);
            charactors.Remove(gid);

            if (gid == LocalPlayerGID)
            {
                LocalPlayerDisAppeared();
            }
        }
        else
        {
            Debug.LogError("Delete Monster Not Found, gid = " + gid);
        }
    }

    void LocalPlayerAppeared(long gid, Charactor charactor)
    {
        LocalPlayer = charactor;

        if (GameApp.Instance.CurSceneCamera != null)
        {
            GameApp.Instance.CurSceneCamera.SetFocusTrans(charactor.transform);
        }

        PlayerInput.Instance.SetEnable(true);

        if (null != onLocalPlayerAppear) onLocalPlayerAppear(charactor, true);
    }

    void LocalPlayerDisAppeared()
    {
        LocalPlayer = null;
        GameApp.Instance.CurSceneCamera.SetFocusTrans(null);
        PlayerInput.Instance.SetEnable(false);

        if (null != onLocalPlayerAppear) onLocalPlayerAppear(null, false);
    }

    public void BindLocalPlayerAppear(System.Action<Charactor, bool> action, in bool bind)
    {
        if (bind)
        {
            onLocalPlayerAppear += action;
        }
        else
        {
            onLocalPlayerAppear -= action;
        }
    }

    public Charactor GetMonster(int _gid)
    {
        if (charactors.ContainsKey(_gid))
        {
            return charactors[_gid];
        }
        return null;
    }

    public AutoConfig.o_config_monster GetMonsterCfg(int id)
    {
        return monster_cfg[id];
    }

    public AutoConfig.o_config_role GetRoleCfg(int id)
    {
        return role_cfg[id];
    }
}
