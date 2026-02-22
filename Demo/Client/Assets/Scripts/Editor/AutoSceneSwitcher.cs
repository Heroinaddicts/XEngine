using UnityEngine;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;
using XEngine;

[InitializeOnLoad]
public static class AutoSceneSwitcher
{
    private static string _PreviousScenePath;
    private const string INITIALIZE_SCENE_NAME = "InitializeScene";

    static AutoSceneSwitcher()
    {
        EditorApplication.playModeStateChanged += OnPlayModeStateChanged;
    }

    private static void OnPlayModeStateChanged(PlayModeStateChange state)
    {
        switch (state)
        {
            case PlayModeStateChange.EnteredPlayMode:
                EnterPlayModeHandler();
                break;

            case PlayModeStateChange.ExitingPlayMode:
                ExitPlayModeHandler();
                break;
        }
    }

    private static void EnterPlayModeHandler()
    {
        Scene currentScene = SceneManager.GetActiveScene();
        _PreviousScenePath = currentScene.path;

        string initializeScenePath = FindScenePath(INITIALIZE_SCENE_NAME);

        if (!string.IsNullOrEmpty(initializeScenePath))
        {
            SceneManager.LoadScene(initializeScenePath, LoadSceneMode.Single);
        }
    }

    private static void ExitPlayModeHandler()
    {
        XApi.NetworkApi.Clear();
        if (!string.IsNullOrEmpty(_PreviousScenePath))
        {
            if (System.IO.File.Exists(_PreviousScenePath))
            {
                SceneManager.LoadScene(_PreviousScenePath);
            }
        }
    }
    private static string FindScenePath(string sceneName)
    {
        string[] sceneGuids = AssetDatabase.FindAssets($"t:Scene {sceneName}");

        foreach (string guid in sceneGuids)
        {
            string path = AssetDatabase.GUIDToAssetPath(guid);
            if (System.IO.Path.GetFileNameWithoutExtension(path) == sceneName)
            {
                return path;
            }
        }
        return null;
    }
}