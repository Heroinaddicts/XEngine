using AutoConfig;
using GameUnit;
using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.Tilemaps;
using XEngine;
using XGame;
using XGameClient;
using static GameUnitApi;

public interface IMovement : IModule
{
    public delegate void fArrived(in bool arrived);
    void MainCharacterMoveto(in Vector2 pos, fArrived ret);
    void StopMainCharacterMoveto();

    bool CanSlide(IUnit unit, IGameTilemap tilemap, in Vector2 delta, ref Vector2 pos);
}

public class Movement : IMovement
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IMovement, Movement>();
    }

    public bool Initialize()
    {
        return true;
    }

    IFiniteStateMachine _FiniteStateMachine;
    IGameSceneManager _GameScene;
    ISpineAnimationController _SpineAnimationController;
    ICharacterManager _CharacterManager;
    IGameSceneManager _GameSceneManager;
    IClient _Client;

    public bool Launch()
    {
        _FiniteStateMachine = Launcher.GetModule<IFiniteStateMachine>();
        _GameScene = Launcher.GetModule<IGameSceneManager>();
        _SpineAnimationController = Launcher.GetModule<ISpineAnimationController>();
        _CharacterManager = Launcher.GetModule<ICharacterManager>();
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();
        _Client = Launcher.GetModule<IClient>();

        _FiniteStateMachine.RegistStateEnter(AutoEnum.eState.Idle, OnEnterIdle);
        _FiniteStateMachine.RegistStateEnter(AutoEnum.eState.Moving, OnEnterMoving);
        _FiniteStateMachine.RegistStateExit(AutoEnum.eState.Moving, OnExitMoving);
        _FiniteStateMachine.RegistStateEnter(AutoEnum.eState.Stuned, OnEnterStuned);

        return true;
    }

    public void Release()
    {

    }
    static void GetBestSlideEdges(Vector2 delta, out int first, out int second)
    {
        delta.Normalize();

        float best = -1f;
        float secondBest = -1f;
        first = -1;
        second = -1;

        for (int i = 0; i < 4; i++)
        {
            float rad = AutoConfig.AutoConst.RhombusMap.RhombusAngles[i] * Mathf.Deg2Rad;
            Vector2 edgeDir = new Vector2(Mathf.Cos(rad), Mathf.Sin(rad));

            float dot = Vector2.Dot(delta, edgeDir);
            if (dot > best)
            {
                secondBest = best;
                second = first;
                best = dot;
                first = i;
            }
            else if (dot > secondBest)
            {
                secondBest = dot;
                second = i;
            }
        }
    }

    public bool CanSlide(IUnit unit, IGameTilemap tilemap, in Vector2 delta, ref Vector2 pos)
    {
        int edgeA, edgeB;
        GetBestSlideEdges(delta, out edgeA, out edgeB);

        // ��һ�����߷���
        {
            float rad = AutoConfig.AutoConst.RhombusMap.RhombusAngles[edgeA] * Mathf.Deg2Rad;
            Vector2 slideDir = new Vector2(Mathf.Cos(rad), Mathf.Sin(rad));
            Vector2 slidePos = unit.Position + slideDir * unit.GetFloat(eEntityAttr.Speed_Float) * Time.deltaTime;

            if (tilemap.CanMove(slidePos))
            {
                pos = slidePos;
                return true;
            }
        }

        {
            float rad = AutoConfig.AutoConst.RhombusMap.RhombusAngles[edgeB] * Mathf.Deg2Rad;
            Vector2 slideDir = new Vector2(Mathf.Cos(rad), Mathf.Sin(rad));
            Vector2 slidePos = unit.Position + slideDir * unit.GetFloat(eEntityAttr.Speed_Float) * Time.deltaTime;

            if (tilemap.CanMove(slidePos))
            {
                pos = slidePos;
                return true;
            }
        }

        return false;
    }

    void UpdatePostion(GameUnitApi.IUnit unit)
    {
        IGameTilemap tilemap = _GameScene.GetCurrentTilemap();
        bool ret = false;
        if (tilemap != null)
        {
            float angleInRadians = unit.Angle * Mathf.Deg2Rad;
            Vector2 direction = new Vector2(Mathf.Cos(angleInRadians), Mathf.Sin(angleInRadians));
            Vector2 delta = direction * unit.GetFloat(eEntityAttr.Speed_Float) * Time.deltaTime;
            Vector2 newPosition = unit.Position + delta;
            if (tilemap.CanMove(newPosition))
            {
                unit.Position = newPosition;
                ret = true;
            }
            else
            {
                Vector2 pos = new Vector2();
                if (CanSlide(unit, tilemap, delta, ref pos))
                {
                    unit.Position = pos;
                    ret = true;
                }
            }
        }

        if (false == ret)
        {
            _FiniteStateMachine.EnterState(unit.Guid, AutoEnum.eState.Idle, unit.Angle, unit.Position, true);
        }
    }

    void OnEnterIdle(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        GameUnitApi.IUnit unit = GameUnitApi.QueryUnit(guid);
        if (null == unit)
        {
            XApi.Error($"Where is unit {guid}");
            throw new Exception($"Where is unit {guid}");
        }

        unit.Position = pos;
        unit.Angle = angle;
        _SpineAnimationController.HandleAnimation(unit, "idle", true);
        _SpineAnimationController.FlipCharacterToDirection(unit, angle);
    }

    void OnEnterMoving(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        GameUnitApi.IUnit unit = GameUnitApi.QueryUnit(guid);
        if (null == unit)
        {
            XApi.Error($"Where is unit {guid}");
            throw new Exception($"Where is unit {guid}");
        }

        if (_CharacterManager.GetMainCharacterUnit() != null && unit.Guid == _CharacterManager.GetMainCharacterUnit().Guid)
        {
            if ((unit.Position - pos).magnitude > AutoConfig.AutoConst.AllowableError.PositionErrorRange)
            {
                unit.Position = pos;
            }
        }
        else
        {
            unit.Position = pos;
        }

        unit.Angle = angle;
        XApi.Debug($"OnEnterMoving Guid:{guid} Pos:{unit.Position} Angle:{angle}");
        unit.RegistUpdate(UpdatePostion);

        _SpineAnimationController.HandleAnimation(unit, "run", true);
        _SpineAnimationController.FlipCharacterToDirection(unit, angle);
    }

    void OnExitMoving(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        GameUnitApi.IUnit unit = GameUnitApi.QueryUnit(guid);
        if (null == unit)
        {
            XApi.Error($"Where is unit {guid}");
            throw new Exception($"Where is unit {guid}");
        }
        unit.CancelRegistUpdate(UpdatePostion);
        unit.Position = pos;
        unit.Angle = angle;
    }

    void OnEnterStuned(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        GameUnitApi.IUnit unit = GameUnitApi.QueryUnit(guid);
        if (null == unit)
        {
            XApi.Error($"Where is unit {guid}");
            throw new Exception($"Where is unit {guid}");
        }
        unit.Position = pos;
        unit.Angle = angle;
        //_SpineAnimationController.HandleAnimation(unit, "Entangle", true);
        _SpineAnimationController.FlipCharacterToDirection(unit, angle);
    }

    List<Vector2> _Path = null;
    IMovement.fArrived _Ret = null;

    void ToNextCorner(in XApi.ITimer.eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)
    {
        switch (state)
        {
            case XApi.ITimer.eState.END:
                {
                    if (murder)
                    {
                        _Ret?.Invoke(false);
                        _Ret = null;
                        _Path = null;
                        return;
                    }

                    IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
                    if (null == mainCharacter)
                    {
                        XApi.TimerApi.StopTimer(ToNextCorner, this);
                        _Ret?.Invoke(false);
                        _Ret = null;
                        _Path = null;
                        return;
                    }

                    mainCharacter.Position = _Path[0];
                    _Path.RemoveAt(0);
                    if (_Path.Count > 0)
                    {
                        Vector2 dir = _Path[0] - mainCharacter.Position;
                        float speed = mainCharacter.GetFloat(eCharacterAttr.Speed_Float);
                        float distance = dir.magnitude;
                        mainCharacter.Angle = (Mathf.Atan2(dir.y, dir.x) * Mathf.Rad2Deg + 360f) % 360f;
                        if (_FiniteStateMachine.EnterState(mainCharacter.Guid, AutoConfig.AutoEnum.eState.Moving, mainCharacter.Angle, mainCharacter.Position))
                        {
                            oStateChange change = new oStateChange();
                            change.Angle = mainCharacter.Angle;
                            change.State = (int)AutoConfig.AutoEnum.eState.Moving;
                            change.Pos = new oVector2();
                            change.Pos.X = mainCharacter.Position.x;
                            change.Pos.Y = mainCharacter.Position.y;
                            _Client.SendProtobuf(eClientID.StateChange, change);
                            XApi.TimerApi.StartTimer(ToNextCorner, this, this, (int)((distance / speed) * 1000), 1, 100);
                        }
                        else
                        {
                            _Ret?.Invoke(false);
                            _Ret = null;
                            _Path = null;
                        }
                    }
                    else
                    {
                        _Ret?.Invoke(true);
                        _Ret = null;
                        _Path = null;

                        if (_FiniteStateMachine.EnterState(mainCharacter.Guid, AutoConfig.AutoEnum.eState.Idle, mainCharacter.Angle, mainCharacter.Position))
                        {
                            oStateChange change = new oStateChange();
                            change.Angle = mainCharacter.Angle;
                            change.State = (int)AutoConfig.AutoEnum.eState.Idle;
                            change.Pos = new oVector2();
                            change.Pos.X = mainCharacter.Position.x;
                            change.Pos.Y = mainCharacter.Position.y;

                            _Client.SendProtobuf(eClientID.StateChange, change);
                        }
                    }
                    break;
                }
        }
    }

    public void MainCharacterMoveto(in Vector2 pos, IMovement.fArrived ret)
    {
        _Ret?.Invoke(false);
        _Ret = null;
        _Path = null;
        XApi.TimerApi.StopTimer(ToNextCorner, this);

        IUnit mainCharacter = _CharacterManager.GetMainCharacterUnit();
        if (null == mainCharacter)
        {
            ret?.Invoke(false);
            return;
        }

        _GameSceneManager.GetCurrentTilemap().FindPath(mainCharacter.Position, pos, (List<Vector2> path) =>
        {
            if (path.Count > 0)
            {
                _Path = path.ToList<Vector2>();
                _Ret = ret;
                float speed = mainCharacter.GetFloat(eCharacterAttr.Speed_Float);

                Vector2 dir = path[0] - mainCharacter.Position;
                float distance = dir.magnitude;
                mainCharacter.Angle = (Mathf.Atan2(dir.y, dir.x) * Mathf.Rad2Deg + 360f) % 360f;

                if (_FiniteStateMachine.EnterState(mainCharacter.Guid, AutoConfig.AutoEnum.eState.Moving, mainCharacter.Angle, mainCharacter.Position))
                {
                    oStateChange change = new oStateChange();
                    change.Angle = mainCharacter.Angle;
                    change.State = (int)AutoConfig.AutoEnum.eState.Moving;
                    change.Pos = new oVector2();
                    change.Pos.X = mainCharacter.Position.x;
                    change.Pos.Y = mainCharacter.Position.y;
                    _Client.SendProtobuf(eClientID.StateChange, change);
                    XApi.TimerApi.StartTimer(ToNextCorner, this, this, (int)((distance / speed) * 1000), 1, 100);
                }
                else
                {
                    ret?.Invoke(false);
                }
            }
            else
            {
                ret?.Invoke(false);
            }
        });


    }

    public void StopMainCharacterMoveto()
    {
        XApi.TimerApi.StopTimer(ToNextCorner, this);
        _Ret?.Invoke(false);
        _Ret = null;
        _Path = null;
    }
}
