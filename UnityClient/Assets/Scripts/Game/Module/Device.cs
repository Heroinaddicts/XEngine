using System;
using System.Collections;
using System.Collections.Generic;
using System.Net.NetworkInformation;
using TCore.Api;
using UnityEngine;

public class Device : iModule
{
    string _identifier;

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void ModuleRegister()
    {
        Module.Registe(typeof(Device), new Device());
    }


    Device() : base("Device") {}
    public override bool Initialize()
    {
        NetworkInterface[] nis = NetworkInterface.GetAllNetworkInterfaces();
        if (nis.Length <= 0)
        {
            return false;
        }

        _identifier = nis[0].GetPhysicalAddress().ToString();
        return true;
    }

    public override bool Launch()
    {
        return true;
    }

    public override void Release()
    {

    }

    public string GetDeviceIdentifier()
    {
        return _identifier;
    }
}
