using System;
using System.Collections.Generic;
using System.Linq;
using UnityEditor;
using UnityEngine;
using XEngine;
using XGame;
using XUtils;

public abstract class ILauncher<T> : MonoSingleton<T> where T : MonoBehaviour
{
    static Dictionary<Type, IModule> s_Modules = new Dictionary<Type, IModule>();
    static KeyValuePair<Type, IModule>[] s_ModulesArray = null;
    public static void LaunchModule()
    {
        foreach (var kv in s_Modules)
        {
            if (!kv.Value.Initialize())
            {
                XApi.Error($"{kv.Key.Name} Initialize error");
                return;
            }

            XApi.Trace($"{kv.Key.Name} Initialized");
        }

        foreach (var kv in s_Modules)
        {
            if (!kv.Value.Launch())
            {
                XApi.Error($"{kv.Key.Name} Launch error");
                return;
            }
            XApi.Trace($"{kv.Key.Name} Launched");
        }
    }

    protected override void Initialize()
    {
        LaunchModule();
    }

    public bool RegistM<I, M>()
        where I : IModule
        where M : I, new()
    {
        if (s_Modules.ContainsKey(typeof(I)))
        {
            return false;
        }

        s_Modules.Add(typeof(I), new M());
        return true;
    }

    public M GetM<M>() where M : IModule
    {
        return (M)s_Modules[typeof(M)];
    }

    public void ReleaseM()
    {
        foreach (var kv in s_Modules)
        {
            kv.Value.Release();
            XApi.Trace($"{kv.Key.Name} Released");
        }
        s_Modules.Clear();
        DestroyImmediate(Instance.gameObject);
    }

    public void Update()
    {
        if (null == s_ModulesArray)
            s_ModulesArray = s_Modules.ToArray();

        for (int i = 0; i < s_ModulesArray.Length; i++)
        {
            s_ModulesArray[i].Value.Update();
        }

        XApi.Update();
    }

    public void FixedUpdate()
    {
        if (null == s_ModulesArray)
            s_ModulesArray = s_Modules.ToArray();

        for (int i = 0; i < s_ModulesArray.Length; i++)
        {
            s_ModulesArray[i].Value.FixedUpdate();
        }

        XApi.FixedUpdate();
    }

    public void LateUpdate()
    {
        if (null == s_ModulesArray)
            s_ModulesArray = s_Modules.ToArray();

        for (int i = 0; i < s_ModulesArray.Length; i++)
        {
            s_ModulesArray[i].Value.LateUpdate();
        }
    }
}

