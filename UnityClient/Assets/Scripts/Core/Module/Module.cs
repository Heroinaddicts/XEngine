using System;
using System.Collections;
using System.Collections.Generic;
using TCore.Api;
using UnityEngine;

public static class Module 
{
    static IDictionary<Type, iModule> s_modules = new Dictionary<Type, iModule>();

    public static bool Registe(in Type type, in iModule module)
    {
        if (s_modules.ContainsKey(type))
        {
            Debug.LogError("module " + module._name + " already exists");
            return false;
        }

        Debug.Log("module " + module._name + " registe success");
        s_modules.Add(type, module);
        return true;
    }

    public static T GetModule<T>() where T : iModule
    {
        iModule module;
        if (s_modules.TryGetValue(typeof(T), out module))
        {
            return module as T;
        }
        return null;
    }

    public static bool Lanuch()
    {
        foreach (KeyValuePair<Type, iModule> kv in s_modules)
        {
            if (!kv.Value.Initialize())
            {
                Debug.LogError("module " + kv.Value._name + " Initiazlie error");
                return false;
            }

            Debug.Log("module " + kv.Value._name + " Initiazlie successed");
        }

        foreach (KeyValuePair<Type, iModule> kv in s_modules)
        {
            if (!kv.Value.Launch())
            {
                Debug.LogError("module " + kv.Value._name + " Launch error");
                return false;
            }

            Debug.Log("module " + kv.Value._name + " Launch successed");
        }

        return true;
    }
}
