using UnityEngine;

public static class GameObjectCommon
{
    public static GameObject GetChildByPath(GameObject parent, in string path)
    {
        if (parent == null || string.IsNullOrEmpty(path))
            return null;
        string[] parts = path.Trim('/').Split('/');
        if (parts.Length == 0)
            return null;
        GameObject current = parent;
        for (int i = 0; i < parts.Length; i++)
        {
            Transform child = current.transform.Find(parts[i]);
            if (child == null)
                return null;
            current = child.gameObject;
        }
        return current;
    }

    public static GameObject GetGameObjectByPath(in string path)
    {
        if (string.IsNullOrEmpty(path))
            return null;
        string[] parts = path.Trim('/').Split('/');
        if (parts.Length == 0)
            return null;
        GameObject current = null;
        foreach (GameObject root in UnityEngine.SceneManagement.SceneManager.GetActiveScene().GetRootGameObjects())
        {
            if (root.name == parts[0])
            {
                current = root;
                break;
            }
        }
        if (current == null)
            return null;

        for (int i = 1; i < parts.Length; i++)
        {
            Transform child = current.transform.Find(parts[i]);
            if (child == null)
                return null;
            current = child.gameObject;
        }

        return current;
    }

    public static void ClearAllChild(GameObject go)
    {
        for (int i = go.transform.childCount - 1; i >= 0; i--)
        {
            Transform child = go.transform.GetChild(i);
            Object.Destroy(child.gameObject);
        }
    }
}
