
using AutoConfig;
using Spine.Unity;
using System;
using System.Linq;
using UnityEngine;
using XGame;
using XGameClient;
using static GameUnitApi;

public interface IDeath : IModule
{
}

class Death : IDeath
{

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IDeath, Death>();
    }

    IFiniteStateMachine _FSM;
    ISpineAnimationController _SAC;
    IAudioManager _AudioManager;
    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _FSM = Launcher.GetModule<IFiniteStateMachine>();
        _FSM.RegistStateEnter(AutoConfig.AutoEnum.eState.Death, OnEnterDeath);

        _AudioManager = Launcher.GetModule<IAudioManager>();
        _SAC = Launcher.GetModule<ISpineAnimationController>();
        return true;
    }

    public void Release()
    {

    }

    void OnEnterDeath(in UInt64 guid, in AutoConfig.AutoEnum.eState state, in float angle, in Vector2 pos)
    {
        IUnit go = GameUnitApi.QueryUnit(guid);
        if (go != null)
        {

            switch (go.Type)
            {
                case GameUnit.eUnitType.Monster:
                    {
                        AutoConfig.Monster config = AutoConfig.Monster.Get(go.ConfigID);
                        _SAC.HandleAnimation(go, config._DeadAct, false);
                        _AudioManager.PlaySound(AutoConfig.Sound.Get(AutoConfig.Monster.Get(go.ConfigID)._SoundDie)._AudioMaterial);
                        break;
                    }
                case GameUnit.eUnitType.Character:
                    {
                        _SAC.HandleAnimation(go, AutoConfig.Character.Get(go.ConfigID)._DeadAct, false);
                        _AudioManager.PlaySound(AutoConfig.Sound.Get(AutoConfig.Character.Get(go.ConfigID)._DeadSound)._AudioMaterial);
                        break;
                    }
                case GameUnit.eUnitType.Boss:
                    {
                        _AudioManager.PlaySound(AutoConfig.Sound.Get(AutoConfig.MonsterBoss.Get(go.ConfigID)._SoundDie)._AudioMaterial);
                        if (AutoConfig.MonsterBoss.Get(go.ConfigID)._ConcludePartDie != null && AutoConfig.MonsterBoss.Get(go.ConfigID)._ConcludePartDie.Any())
                        {
                            string[] concludePartDie = AutoConfig.MonsterBoss.Get(go.ConfigID)._ConcludePartDie;
                            string[] dieAct = AutoConfig.MonsterBoss.Get(go.ConfigID)._PartDieAct;

                            for (int i = 0; i < concludePartDie.Length; i++)
                            {
                                GameObject part = go.transform.Find(concludePartDie[i]).gameObject;
                                SkeletonAnimation skeletonAnim = part.GetComponent<SkeletonAnimation>();
                                if (part != null)
                                {
                                    skeletonAnim.AnimationState.SetAnimation(0, dieAct[i], false);

                                    //_SAC.HandleAnimation(unitPart, dieAct[i], false);
                                    //Debug.LogError($"Boss 部位死亡动画 播放 部位:{concludePartDie[i]} 动画:{dieAct[i]}");
                                }
                            }
                        }
                        else
                        {
                            _SAC.HandleAnimation(go, AutoConfig.MonsterBoss.Get(go.ConfigID)._DeadAct, false);
                        }
                        break;
                    }

            }
        }
    }
}
