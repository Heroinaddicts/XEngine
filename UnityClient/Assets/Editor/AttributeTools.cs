using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using UnityEditor;
using UnityEngine;

public class AttributeTools : MonoBehaviour
{
    static string _attr_dir;
    static string _template_dir;

    const string FILE_HEAD = "using System.Collections;\nusing System.Collections.Generic;\nusing UnityEngine;\n";

    struct AttrInfo
    {
        public string name;
        public string type_name;
        public string desc;
    }

    class AttrClassInfo
    {
        public string parent;
        public AttrInfo[] attr_infos;
    }

    static IDictionary<string, XmlDocument> _xml_doc_map = new Dictionary<string, XmlDocument>();
    static IDictionary<string, AttrClassInfo> _attr_class_map = new Dictionary<string, AttrClassInfo>();
    static IDictionary<string, string> _file_content_map = new Dictionary<string, string>();

    static AttrInfo[] GetAttrInfo(in string name)
    {
        AttrClassInfo info;
        if (name == null || !_attr_class_map.TryGetValue(name, out info))
        {
            return new AttrInfo[0];
        }

        AttrInfo[] parentInfos;
        if (info.parent != "" && info.parent != null)
        {
            parentInfos = GetAttrInfo(info.parent);
        }
        else
        {
            parentInfos = new AttrInfo[0];
        }

        AttrInfo[] attrInfos = new AttrInfo[parentInfos.Length + info.attr_infos.Length];

        for (int i = 0; i < parentInfos.Length; i++)
        {
            attrInfos[i] = parentInfos[i];
        }

        for (int i = 0; i < info.attr_infos.Length; i++)
        {
            attrInfos[parentInfos.Length + i] = info.attr_infos[i];
        }

        return attrInfos;
    }

    static AttrClassInfo CreateAttrInfo(in string name)
    {
        AttrClassInfo temp;
        if (_attr_class_map.TryGetValue(name, out temp))
        {
            return temp;
        }

        XmlDocument doc;
        if (!_xml_doc_map.TryGetValue(name, out doc))
        {
            Debug.LogError("unknown error");
            return null;
        }

        XmlElement root = doc.DocumentElement;
        int attr_count = root.ChildNodes.Count;
        AttrClassInfo class_info = new AttrClassInfo();
        if (root.Attributes["parent"] != null)
        {
            class_info.parent = root.Attributes["parent"].Value;
            AttrClassInfo parent_class = CreateAttrInfo(root.Attributes["parent"].Value);
        }
        class_info.attr_infos = new AttrInfo[attr_count];
        int index = 0;
        foreach (XmlNode node in root.ChildNodes)
        {
            if (node.Name != "attribute")
            {
                continue;
            }

            AttrInfo info = new AttrInfo();
            info.name = node.Attributes["name"].Value;
            switch (node.Attributes["type"].Value)
            {
                case "int64":
                    {
                        info.type_name = "eAttributeType.INT";
                        break;
                    }
                case "float":
                    {
                        info.type_name = "eAttributeType.FLOAT";
                        break;
                    }
                case "string":
                    {
                        info.type_name = "eAttributeType.STRING";
                        break;
                    }
                case "bool":
                    {
                        info.type_name = "eAttributeType.BOOL";
                        break;
                    }
                default:
                    {
                        Debug.LogError("error attr type : " + node.Attributes["type"].Value);
                        return null;
                    }
            }
            if(node.Attributes["desc"] != null)
            {
                info.desc = node.Attributes["desc"].Value;
            } else
            {
                info.desc = "no desc";
            }
            class_info.attr_infos[index++] = info;
        }

        _attr_class_map.Add(name, class_info);
        return class_info;
    }

    static void CreateAttrs()
    {
        foreach (KeyValuePair<string, XmlDocument> kv in _xml_doc_map)
        {
            CreateAttrInfo(kv.Key);
        }
    }

