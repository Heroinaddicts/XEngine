using HttpDef;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public class GameScene : IScene
{
    public Camera MainCamera;

    AutoConfig _config;
    protected override void OnResponse(in eRequestID rid, in bool isSuccess, in string data)
    {

    }

    protected override void __Awake()
    {
        _config = Module.GetModule<Config>().GetConfig();
    }

    protected override void __OnEnable()
    {

    }

    protected override void __OnDisable()
    {

    }

    protected override void __Start()
    {

    }

    protected override void __OnUpdate()
    {
        List<Vector3> positions = new List<Vector3>();
        float distance = 0;
        for (int i = 1; i < positions.Count; i++)
        {
            float x = positions[i].x - positions[i - 1].x;
            float y = positions[i].y - positions[i - 1].y;
            float z = positions[i].z - positions[i - 1].z;

            distance += (float)Math.Sqrt(x * x + y * y + z * z);
        }
    }
}
