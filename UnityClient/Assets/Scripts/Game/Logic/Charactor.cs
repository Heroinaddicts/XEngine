using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public abstract class Charactor : BaseObject
{
    [System.NonSerialized] protected CharactorAttr charactorAttr;
    protected event Action<Charactor> evntOnDead;
    protected event Action<Charactor> evntOnBirth;
    protected event Action<bool> onMove;

    [System.NonSerialized]
    protected PointMove moveParam;
    [System.NonSerialized]
    protected AngleMove angleMove;
    [System.NonSerialized]
    protected RotateParam rotateParam = new RotateParam();

    [NaughtyAttributes.ReadOnly]
    public int cfgId;
    public abstract void Initialize(int cfg_id, long gid);
    public abstract void InitData(in int config_id);
    public abstract void OnMove(bool _move);

    public virtual void Awake()
    {
        charactorAttr = GetComponent<CharactorAttr>();
        moveParam = new PointMove(transform);
        moveParam.moveCallback = OnMove;

        //GetComponent<Collider>().Raycast()
        //Physics.Raycast()
    }


    public virtual void Move(Vector3 _cur, Vector3 _target)
    {
        moveParam.Start(_target);
        RotateToPoint(moveParam.move_target);
    }

    public virtual void Move(Vector3 _start, float angle)
    {
        float x = (float)Math.Sin(angle);
        float z = (float)Math.Cos(angle);
        transform.position = _start;
        Vector3 dir = Vector3.Normalize(new Vector3(x, 0, z));
        angleMove.Start(_start, dir);
        RotateToQuat(Quaternion.Euler(0, angle * Mathf.Rad2Deg, 0));
    }

    public void StopMove(Vector3 pos)
    {
        angleMove.Stop(pos);
    }

    protected void RotateToPoint(Vector3 point)
    {
        Vector3 dir = point - transform.position;
        if (!dir.Equals(Vector3.zero))
        {
            rotateParam.Start(Quaternion.LookRotation(dir));
        }
    }

    protected void RotateToQuat(Quaternion q)
    {
        rotateParam.Start(q);
    }

    public virtual void Update()
    {
        float dt = Time.deltaTime;
        if (moveParam.onMove)
        {
            transform.Translate(moveParam.move_dir * charactorAttr.GetMove_speed() * dt * 0.95f, Space.World);
            CheckIsArrival();
        }
        if (rotateParam.onRotate)
        {
            rotateParam.Lerp(transform, dt);
            CheckRotate();
        }
    }

    public bool CheckIsArrival()
    {
        Vector3 path_dir = (moveParam.move_target - transform.position);
        float dot = Vector3.Dot(path_dir, moveParam.move_dir);
        if (dot < 0)
        {
            moveParam.onMove = false;
            transform.position = moveParam.move_target;
            return true;
        }
        return false;
    }

    public void CheckRotate()
    {
        float eularY = rotateParam.GetEularY(transform.rotation);
        if (Mathf.Abs(eularY) < 1)
        {
            rotateParam.Stop();
        }
    }

    private void OnDrawGizmosSelected()
    {
        if (moveParam != null && moveParam.onMove)
        {
            Color color = Gizmos.color;
            Gizmos.color = Color.red;
            Gizmos.DrawSphere(moveParam.move_target, 0.5f);
            Gizmos.DrawLine(transform.position, moveParam.move_target);
            Gizmos.color = color;
        }
    }

    public void BeDamage(float _damage)
    {
        float hp = charactorAttr.GetHp();
        hp = Math.Max(0, hp - _damage);
        charactorAttr.SetHp(hp);
    }

    public void BindDeadCallback(Action<Charactor> action)
    {
        evntOnDead += action;
    }

    private void OnHPChanged()
    {
        if (charactorAttr.GetHp() <= 0)
        {
            if (evntOnDead != null) evntOnDead(this);
        }
    }
}

public class PointMove
{
    //[System.NonSerialized] public float move_speed = 20;
    [System.NonSerialized] public Vector3 move_target;
    [System.NonSerialized] public Vector3 move_dir;
    [System.NonSerialized] public bool onMove = false;
    public Action<bool> moveCallback;

    protected Transform trans;
    PointMove() { }
    public PointMove(Transform t) { trans = t; }

    public void Start(Vector3 _target)
    {
        move_target = _target;
        move_dir = (move_target - trans.position).normalized;
        onMove = true;
        if (null != moveCallback) moveCallback(true);
    }

    public void Stop()
    {
        onMove = false;
        if (null != moveCallback) moveCallback(false);
    }
}

public class AngleMove
{
    [System.NonSerialized] public Vector3 start_pos;
    [System.NonSerialized] public Vector3 move_dir;
    [System.NonSerialized] public Vector3 stop_pos;
    [System.NonSerialized] public bool onMove = false;
    public Action<bool> moveCallback;

    protected Transform trans;

    AngleMove() { }
    public AngleMove(Transform t) { trans = t; }

    public void Start(Vector3 pos, Vector3 dir)
    {
        start_pos = pos;
        move_dir = dir;
        onMove = true;
        if (null != moveCallback) moveCallback(true);
    }

    public void Stop(Vector3 pos)
    {
        trans.position = pos;
        onMove = false;
        if (null != moveCallback) moveCallback(false);
    }
}

public class RotateParam
{
    [System.NonSerialized] public Quaternion qTarget;
    [System.NonSerialized] public Quaternion qInverse;
    [System.NonSerialized] public bool onRotate;

    public void Start(Quaternion _target)
    {
        qTarget = _target;
        qInverse = Quaternion.Inverse(qTarget);
        onRotate = true;
    }

    public void Stop()
    {
        onRotate = false;
    }

    public float GetEularY(Quaternion cur)
    {
        Quaternion q = cur * qInverse;
        return q.eulerAngles.y;
    }

    public void Lerp(Transform trans, float dt)
    {
        trans.rotation = Quaternion.Lerp(trans.rotation, qTarget, dt * 10);
    }
}

