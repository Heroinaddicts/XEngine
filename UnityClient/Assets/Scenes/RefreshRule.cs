using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RefreshRule : MonoBehaviour
{
    [SerializeField]
    [Header("刷新规则ID")]
    public int ConfigID;

    [SerializeField]
    [Header("规则描述")]
    public string Desc;
}
