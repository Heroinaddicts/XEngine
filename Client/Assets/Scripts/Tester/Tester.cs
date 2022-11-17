using System.Collections;
using System.Collections.Generic;
using TCore.Api;
using UnityEngine;
using UnityEngine.Tilemaps;

public class Tester : MonoBehaviour, iTimer
{
    Tilemap tm;

    public void OnEnd(in int id, in int index, in int offset, in object context, in bool natural)
    {
        throw new System.NotImplementedException();
    }

    public void OnStart(in int id, in int index, in int offset, in object context)
    {
        throw new System.NotImplementedException();
    }

    public void OnTime(in int id, in int index, in int offset, in object context)
    {
        throw new System.NotImplementedException();
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            Vector2 pos = Input.mousePosition;
            Debug.Log(pos);
        }
    }
}