    static void CreateAttributeCodeFile()
    {
        foreach (var temp in _attr_class_map)
        {
            string filename = temp.Key + "_attr.cs";
            string filecontent = FILE_HEAD;

            if (temp.Value.parent != null && temp.Value.parent != "")
            {
                filecontent += "public class " + Tools.String.Capitalize(temp.Key) + "Attr : " + Tools.String.Capitalize(temp.Value.parent) + "Attr\n{\n";
            }
            else
            {
                filecontent += "public class " + Tools.String.Capitalize(temp.Key) + "Attr : IAttribute\n{\n";
            }

            AttrInfo[] attrInfos = GetAttrInfo(temp.Key);
            AttrInfo[] parentInfos = GetAttrInfo(temp.Value.parent);
            filecontent += "    public enum e" + Tools.String.Capitalize(temp.Key) + "AttrIndex\n";
            filecontent += "    {\n";

            for (int i = 0; i < temp.Value.attr_infos.Length; i++)
            {
                filecontent += "        " + Tools.String.Capitalize(temp.Value.attr_infos[i].name) + " = " + (parentInfos.Length + i) + ",\n";
            }
            filecontent += "    }\n\n";

            filecontent += "    void Awake()\n    {\n";
            filecontent += "        Attributes = new AttrInfo[" + attrInfos.Length + "];\n\n";
            for (int i = 0; i < attrInfos.Length; i++)
            {
                filecontent += "        Attributes[" + i + "] = new AttrInfo(\"" + attrInfos[i].name + "\", " + i + ", " + attrInfos[i].type_name + ", \"" + attrInfos[i].desc + "\");\n";
            }
            filecontent += "    }\n";

            for (int i = 0; i < temp.Value.attr_infos.Length; i++)
            {
                switch (temp.Value.attr_infos[i].type_name)
                {
                    case "eAttributeType.INT":
                        {
                            filecontent += "    public long Get" + Tools.String.Capitalize(temp.Value.attr_infos[i].name) + "()\n";
                            filecontent += "    {\n";
                            filecontent += "        return Attributes[" + (parentInfos.Length + i) + "].GetInt();\n";
                            filecontent += "    }\n\n";

                            filecontent += "    public void Set" + Tools.String.Capitalize(temp.Value.attr_infos[i].name) + "(in long value, in object context = null, in bool trigger = true)\n";
                            filecontent += "    {\n";
                            filecontent += "        Attributes[" + (parentInfos.Length + i) + "].SetInt(value, context, trigger);\n";
                            filecontent += "    }\n\n";
                            break;
                        }
                    case "eAttributeType.FLOAT":
                        {
                            filecontent += "    public float Get" + Tools.String.Capitalize(temp.Value.attr_infos[i].name) + "()\n";
                            filecontent += "    {\n";
                            filecontent += "        return Attributes[" + (parentInfos.Length + i) + "].GetFloat();\n";
                            filecontent += "    }\n\n";

                            filecontent += "    public void Set" + Tools.String.Capitalize(temp.Value.attr_infos[i].name) + "(in float value, in object context = null, in bool trigger = true)\n";
                            filecontent += "    {\n";
                            filecontent += "        Attributes[" + (parentInfos.Length + i) + "].SetFloat(value, context, trigger);\n";
                            filecontent += "    }\n\n";
                            break;
                        }
                    case "eAttributeType.STRING":
                        {
                            filecontent += "    public string Get" + Tools.String.Capitalize(temp.Value.attr_infos[i].name) + "()\n";
                            filecontent += "    {\n";
                            filecontent += "        return Attributes[" + (parentInfos.Length + i) + "].GetString();\n";
                            filecontent += "    }\n\n";

                            filecontent += "    public void Set" + Tools.String.Capitalize(temp.Value.attr_infos[i].name) + "(in string value, in object context = null, in bool trigger = true)\n";
                            filecontent += "    {\n";
                            filecontent += "        Attributes[" + (parentInfos.Length + i) + "].SetString(value, context, trigger);\n";
                            filecontent += "    }\n\n";
                            break;
                        }
                    case "eAttributeType.BOOL":
                        {
                            filecontent += "    public bool Get" + Tools.String.Capitalize(temp.Value.attr_infos[i].name) + "()\n";
                            filecontent += "    {\n";
                            filecontent += "        return Attributes[" + (parentInfos.Length + i) + "].GetBool();\n";
                            filecontent += "    }\n\n";

                            filecontent += "    public void Set" + Tools.String.Capitalize(temp.Value.attr_infos[i].name) + "(in bool value, in object context = null, in bool trigger = true)\n";
                            filecontent += "    {\n";
                            filecontent += "        Attributes[" + (parentInfos.Length + i) + "].SetBool(value, context, trigger);\n";
                            filecontent += "    }\n\n";
                            break;
                        }
                }
            }

            filecontent += "}\n";

            _file_content_map.Add(temp.Key, filecontent);
        }

        if (!Directory.Exists(_attr_dir))
        {
            Directory.CreateDirectory(_attr_dir);
            return;
        }

        DirectoryInfo direction = new DirectoryInfo(_attr_dir);
        FileInfo[] files = direction.GetFiles("*", SearchOption.AllDirectories);
        for (int i = 0; i < files.Length; i++)
        {
            if (files[i].Name.EndsWith(".meta"))
            {
                continue;
            }
            string FilePath = _attr_dir + "/" + files[i].Name;
            File.Delete(FilePath);
        }

        foreach (var kv in _file_content_map)
        {
            StreamWriter sw;
            FileInfo t = new FileInfo(_attr_dir + "/" + char.ToUpper(kv.Key[0]) + kv.Key.Substring(1) + "Attr.cs");
            sw = t.CreateText();
            sw.WriteLine(kv.Value);
            sw.Close();
            sw.Dispose();
        }
    }


    [MenuItem("Tools/Attribute/Generat Code")]
    public static void GeneratCode()
    {
        _template_dir = Application.dataPath + "/Editor/ModelTemplate";
        _attr_dir = Application.dataPath + "/Scripts/Attributes";

        {
            _xml_doc_map.Clear();
            _attr_class_map.Clear();
            _file_content_map.Clear();

            DirectoryInfo dir = new DirectoryInfo(_template_dir);
            FileInfo[] xmlfiles = dir.GetFiles("*.xml");
            foreach (FileInfo f in xmlfiles)
            {
                XmlDocument xml = new XmlDocument();
                try
                {
                    xml.Load(f.FullName);
                    _xml_doc_map.Add(f.Name.Split(".xml")[0], xml);
                }
                catch (Exception e)
                {
                    Debug.LogError(e.Message);
                    return;
                }
            }

            CreateAttrs();
            CreateAttributeCodeFile();
        }

        AssetDatabase.Refresh();
    }
}
