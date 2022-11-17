using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using System.Xml;
using System.IO;
using System.Reflection;
using System;

[CanEditMultipleObjects]
[CustomEditor(typeof(Transform))]
public class TransformInspector : Editor
{
    SerializedProperty mPos;
    SerializedProperty mScale;
    public override void OnInspectorGUI()
    {
        EditorGUIUtility.labelWidth = 15f;
        mPos = serializedObject.FindProperty("m_LocalPosition");
        mScale = serializedObject.FindProperty("m_LocalScale");

        DrawPosition();
        DrawRotation();
        DrawScale();

        serializedObject.ApplyModifiedProperties();
    }

    void DrawPosition()
    {
        GUILayout.BeginHorizontal();
        {
            bool reset = GUILayout.Button("P", GUILayout.Width(20f));
            EditorGUILayout.LabelField("Position", GUILayout.Width(50f));
            EditorGUILayout.PropertyField(mPos.FindPropertyRelative("x"));
            EditorGUILayout.PropertyField(mPos.FindPropertyRelative("y"));
            EditorGUILayout.PropertyField(mPos.FindPropertyRelative("z"));
            if (reset) mPos.vector3Value = Vector3.zero;
        }
        GUILayout.EndHorizontal();
    }

    void DrawScale()
    {
        GUILayout.BeginHorizontal();
        {
            bool reset = GUILayout.Button("S", GUILayout.Width(20f));
            EditorGUILayout.LabelField("Scale", GUILayout.Width(50f));
            EditorGUILayout.PropertyField(mScale.FindPropertyRelative("x"));
            EditorGUILayout.PropertyField(mScale.FindPropertyRelative("y"));
            EditorGUILayout.PropertyField(mScale.FindPropertyRelative("z"));
            if (reset) mScale.vector3Value = Vector3.one;
        }
        GUILayout.EndHorizontal();
    }

    void DrawRotation()
    {
        GUILayout.BeginHorizontal();
        {
            bool reset = GUILayout.Button("R", GUILayout.Width(20f));
            EditorGUILayout.LabelField("Rotation", GUILayout.Width(50f));
            Vector3 ls = (serializedObject.targetObject as Transform).localEulerAngles;
            FloatField("X", ref ls.x);
            FloatField("Y", ref ls.y);
            FloatField("Z", ref ls.z);
            if (reset)
                (serializedObject.targetObject as Transform).localEulerAngles = Vector3.zero;
            else
                (serializedObject.targetObject as Transform).localEulerAngles = ls;
        }
        GUILayout.EndHorizontal();
    }

    void FloatField(string name, ref float f)
    {
        f = EditorGUILayout.FloatField(name, f);
    }

    [MenuItem("Tools/导出怪物动画配置")]
    public static void ExportAnimationInfo()
    {
        var resPath = Application.dataPath + "/Resources/Prefabs/monster";

        string pathString = Directory.GetFiles(resPath)[0];

        GameObject obj = PrefabUtility.LoadPrefabContents(pathString);

        AnimationClip[] aniClip = AnimationUtility.GetAnimationClips(obj);

        XmlDocument doc = new XmlDocument();
        XmlDeclaration declaration = doc.CreateXmlDeclaration("1.0", "UTF-8", null);
        //2.将头部信息添加到xml文档
        doc.AppendChild(declaration);
        //3. 创建一个根节点
        XmlNode root = doc.CreateNode(XmlNodeType.Element, "Root", null);
        doc.AppendChild(root);

        for (int i = 0; i < aniClip.Length; i++)
        {
            AnimationClip clip = aniClip[i];
            XmlElement ele = doc.CreateElement("AnimationClip");
            ele.SetAttribute("name", clip.name);
            ele.SetAttribute("length", clip.length.ToString());
            for (int j = 0; j < clip.events.Length; j++)
            {
                var evnt = clip.events[j];
                XmlElement ele1 = doc.CreateElement("AnimationEvent");
                ele1.SetAttribute("function", evnt.functionName);
                ele1.SetAttribute("time", evnt.time.ToString());
                ele.AppendChild(ele1);
            }
            root.AppendChild(ele);
        }

        doc.Save(Application.dataPath + "/../../Documents/Config/MonsterAnimation.xml");
        UnityEditor.EditorUtility.DisplayDialog("", "导出完成", "确认");
    }


    //public static void ExportZombieBornPoints()
    //{
    //    GameObject obj = GameObject.Find("ZombieBornPoints");
    //    TextAsset xmlFile = Resources.Load<TextAsset>("XmlConfig/refreshPoints"); //加载path路径下的xml文件内容
    //    XmlDocument document = new XmlDocument(); //创建xml文件的变量
    //    document.LoadXml(xmlFile.text); //让xml文件变量读取xmlFile内的文本
    //    document.DocumentElement.RemoveAll();
    //    System.Type type = typeof(RefreshPoint);
    //    FieldInfo[] fields = type.GetFields();

