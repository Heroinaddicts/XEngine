using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIRole : MonoBehaviour
{
    public Text Name;

    public GameObject Role;

    public void SetName(in string name)
    {
        Name.text = name;
    }
}
