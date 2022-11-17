using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Player : Charactor
{
    [System.NonSerialized]public PlayerAttr playerAttr;
    public NavMeshAgent agent;
    public float move_speed;
    public Transform shootPoint;
    //左手武器锚点
    public Transform weaponAnchorL;
    //右手武器锚点
    public Transform weaponAnchorR;
    public PlayerControl playerControl;

    protected bool isLocalActor = false;

    public override void Awake()
    {
        playerAttr = GetComponent<PlayerAttr>();
        angleMove = new AngleMove(transform);
        angleMove.moveCallback = OnMove;
    }

    private void Start()
    {
        
    }

    public override void Initialize(int cfg_id, long _gid)
    {
        isLocalActor = _gid == CharactorManager.Instance.LocalPlayerGID;
        gid = _gid;
        cfgId = cfg_id;
        AutoConfig.o_config_role cfg_role = CharactorManager.Instance.GetRoleCfg(cfgId);
        playerAttr.SetWeapon_gun(cfg_role.weapon_gun);
        playerAttr.SetWeapon_melee(cfg_role.weapon_near);

        InitData(cfg_id);

        move_speed = cfg_role.speed_move;
    }

    public override void InitData(in int config_id)
    {
        AutoConfig.o_config_role cfg_role = CharactorManager.Instance.GetRoleCfg(config_id);
        playerAttr.SetMove_speed(cfg_role.speed_move);
        playerAttr.SetHp(cfg_role.life);
        playerAttr.SetCfg_id(cfgId);
        playerAttr.SetResources(cfg_role.resources);
        playerAttr.SetName(cfg_role.name);
        playerAttr.SetDesc(cfg_role.desc);
        playerAttr.SetIcon(cfg_role.icon);
    }

    public override void Update()
    {
        float dt = Time.deltaTime;
        if(angleMove.onMove)
        {
            agent.Move(angleMove.move_dir * move_speed * dt);
        }
        if (rotateParam.onRotate)
        {
            rotateParam.Lerp(transform, dt);
            CheckRotate();
        }

        if(isLocalActor && Input.GetKeyDown(KeyCode.Space))
        {
            OnShoot();
        }
    }

    void OnShoot()
    {
        RaycastHit hitInfo;
        bool isHit = Physics.Raycast(shootPoint.position, transform.forward, out hitInfo, 100, LAYERS.MONSTER | LAYERS.STATICS);
        if(isHit)
        {
            oPLAYER_HIT info = new oPLAYER_HIT();
            info.HitInfo = new oPlayerHit();
            Charactor _monster = hitInfo.transform.gameObject.GetComponent<Charactor>();
            if(_monster)
            {
                info.HitInfo.Gid = _monster.Gid;
                info.HitInfo.HitLayer = eHitLayer.LayerMonster;
            }
            else
            {
                info.HitInfo.Gid = 0;
                info.HitInfo.HitLayer = eHitLayer.LayerWall;
            }
            info.HitInfo.HitPos = new oVector3() { X = hitInfo.point.x, Y = hitInfo.point.y, Z = hitInfo.point.z};
            info.HitInfo.HitNormal = new oVector3() { X = hitInfo.normal.x, Y = hitInfo.normal.y, Z = hitInfo.normal.z };
            ConnectionManager.SendProto<oPLAYER_HIT>(eConnectionType.GameGate, eCPID.PlayerHit, info);
        }
    }

    public override void OnMove(bool _move)
    {
        playerControl.SetMove(_move);
    }

    private void OnDrawGizmos()
    {
        if(isLocalActor)
        {
            Color color = Gizmos.color;
            Gizmos.color = Color.blue;
            Gizmos.DrawLine(shootPoint.position, shootPoint.position + transform.forward * 100);
            Gizmos.color = color;
        }
    }

    [NaughtyAttributes.Button("装备枪械")]
    void TestGun()
    {
        Weapon weapon = Instantiate<Weapon>(Resources.Load<Weapon>("Prefabs/weapons/SA_Wep_AssaultRifle01"));
        weapon.transform.parent = weaponAnchorR;
        weapon.transform.localPosition = Vector3.zero;
        weapon.transform.localRotation = Quaternion.identity;
        weapon.transform.localScale = Vector3.one;
        playerControl.SetWeapon(weapon.category);
    }

    [NaughtyAttributes.Button("测试开火")]
    void TestFire()
    {
        Animator ani = playerControl.animator;
        bool _fire = ani.GetBool("Shoot_b");
        playerControl.SetFire(!_fire);
    }

    [NaughtyAttributes.Button("连发")]
    void TestAuto()
    {
        Animator ani = playerControl.animator;
        bool _auto = ani.GetBool("FullAuto_b");
        playerControl.SetAutoMode(!_auto);
    }
}
