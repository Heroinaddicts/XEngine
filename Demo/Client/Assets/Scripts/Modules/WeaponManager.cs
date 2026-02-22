using System;
using AutoConfig;
using UnityEngine;
using XEngine;
using XGame;
using XUtils;
using static GameUnitApi;

public interface IWeaponManager : IModule
{
    public void CharacterChangeWeapon(Int64 weaponID, IUnit changerPlayer);

}

class WeaponManager : IWeaponManager
{

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IWeaponManager, WeaponManager>();
    }

    IClient _Client;


    public bool Initialize()
    {
        return true;
    }

    public bool Launch()
    {
        _Client = Launcher.GetModule<IClient>();
        return true;
    }

    public void Release()
    {

    }

    public void CharacterChangeWeapon(Int64 weaponID, IUnit changerPlayer)
    {
        GameObject weaponItem = null;
        if (weaponID  != AutoConfig.Equipment.INVALID_CONFIG_ID)
        {
            weaponItem = Resources.Load<GameObject>(AutoConfig.Equipment.Get(weaponID)._AppearPrefab);
        }
        else if (weaponID == AutoConfig.Equipment.INVALID_CONFIG_ID)
        {
            weaponItem = Resources.Load<GameObject>(Equipment.Get(AutoConfig.Character.Get(changerPlayer.ConfigID)._InitialWeapon)._AppearPrefab);
        }
        GameObject playerItem = changerPlayer.transform.Find("SkeletonUtility-SkeletonRoot/root/body/spine_2/spine_1/spine_3/arm_R/forearm_R/hand_R/weapon").gameObject;
        for (int i = 0; i < playerItem.transform.childCount; i++)
        {
            Transform child = playerItem.transform.GetChild(i);
            GameObject.Destroy(child.gameObject);
        }
        if (weaponItem != null && playerItem != null)
        {
            GameObject weaponIns = GameObject.Instantiate(weaponItem);
            weaponIns.transform.SetParent(playerItem.transform);
            weaponIns.transform.localPosition = Vector3.zero;

            Spine.Unity.SkeletonAnimation skeletonAnimation = changerPlayer.gameObject.GetComponent<Spine.Unity.SkeletonAnimation>();
            float rotationZ = 200f;
            if (skeletonAnimation.skeleton.ScaleX < 0)
            {
                rotationZ = 290f;
            }
            weaponIns.transform.localRotation = Quaternion.Euler(0, 0, rotationZ);
            weaponIns.GetComponent<SpriteRenderer>().sortingOrder = 10;
        }
    }
}