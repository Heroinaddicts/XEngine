using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerControl : MonoBehaviour
{
    public Animator animator;
    public void SetMove(bool _move)
    {
        SetSpeed(_move ? 0.3f : 0);
    }

    public void SetWeapon(WEAPON_CATEGORY weapon)
    {
        animator.SetInteger("WeaponType_int", (int)weapon);
    }

    public void SetAutoMode(bool _fire)
    {
        animator.SetBool("FullAuto_b", _fire);
    }

    public void SetFire(bool _fire)
    {
        animator.SetBool("Shoot_b", _fire);
    }

    void SetSpeed(float _speed)
    {
        animator.SetFloat("Speed_f", _speed);
    }

}
