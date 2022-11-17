using System.Collections;
using System.Collections.Generic;
using TNet;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Core : Singleton<Core>
{
    static bool _pause = false;
    public static void Pause()
    {
        _pause = true;
        NetWorker._pause = true;
    }

    public static void Resume()
    {
        _pause = false;
        NetWorker._pause = false;
    }

    private void OnEnable()
    {
        //Core.Resume();
    }
    private void OnDisable()
    {
        //Core.Pause();
    }

    public void LoadScene(in string name)
    {
        Core.Pause();
        SceneManager.LoadScene(name);
        SceneManager.sceneLoaded += (Scene scene, LoadSceneMode sceneType) =>
        {
            Core.Resume();
        };
        //Core.Resume();
    }

    // Start is called before the first frame update
    void Start()
    {
        //DontDestroyOnLoad(this);
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if (Core._pause)
        {
            return;
        }
        NetWorker.Loop();
    }
}
