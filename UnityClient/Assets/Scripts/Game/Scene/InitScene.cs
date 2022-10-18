using HttpDef;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class InitScene : IScene
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Initialize()
    {
#if UNITY_EDITOR
//         Scene scene = SceneManager.GetActiveScene();
//         if (scene != null && (scene.name.Equals("InitScene")))
//         {
//             return;
//         }
//         SceneManager.LoadScene("InitScene");
#endif //UNITY_EDITOR
    }

    bool _isInited = false;
    protected override void __Awake()
    {
        
    }

    protected override void __Start()
    {
        if (Module.Lanuch())
        {
            SceneManager.LoadScene("LoginScene");
        }
    }

    protected override void __OnEnable()
    {

    }

    protected override void __OnDisable()
    {

    }

    protected override void __OnUpdate()
    {
    }

    protected override void OnResponse(in eRequestID rid, in bool isSuccess, in string data)
    {
    }
}
