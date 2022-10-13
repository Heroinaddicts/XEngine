/******************************************/
/*                                        */
/*     Copyright (c) 2018 monitor1394     */
/*     https://github.com/monitor1394     */
/*                                        */
/******************************************/

using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;
using UnityEditor;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ForServer : EditorWindow
{
    private static float autoCutMinX = 1000;
    private static float autoCutMaxX = 0;
    private static float autoCutMinY = 1000;
    private static float autoCutMaxY = 0;

    static IDictionary<int, string> _obj_file_map = new Dictionary<int, string>();

    static string GetMeshFileName(Mesh mesh)
    {
        return mesh.name + "_" + mesh.GetInstanceID() + ".obj";
    }

    static string GetServerPhysxDataPath()
    {
        return Application.dataPath + "../../Physx/" + SceneManager.GetActiveScene().name + "/";
    }
    static string GetMeshPath(Mesh mesh)
    {
        return GetServerPhysxDataPath() + GetMeshFileName(mesh);
    }

    static void ExportObjForServer(Transform tf, ref XmlDocument doc, ref XmlElement root)
    {
        if (false == tf.gameObject.activeSelf)
        {
            return;
        }

        MeshFilter filter = tf.GetComponent<MeshFilter>();
        if (filter != null)
        {
            Mesh mesh = filter.sharedMesh;
            if (mesh != null)
            {
                if (!_obj_file_map.ContainsKey(mesh.GetInstanceID()))
                {
                    if (tf.localScale.x != 1 || tf.localScale.y != 1 || tf.localScale.z != 1)
                    {
                        Debug.Log("Scaled " + mesh.name + " : " + tf.localScale);
                    }

                    Quaternion r = tf.localRotation;
                    StringBuilder sb = new StringBuilder();
                    foreach (Vector3 v in mesh.vertices)
                    {
                        UpdateAutoCutRect(v);
                        sb.AppendFormat("v {0} {1} {2}\n", v.x, v.y, v.z);
                    }
                    foreach (Vector3 nn in mesh.normals)
                    {
                        Vector3 v = r * nn;
                        sb.AppendFormat("vn {0} {1} {2}\n", v.x, v.y, v.z);
                    }
                    foreach (Vector3 v in mesh.uv)
                    {
                        sb.AppendFormat("vt {0} {1}\n", v.x, v.y);
                    }
                    for (int i = 0; i < mesh.subMeshCount; i++)
                    {
                        int[] triangles = mesh.GetTriangles(i);
                        for (int j = 0; j < triangles.Length; j += 3)
                        {
                            sb.AppendFormat("f {1} {0} {2}\n",
                                triangles[j + 2] + 1,
                                triangles[j + 1] + 1,
                                triangles[j] + 1);
                        }
                    }
                    StreamWriter writer = new StreamWriter(GetMeshPath(mesh));
                    writer.Write(sb.ToString());
                    _obj_file_map.Add(mesh.GetInstanceID(), GetMeshPath(mesh));
                    writer.Close();
                }

                XmlElement sceneobj = doc.CreateElement("SceneObj");
                sceneobj.SetAttribute("SceneObj", GetMeshFileName(mesh));
                sceneobj.SetAttribute("x", tf.position.x.ToString());
                sceneobj.SetAttribute("y", tf.position.y.ToString());
                sceneobj.SetAttribute("z", (tf.position.z).ToString());

                sceneobj.SetAttribute("ScaleX", tf.localScale.x.ToString());
                sceneobj.SetAttribute("ScaleY", tf.localScale.y.ToString());
                sceneobj.SetAttribute("ScaleZ", tf.localScale.z.ToString());

                sceneobj.SetAttribute("QuaternionX", tf.rotation.x.ToString());
                sceneobj.SetAttribute("QuaternionY", tf.rotation.y.ToString());
                sceneobj.SetAttribute("QuaternionZ", tf.rotation.z.ToString());
                sceneobj.SetAttribute("QuaternionW", tf.rotation.w.ToString());
                root.AppendChild(sceneobj);
            }
        }

        for (int i = 0; i < tf.childCount; i++)
        {
            ExportObjForServer(tf.GetChild(i), ref doc, ref root);
        }
    }

    [MenuItem("ForServer/ExportObjsForServerPhysx")]
    public static void ExportObjsForServerPhysx()
    {
        _obj_file_map.Clear();
        XmlDocument doc = new XmlDocument();
        XmlElement root = doc.CreateElement("root");
        doc.AppendChild(root);

        Transform[] tfs = Selection.transforms;
        for (int i = 0; i < tfs.Length; i++)
        {
            ExportObjForServer(tfs[i], ref doc, ref root);
        }

        doc.Save(GetServerPhysxDataPath() + "SceneObjs.xml");
        OpenDir(GetServerPhysxDataPath());
    }

    private static void OpenDir(string path)
    {
        DirectoryInfo dir = Directory.GetParent(path);
        int index = path.LastIndexOf("/");
        OpenCmd("explorer.exe", dir.FullName);
    }

    private static void OpenCmd(string cmd, string args)
    {
        System.Diagnostics.Process.Start(cmd, args);
    }

    private static long GetMsTime()
    {
        return System.DateTime.Now.Ticks / 10000;
        //return (System.DateTime.Now.ToUniversalTime().Ticks - 621355968000000000) / 10000;
    }

    private static void UpdateAutoCutRect(Vector3 v)
    {
        if (v.x < autoCutMinX) autoCutMinX = v.x;
        if (v.x > autoCutMaxX) autoCutMaxX = v.x;
        if (v.z < autoCutMinY) autoCutMinY = v.z;
        if (v.z > autoCutMaxY) autoCutMaxY = v.z;
    }
}
