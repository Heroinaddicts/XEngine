using GameUnit;
using System;
using UnityEngine;
using XEngine;
using XGame;
using static GameUnitApi;

public interface ILevelManager : IModule
{

}
public class LevelManager : ILevelManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<ILevelManager, LevelManager>();
    }

    public bool Initialize()
    {
        GameUnitApi.RegistUnitEvent(GameUnit.eUnitType.Character, OnCharacterUnitEvent);

        return true;
    }

    public bool Launch()
    {
        return true;
    }

    public void Release()
    {

    }

    void OnCharacterUnitEvent(in eUnitEvent ev, IUnit unit, in bool isParentType, in string file, in int line)
    {
        switch(ev)
        {
            case eUnitEvent.Create:
                {
                    unit.RegistInt64AttributeChange(eCharacterAttr.Level_Int64, eUnitAttributeEvent.Changed, CharacterLevelChange);
                    break;
                }
        }
    }

    void OnEffectReleaseTimer(in XApi.ITimer.eState state, in int count, in Int64 tick, in object obj, in object context, in bool murder)
    {
        if (XApi.ITimer.eState.END == state) 
        {
            GameObject t = obj as GameObject;
            GameObject.Destroy(t);
        }
    }

    bool CharacterLevelChange(in eUnitAttributeEvent ev, in IUnit unit, in int index, in Int64 oldValue, in Int64 newValue)
    {
        AutoConfig.CharacterLevel config = AutoConfig.CharacterLevel.Get(oldValue);
        if (null != config)
        {
            AutoConfig.SpecialEffects effect = AutoConfig.SpecialEffects.Get(config._SpecialEffect);
            if (null != effect)
            {
                GameObject o = Resources.Load<GameObject>(effect._Prefab);
                if (null != o)
                {
                    GameObject t = GameObject.Instantiate(o);
                    t.transform.parent = unit.gameObject.transform;
                    t.transform.position = Vector3.zero;
                    XApi.TimerApi.StartTimer(OnEffectReleaseTimer, t, null, (int)effect._Time, 1, (int)effect._Time);
                }
            }
        }

        return true;
    }
}
