using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MapInterface : MonoBehaviour
{
    public GameObject GameRoles;
    // Start is called before the first frame update
    public void AddRole(GameObject role)
    {
        if (role)
        {
            role.transform.parent = GameRoles.transform;
        }
    }
}
