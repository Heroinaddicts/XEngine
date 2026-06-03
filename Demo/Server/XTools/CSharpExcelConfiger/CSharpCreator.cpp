#include "LuaCreator.h"
#include "Enum.h"
#include "Const.h"
#include "Array.h"
#include "ExcelData.h"
#include "XFile.h"

namespace ExcelConfiger {

    static std::string s_ConfigScript;

    __forceinline void SaveFile(const std::string& path, const std::string& content) {
        XFile file;
        if (!file.Open(path, "w+")) {
            XASSERT(false, "wtf");
            return;
        }

        file << content;
        file.Save();
        file.Close();
    }

    __forceinline std::string CreateCSharpEnumScript() {
        std::string script = "    public static class AutoEnum\n";
        script += "    {\n";
        const std::map<std::string, Enum*>& enums = Enum::GetEnums();

        for (auto itorEnum = enums.begin(); itorEnum != enums.end(); itorEnum++) {
            script += "        public enum " + itorEnum->first + "\n        {\n";

            const std::map<std::string, Value*>& values = itorEnum->second->GetValueMap();

            for (auto itorValue = values.begin(); itorValue != values.end(); itorValue++) {
                script += "            " + itorValue->first + " = " + SafeString::Int64ToString(itorValue->second->_Int) + ", //" + itorValue->second->_Desc + "\n";
            }

            script += "        }\n\n";
        }
        script += "    }\n";

        return script;
    }

    __forceinline std::string CreateCSharpConstScript() {
        std::string script = "    public static class AutoConst\n";
        script += "    {\n";

        const std::map<std::string, Const*>& consts = Const::GetConsts();
        for (auto itorConsts = consts.begin(); itorConsts != consts.end(); itorConsts++) {
            script += "        public static class " + itorConsts->first + "\n";
            script += "        {\n";
            for (auto itorValue = itorConsts->second->_Values.begin(); itorValue != itorConsts->second->_Values.end(); itorValue++) {
                if (itorValue->second->_Info._IsArray) {
                    switch (itorValue->second->_Info._ValueType)
                    {
                    case eValueType::Int:
                        script += "            public static readonly Int64[] " + itorValue->first + " = {" + SafeString::Replace(itorValue->second->_ValueString, ";", ", ") + "};\n";
                        break;
                    case eValueType::Float:
                    {
                        script += "            public static readonly float[] " + itorValue->first + " = {";
                        std::vector<std::string> values;
                        SafeString::Split(itorValue->second->_ValueString, ";", values);
                        for (int i = 0; i < values.size(); i++) {
                            script += values[i] + "f";
                            if (i != values.size() - 1) {
                                script += ", ";
                            }
                        }
                        script += "};\n";
                        break;
                    }
                    case eValueType::String:
                    {
                        script += "            public static readonly string[] " + itorValue->first + " = {";
                        std::vector<std::string> values;
                        SafeString::Split(itorValue->second->_ValueString, ";", values);
                        for (int i = 0; i < values.size(); i++) {
                            script += "\"" + values[i] + "\"";
                            if (i != values.size() - 1) {
                                script += ", ";
                            }
                        }
                        script += "};\n";
                        break;
                    }
                    case eValueType::Bool:
                        script += "            public static readonly bool[] " + itorValue->first + " = {" + SafeString::Replace(itorValue->second->_ValueString, ";", ", ") + "};\n";
                        break;
                    case eValueType::Enum:
                    {
                        script += "            public static readonly AutoConfig.AutoEnum." + Enum::GetEnum(itorValue->second->_Info._TypeString)->_Name + "[] " + itorValue->first + " = {";
                        std::vector<std::string> values;
                        SafeString::Split(itorValue->second->_ValueString, ";", values);
                        for (int i = 0; i < values.size(); i++) {
                            script += "AutoConfig.AutoEnum." + Enum::GetEnum(itorValue->second->_Info._TypeString)->_Name + "." + values[i];
                            if (i != values.size() - 1) {
                                script += ", ";
                            }
                        }
                        script += "};\n";

                        break;
                    }
                    }
                }
                else {
                    switch (itorValue->second->_Info._ValueType)
                    {
                    case eValueType::Int:
                        script += "            public static readonly Int64 " + itorValue->first + " = " + itorValue->second->_ValueString + ";\n";
                        break;
                    case eValueType::Float:
                        script += "            public static readonly float " + itorValue->first + " = " + itorValue->second->_ValueString + "f;\n";
                        break;
                    case eValueType::String:
                        script += "            public static readonly string " + itorValue->first + " = \"" + itorValue->second->_ValueString + "\";\n";
                        break;
                    case eValueType::Bool:
                        script += "            public static readonly bool " + itorValue->first + " = " + itorValue->second->_ValueString + ";\n";
                        break;
                    case eValueType::Enum:
                        script += "            public static readonly AutoConfig.AutoEnum." + Enum::GetEnum(itorValue->second->_Info._TypeString)->_Name + " " + itorValue->first + " = AutoConfig.AutoEnum." + Enum::GetEnum(itorValue->second->_Info._TypeString)->_Name + "." + itorValue->second->_ValueString + ";\n";
                        break;
                    }
                }
            }
            script += "        }\n\n";
        }

        script += "    }\n";

        return script;
    }

