using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using TCore.Api;
using UnityEngine;

public class Config : iModule
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void ModuleRegister()
    {
        Module.Registe(typeof(Config), new Config());
    }

    Config() : base("Config") {}

    AutoConfig _config;
    public override bool Initialize()
    {
        _config = new AutoConfig("XmlConfig", XmlLoad);
        return true;
    }

    public XmlDocument XmlLoad(string path, string name)
    {
        TextAsset text_asset = Resources.Load(path + "/" + name) as TextAsset;
        XmlReader reader = XmlReader.Create(new StringReader(text_asset.text));
        XmlDocument doc = new XmlDocument();
        try
        {
            doc.Load(reader);
        }
        catch (IOException ex)
        {
            Debug.LogError("load config error " + path + "/" + name);
        }
        return doc;
    }

    public override bool Launch()
    {
        return true;
    }

    public override void Release()
    {

    }

    public AutoConfig GetConfig()
    {
        return _config;
    }
}
