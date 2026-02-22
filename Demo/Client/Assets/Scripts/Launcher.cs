using UnityEditor;
using UnityEngine;
using XEngine;
using XGame;

public class Launcher : ILauncher<Launcher>
{
    public static T GetModule<T>() where T : IModule
    {
        return Instance.GetM<T>();
    }
    public static bool RegistModule<I, M>()
        where I : IModule
        where M : I, new()
    {
        return Instance.RegistM<I, M>();
    }

    protected override void Initialize()
    {
        Debug.Log("Launcher Initialize");
        base.Initialize();
    }

    public static void Release()
    {
        Instance.ReleaseM();
    }
}

#if UNITY_EDITOR
[InitializeOnLoad]
public static class EditorClearModule
{
    static EditorClearModule()
    {
        // 注册播放模式状态改变的回调
        EditorApplication.playModeStateChanged += OnPlayModeStateChanged;
    }

    private static void OnPlayModeStateChanged(PlayModeStateChange state)
    {
        if (state == PlayModeStateChange.ExitingPlayMode || state == PlayModeStateChange.EnteredEditMode)
        {
            Launcher.Release();
        }
    }
}
#endif //UNITY_EDITOR