    __forceinline std::string CreateExcelDataScript(const ExcelData* excelData) {
        std::string script = "using System;\n";
        script += "using System.Collections.Generic;\n";
        script += "using System.Collections.ObjectModel;\n";
        script += "namespace AutoConfig\n{\n";
        script += "    public class " + excelData->_Def->_Name + "\n";
        script += "    {\n";

        script += "        public const Int64 INVALID_CONFIG_ID = 0;\n";


        std::string constructorDef = "        public " + excelData->_Def->_Name + "(";
        std::string constructorBody = "        {\n";

        for (int i = 0; i < excelData->_Def->_MemberInfos.size(); i++) {
            if (excelData->_Def->_MemberInfos[i]._TypeInfo._IsArray) {
                switch (excelData->_Def->_MemberInfos[i]._TypeInfo._ValueType) {
                case eValueType::Int: {
                    script += "        public readonly Int64[] _" + excelData->_Def->_MemberInfos[i]._Name + ";\n";
                    constructorDef += "Int64[] " + excelData->_Def->_MemberInfos[i]._Name;
                    break;
                }
                case eValueType::Float: {
                    script += "        public readonly float[] _" + excelData->_Def->_MemberInfos[i]._Name + ";\n";
                    constructorDef += "float[] " + excelData->_Def->_MemberInfos[i]._Name;
                    break;
                }
                case eValueType::String: {
                    script += "        public readonly string[] _" + excelData->_Def->_MemberInfos[i]._Name + ";\n";
                    constructorDef += "string[] " + excelData->_Def->_MemberInfos[i]._Name;
                    break;
                }
                case eValueType::Bool: {
                    script += "        public readonly bool[] _" + excelData->_Def->_MemberInfos[i]._Name + ";\n";
                    constructorDef += "bool[] " + excelData->_Def->_MemberInfos[i]._Name;
                    break;
                }
                case eValueType::Enum: {
                    script += "        public readonly AutoEnum." + excelData->_Def->_MemberInfos[i]._TypeInfo._TypeString + "[] _" + excelData->_Def->_MemberInfos[i]._Name + ";\n";
                    constructorDef += "AutoEnum." + excelData->_Def->_MemberInfos[i]._TypeInfo._TypeString + "[] " + excelData->_Def->_MemberInfos[i]._Name;
                    break;
                }
                }
            }
            else {
                switch (excelData->_Def->_MemberInfos[i]._TypeInfo._ValueType) {
                case eValueType::Int: {
                    script += "        public readonly Int64 _" + excelData->_Def->_MemberInfos[i]._Name + ";\n";
                    constructorDef += "Int64 " + excelData->_Def->_MemberInfos[i]._Name;
                    break;
                }
                case eValueType::Float: {
                    script += "        public readonly float _" + excelData->_Def->_MemberInfos[i]._Name + ";\n";
                    constructorDef += "float " + excelData->_Def->_MemberInfos[i]._Name;
                    break;
                }
                case eValueType::String: {
                    script += "        public readonly string _" + excelData->_Def->_MemberInfos[i]._Name + ";\n";
                    constructorDef += "string " + excelData->_Def->_MemberInfos[i]._Name;
                    break;
                }
                case eValueType::Bool: {
                    script += "        public readonly bool _" + excelData->_Def->_MemberInfos[i]._Name + ";\n";
                    constructorDef += "bool " + excelData->_Def->_MemberInfos[i]._Name;
                    break;
                }
                case eValueType::Enum: {
                    script += "        public readonly AutoEnum." + excelData->_Def->_MemberInfos[i]._TypeInfo._TypeString + " _" + excelData->_Def->_MemberInfos[i]._Name + ";\n";
                    constructorDef += "AutoEnum." + excelData->_Def->_MemberInfos[i]._TypeInfo._TypeString + " " + excelData->_Def->_MemberInfos[i]._Name;
                    break;
                }
                }
            }

            constructorBody += "            _" + excelData->_Def->_MemberInfos[i]._Name += " = " + excelData->_Def->_MemberInfos[i]._Name + ";\n";
            if (i != excelData->_Def->_MemberInfos.size() - 1) {
                constructorDef += ", ";
            }
        }
        constructorDef += ")\n";
        constructorBody += "        }\n";

        script += constructorDef + constructorBody;

        if (excelData->_Def->_MemberInfos[0]._TypeInfo._ValueType == eValueType::Int) {
            script += "        public readonly static ReadOnlyDictionary<Int64, " + excelData->_Def->_Name + "> Datas = new ReadOnlyDictionary<Int64, " + excelData->_Def->_Name + ">(\n";
            script += "            new Dictionary<Int64, " + excelData->_Def->_Name + ">() {\n";
        }
        else if (excelData->_Def->_MemberInfos[0]._TypeInfo._ValueType == eValueType::String) {
            script += "        public readonly static ReadOnlyDictionary<string, " + excelData->_Def->_Name + "> Datas = new ReadOnlyDictionary<string, " + excelData->_Def->_Name + ">(\n";
            script += "            new Dictionary<string, " + excelData->_Def->_Name + ">() {\n";
        }
        else {
            XASSERT(false, "wtf");
        }
        for (int row = 0; row < excelData->_Values.size(); row++) {
            if (excelData->_Def->_MemberInfos[0]._TypeInfo._ValueType == eValueType::Int) {
                script += "                {" + excelData->_Values[row][0]->_ValueString + ", new " + excelData->_Def->_Name + "(";
            }
            else if (excelData->_Def->_MemberInfos[0]._TypeInfo._ValueType == eValueType::String) {
                script += "                {\"" + excelData->_Values[row][0]->_ValueString + "\", new " + excelData->_Def->_Name + "(";
            }
            else {
                XASSERT(false, "wtf");
            }

            for (int column = 0; column < excelData->_Def->_MemberInfos.size(); column++) {
                const ExcelDef::MemberInfo* info = &(excelData->_Def->_MemberInfos[column]);
                const Value* data = excelData->_Values[row][column];
                if (info->_TypeInfo._IsArray) {
                    switch (info->_TypeInfo._ValueType) {
                    case eValueType::Int: {
                        script += "new Int64[] {";
                        std::vector<std::string> values;
                        SafeString::Split(data->_ValueString, ";", values);
                        for (int index = 0; index < values.size(); index++) {
                            script += values[index];
                            if (index != values.size() - 1) {
                                script += ",";
                            }
                        }
                        break;
                    }
                    case eValueType::Float: {
                        script += "new float[] {";
                        std::vector<std::string> values;
                        SafeString::Split(data->_ValueString, ";", values);
                        for (int index = 0; index < values.size(); index++) {
                            script += values[index] + "f";
                            if (index != values.size() - 1) {
                                script += ",";
                            }
                        }
                        break;
                    }
                    case eValueType::String: {
                        script += "new string[] {";
                        std::vector<std::string> values;
                        SafeString::Split(data->_ValueString, ";", values);
                        for (int index = 0; index < values.size(); index++) {
                            script += "\"" + values[index] + "\"";
                            if (index != values.size() - 1) {
                                script += ",";
                            }
                        }
                        break;
                    }
                    case eValueType::Bool: {
                        script += "new bool[] {";
                        std::vector<std::string> values;
                        SafeString::Split(data->_ValueString, ";", values);
                        for (int index = 0; index < values.size(); index++) {
                            script += values[index];
                            if (index != values.size() - 1) {
                                script += ",";
                            }
                        }
                        break;
                    }
                    case eValueType::Enum: {
                        script += "new AutoEnum." + info->_TypeInfo._TypeString + "[] {";
                        std::vector<std::string> values;
                        SafeString::Split(data->_ValueString, ";", values);
                        for (int index = 0; index < values.size(); index++) {
                            script += "AutoEnum." + info->_TypeInfo._TypeString + "." + values[index];
                            if (index != values.size() - 1) {
                                script += ",";
                            }
                        }
                        break;
                    }
                    }
                    script += "}";
                }
                else {
                    switch (info->_TypeInfo._ValueType) {
                    case eValueType::Int: {
                        script += data->_ValueString;
                        break;
                    }
                    case eValueType::Float: {
                        script += data->_ValueString + "f";
                        break;
                    }
                    case eValueType::String: {
                        script += "\"" + data->_ValueString + "\"";
                        break;
                    }
                    case eValueType::Bool: {
                        script += data->_ValueString;
                        break;
                    }
                    case eValueType::Enum: {
                        script += "AutoEnum." + info->_TypeInfo._TypeString + "." + data->_ValueString;
                        break;
                    }
                    }
                }

                if (column != excelData->_Def->_MemberInfos.size() - 1) {
                    script += ", ";
                }
            }

            script += ")},\n";
        }
        script += "            }\n";
        script += "        );\n";

        if (excelData->_Def->_MemberInfos[0]._TypeInfo._ValueType == eValueType::Int) {
            script += "        public static " + excelData->_Def->_Name + " Get(Int64 id) {\n";
            script += "            " + excelData->_Def->_Name + " config = null; if (Datas.TryGetValue(id, out config)) return config; return null;\n";
            script += "        }\n";
        }
        else if (excelData->_Def->_MemberInfos[0]._TypeInfo._ValueType == eValueType::String) {
			script += "        public static " + excelData->_Def->_Name + " Get(string id) {\n";
			script += "            " + excelData->_Def->_Name + " config = null; if (Datas.TryGetValue(id, out config)) return config; return null;\n";
            script += "        }\n";
        }
        else {
            XASSERT(false, "wtf");
        }

        script += "    }\n";
        script += "}\n";
        return script;
    }

    bool CreateCSharpScripts(const std::string& path) {
        std::string autoEnumScript = "using System;\n\n";
        autoEnumScript += "namespace AutoConfig\n{\n";
        autoEnumScript += CreateCSharpEnumScript();
        autoEnumScript += "\n}\n";
        SaveFile(path + "/AutoEnum.cs", autoEnumScript);

        std::string autoConstScript = "using System;\n\n";
        autoConstScript += "namespace AutoConfig\n{\n";
        autoConstScript += CreateCSharpConstScript();
        autoConstScript += "\n}\n";
        SaveFile(path + "/AutoConst.cs", autoConstScript);

        for (auto itor = ExcelData::GetExcelDataMap().begin(); itor != ExcelData::GetExcelDataMap().end(); itor++) {
            std::string autoExcelScript = CreateExcelDataScript(itor->second);
            SaveFile(path + "/Auto" + itor->first + ".cs", autoExcelScript);
        }

        return true;
    }

}

