using AutoConfig;
using GameUnit;
using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using XEngine;
using XGame;
using XGameClient;
using static GameUnitApi;

public interface IAllTests : IModule
{
}

public class AllTests : IAllTests
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IAllTests, AllTests>();
    }

    LineRenderer _FindPathTest;

    public bool Initialize()
    {
        return true;
    }

    IClient _Client;

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        _Client.RegistProtobuf<oFindPathTest>(eServerID.FindPathTest, OnFindPathTest);
        _Client.RegistProtobuf<oRaycastTest>(eServerID.RaycastTest, OnRaycastTest);

        GameObject go = new GameObject("FindPathTest");
        GameObject.DontDestroyOnLoad(go);

        _FindPathTest = go.AddComponent<LineRenderer>();
        _FindPathTest.startWidth = 0.1f;
        _FindPathTest.endWidth = 0.1f;
        _FindPathTest.sortingOrder = 1000;
        return true;
    }

    public void Release()
    {

    }

    void OnFindPathTest(oFindPathTest test)
    {
        _FindPathTest.positionCount = test.Positions.Count;
        for (int i = 0; i < test.Positions.Count; i++)
        {
            _FindPathTest.SetPosition(i, new Vector2(test.Positions[i].X, test.Positions[i].Y));
        }
    }

    void OnRaycastTest(oRaycastTest test)
    {
        _FindPathTest.positionCount = 2;
        _FindPathTest.SetPosition(0, new Vector2(test.Start.X, test.Start.Y));
        _FindPathTest.SetPosition(1, new Vector2(test.End.X, test.End.Y));
    }
}
