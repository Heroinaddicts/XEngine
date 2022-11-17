using System.Collections;
using System.Collections.Generic;
using System.Xml;
using UnityEditor;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ExportRefreshConfig : MonoBehaviour
{
    [System.Obsolete]
    static bool AnalysisRule(Transform tf, XmlDocument document, XmlElement root)
    {
        RefreshRule rp = tf.gameObject.GetComponent<RefreshRule>();
        if (rp != null)
        {
            XmlElement elm = document.CreateElement("RefreshRule");
            elm.SetAttribute("id", rp.ConfigID.ToString());
            root.AppendChild(elm);

            for (int i = 0; i < tf.childCount; i++)
            {
                if (tf.GetChild(i).gameObject.active == false)
                {
                    continue;
                }
                XmlElement elmpoint = document.CreateElement("Position");
                elm.AppendChild(elmpoint);
                elmpoint.SetAttribute("x", tf.GetChild(i).position.x.ToString());
                elmpoint.SetAttribute("y", tf.GetChild(i).position.y.ToString());
                elmpoint.SetAttribute("z", tf.GetChild(i).position.z.ToString());
            }
        }

        for (int i = 0; i < tf.childCount; i++)
        {
            AnalysisRule(tf.GetChild(i), document, root);
        }

        return false;
    }

    [MenuItem("ExportScene/ExportRefreshConfig")]
    public static void OnExportRefreshConfig()
    {
        Transform[] tfs = Selection.transforms;

        string path = Application.dataPath + "/../../Public/Scene/RefreshRule/" + SceneManager.GetActiveScene().name + ".xml";
        XmlDocument doc = new XmlDocument();
        XmlElement root = doc.CreateElement("root");
        doc.AppendChild(root);
        for (int i = 0; i < tfs.Length; i++)
        {
            AnalysisRule(tfs[i], doc, root);
        }

        doc.Save(path);
        AssetDatabase.Refresh();
    }
}