    //    for (int i = 0; i < obj.transform.childCount; i++)
    //    {
    //        Transform trans = obj.transform.GetChild(i);
    //        RefreshPoint point = trans.gameObject.GetComponent<RefreshPoint>();
    //        if (!point)
    //        {
    //            continue;
    //        }
    //        XmlElement ele = document.CreateElement("RefreshPoint");
    //        foreach (FieldInfo info in fields)
    //        {
    //            if (info.FieldType == typeof(System.Collections.Generic.List<int>) || info.FieldType == typeof(System.Collections.Generic.List<float>))
    //            {
    //                IList value = info.GetValue(point) as IList;
    //                string s = "";
    //                if (value != null)
    //                {
    //                    for (int k = 0; k < value.Count; k++)
    //                    {
    //                        s += value[k].ToString();
    //                        if (k != (value.Count - 1))
    //                        {
    //                            s += ",";
    //                        }
    //                    }
    //                }
    //                ele.SetAttribute(info.Name, s);
    //            }
    //            else if (info.FieldType == typeof(bool))
    //            {
    //                bool b = (bool)info.GetValue(point);
    //                ele.SetAttribute(info.Name, (b ? 1 : 0).ToString());
    //            }
    //            else
    //            {
    //                ele.SetAttribute(info.Name, info.GetValue(point).ToString());
    //            }
    //        }
    //        ele.SetAttribute("posx", trans.position.x.ToString());
    //        ele.SetAttribute("posy", trans.position.y.ToString());
    //        ele.SetAttribute("posz", trans.position.z.ToString());
    //        ele.SetAttribute("rotx", trans.rotation.eulerAngles.x.ToString());
    //        ele.SetAttribute("roty", trans.rotation.eulerAngles.y.ToString());
    //        ele.SetAttribute("rotz", trans.rotation.eulerAngles.z.ToString());
    //        document.DocumentElement.AppendChild(ele);
    //    }
    //    document.Save(Application.dataPath + "/../../Documents/Config/refreshPoints.xml");
    //    AssetDatabase.Refresh();
    //    Debug.Log("导出刷新点数据完成");
    //}

    //public static void ImportZombieBornPoints()
    //{
    //    TextAsset xmlFile = Resources.Load<TextAsset>("XmlConfig/refreshPoints"); //加载path路径下的xml文件内容
    //    XmlDocument document = new XmlDocument(); //创建xml文件的变量
    //    document.LoadXml(xmlFile.text); //让xml文件变量读取xmlFile内的文本
    //    document.Load(Application.dataPath + "/../../Documents/Config/refreshPoints.xml");

    //    System.Type type = typeof(RefreshPoint);
    //    FieldInfo[] fields = type.GetFields();

    //    GameObject root = GameObject.Find("ZombieBornPoints");
    //    if (root)
    //    {
    //        GameObject.DestroyImmediate(root);
    //    }
    //    root = new GameObject("ZombieBornPoints");
    //    XmlNode xmlRoot = document.SelectSingleNode("root");

    //    foreach (XmlElement ele in xmlRoot.ChildNodes)
    //    {
    //        GameObject obj = new GameObject("RefreshPoint_" + ele.GetAttribute("id"));
    //        obj.transform.parent = root.transform;
    //        RefreshPoint point = obj.AddComponent<RefreshPoint>();

    //        foreach (FieldInfo field in fields)
    //        {
    //            if (field.FieldType == typeof(System.Collections.Generic.List<int>) || field.FieldType == typeof(System.Collections.Generic.List<float>))
    //            {
    //                var str = ele.GetAttribute(field.Name);
    //                string[] strArray = str.Split(',');
    //                if (string.IsNullOrEmpty(str)) continue;

    //                if (field.FieldType == typeof(System.Collections.Generic.List<int>))
    //                {
    //                    List<int> list = new List<int>();
    //                    foreach (string s in strArray)
    //                    {
    //                        list.Add(int.Parse(s));
    //                    }
    //                    field.SetValue(point, list);
    //                }
    //                else
    //                {
    //                    List<float> list = new List<float>();
    //                    foreach (string s in strArray)
    //                    {
    //                        list.Add(float.Parse(s));
    //                    }
    //                    field.SetValue(point, list);
    //                }
    //            }
    //            else if (field.FieldType == typeof(bool))
    //            {
    //                int val = int.Parse(ele.GetAttribute(field.Name));
    //                field.SetValue(point, val == 1);
    //            }
    //            else if (field.FieldType == typeof(string))
    //            {
    //                field.SetValue(point, ele.GetAttribute(field.Name));
    //            }
    //            else if (field.FieldType == typeof(int))
    //            {
    //                field.SetValue(point, int.Parse(ele.GetAttribute(field.Name)));
    //            }
    //            else if (field.FieldType.IsEnum)
    //            {
    //                var tmp = Enum.Parse(field.FieldType, ele.GetAttribute(field.Name));
    //                field.SetValue(point, tmp);
    //            }
    //            else
    //            {
    //                field.SetValue(point, float.Parse(ele.GetAttribute(field.Name)));
    //            }
    //        }
    //    }

    //}
}
