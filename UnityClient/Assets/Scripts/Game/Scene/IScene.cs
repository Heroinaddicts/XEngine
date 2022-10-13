using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public abstract class IScene : HttpMonoBehaviour
{
    protected abstract void __Awake();
    protected abstract void __Start();
    protected abstract void __OnEnable();
    protected abstract void __OnDisable();
    protected abstract void __OnUpdate();

    protected AsyncOperation loadAsyncOperation;

    private void Awake()
    {
        __Awake();
    }

    private void Start()
    {
        __Start();
    }

    private void OnEnable()
    {
        Core.Resume();
        GameApp.Instance.CurScene = this;
        __OnEnable();
    }

    private void OnDisable()
    {
        __OnDisable();
    }

    void Update()
    {
        if(loadAsyncOperation != null)
        {
            //Debug.Log(loadAsyncOperation.progress + "  "+ loadAsyncOperation.isDone);
            if(loadAsyncOperation.progress >= .9f)
            {
                loadAsyncOperation.allowSceneActivation = true;
            }
        }
        __OnUpdate();
    }
}
