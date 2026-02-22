using UnityEngine;
using XEngine;

public abstract class IScene<T> : MonoBehaviour where T : IScene<T>
{
    T _Scene;

    void Awake()
    {
        _Scene = this as T;
        XApi.Debug($"Scene {_Scene.GetType().Name} Awake");
    }

    void Start()
    {
        XApi.Debug($"Scene {_Scene.GetType().Name} Start");
    }

    void Update()
    {
        
    }
}
