using System.Collections.Generic;
using UnityEngine;

abstract class GameObjectPool : MonoBehaviour
{
    List<GameObject> _Pools = new List<GameObject>();

    private void Awake()
    {
        for (int i = 0; i < GetInitSize(); i++)
        {
            GameObject go = this.CreateGameObject();
            go.transform.parent = transform;
            go.gameObject.SetActive(false);
            _Pools.Add(go);
        }
    }

    public virtual int GetInitSize() { return 100; }
    public abstract GameObject CreateGameObject();

    public GameObject Pop()
    {
        if (_Pools.Count == 0)
        {
            return CreateGameObject();
        }

        GameObject go = _Pools[_Pools.Count - 1];
        _Pools.RemoveAt(_Pools.Count - 1);
        go.gameObject.SetActive(true);
        return go;
    }

    public void Push(GameObject go)
    {
        _Pools.Add(go);
        go.transform.parent = this.transform;
        go.gameObject.SetActive(false);
    }
}
