using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameApp
{
    protected static GameApp instance;
    protected IScene curScene;

    public static GameApp Instance
    {
        get
        {
            if (instance == null)
            {
                instance = new GameApp();
            }
            return instance;
        }
        set
        {
            return;
        }
    }

    public IScene CurScene
    {
        get
        {
            return curScene;
        }
        set
        {
            curScene = value;
        }
    }

    public BaseCamera CurSceneCamera { get; set; }
}
