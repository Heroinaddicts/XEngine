using UnityEngine;
using System.Collections;

public class Singleton<T> : MonoBehaviour where T : MonoBehaviour
{
    private static T _instance;
    private static object _lock = new object();
    public static T Instance
    {
        get
        {
            if (applicationIsQuitting)
            {
#if UNITY_EDITOR
                    Debug.LogWarning("[Singleton] Instance " + typeof(T) +
                        " already destroyed on application quit." +
                        "Won't create again - returning null.");
#endif
                return null;
            }
            lock (_lock)
            {
                if (_instance == null)
                {
                    _instance = (T)FindObjectOfType(typeof(T));
                    if (_instance == null)
                    {
                        GameObject singleton = new GameObject();
                        _instance = singleton.AddComponent<T>();
                        singleton.name = "(singleton) " + typeof(T).ToString();

                        //GameObject singleton = Instantiate(Resources.Load(typeof(T).ToString())) as GameObject;
                        _instance = singleton.GetComponent<T>();
                        (_instance as Singleton<T>).Init();
                        singleton.name = "(singleton) " + typeof(T).ToString();
                        DontDestroyOnLoad(singleton);
#if UNITY_EDITOR
                            Debug.Log("[Singleton] An instance of " + typeof(T) +
                                " is needed in the scene, so '" + singleton +
                                "' was created with DontDestroyOnLoad.");
#endif
                    }
                    else
                    {
#if UNITY_EDITOR
                            Debug.Log("[Singleton] Using instance already created: " + _instance.gameObject.name);
#endif
                    }
                }
                return _instance;
            }
        }
    }

    private static bool applicationIsQuitting = false;
    /// <summary>
    /// When unity quits, it destroys objects in a random order.
    /// In principle, a Singleton is only destroyed when application quits.    
    /// If any script calls Instance after it have been destroyed,     
    ///   it will create a buggy ghost object that will stay on the Editor scene    
    ///   even after stopping playing the Application. Really bad!    
    /// So, this was made to be sure we're not creating that buggy ghost object.    
    /// </summary>
    public void OnDestroy()
    {
        if (ClearMode == false)
            applicationIsQuitting = true;
        ClearMode = false;
    }

    public virtual void Init()
    {

    }

    private void Awake()
    {
        if(_instance == null)
        {
            _instance = this as T;
        }
        DontDestroyOnLoad(this.gameObject);
    }

    private static bool ClearMode = false;
    public static void DestorySingleton()
    {
        lock (_lock)
        {
            if (_instance != null)
            {
                ClearMode = true;
                GameObject.Destroy(_instance.gameObject);
                _instance = null;
            }
        }
    }
}
