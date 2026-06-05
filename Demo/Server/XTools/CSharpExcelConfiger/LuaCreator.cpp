#include "LuaCreator.h"
#include "Enum.h"
#include "Const.h"
#include "Array.h"
#include "ExcelData.h"
#include "XFile.h"

namespace ExcelConfiger {

    static std::string s_ConfigScript;
    static std::string s_MiniStudioJsonTemplate;

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

    __forceinline std::string CreateLuaEnumScript(const std::string& path) {
        std::string script;
        const std::map<std::string, Enum*>& enums = Enum::GetEnums();

        for (auto itorEnum = enums.begin(); itorEnum != enums.end(); itorEnum++) {
            script += "    " + itorEnum->first + " = {\n";

            const std::map<std::string, Value*>& values = itorEnum->second->GetValueMap();

            for (auto itorValue = values.begin(); itorValue != values.end(); itorValue++) {
                script += "        " + itorValue->first + " = " + SafeString::Int64ToString(itorValue->second->_Int) + ", --" + itorValue->second->_Desc + "\n";
            }

            script += "    },\n\n";
        }

        return script;
    }

    __forceinline std::string CreateLuaConstScript(const std::string& path) {
        std::string script;

        const std::map<std::string, Const*>& consts = Const::GetConsts();
        for (auto itorConsts = consts.begin(); itorConsts != consts.end(); itorConsts++) {
            script += "    " + itorConsts->first + " = { --" + itorConsts->second->_Desc + "\n";

            for (auto itorValue = itorConsts->second->_Values.begin(); itorValue != itorConsts->second->_Values.end(); itorValue++) {
                script += "        " + itorValue->first + " = ";

                if (itorValue->second->_Info._IsArray) {
                    script += "{";
                    for (int i = 0; i < itorValue->second->_Array->_Size; i++) {
                        switch (itorValue->second->_Info._ValueType) {
                        case eValueType::Int: {
                            script += SafeString::Int64ToString(itorValue->second->_Array->GetValue(i)->_Int) + ", ";
                            break;
                        }
                        case eValueType::Float: {
                            script += SafeString::FloatToString(itorValue->second->_Array->GetValue(i)->_Float) + ", ";
                            break;
                        }
                        case eValueType::String: {
                            script += "\'" + std::string(itorValue->second->_Array->GetValue(i)->_String) + "\', ";
                            break;
                        }
                        case eValueType::Bool: {
                            script += SafeString::BoolToString(itorValue->second->_Array->GetValue(i)->_BoolValue) + ", ";
                            break;
                        }
                        case eValueType::Enum: {
                            script += "AutoConfig.Enum." + itorValue->second->_Info._TypeString + "." + itorValue->second->_Array->GetValue(i)->_ValueString + ", ";
                            break;
                        }
                        case eValueType::Const: {
                            script += itorValue->first + "." + itorValue->second->_Array->GetValue(i)->_ValueString + ", ";
                            break;
                        }
                        default:
                            XASSERT(false, "wtf");
                            break;
                        }
                    }
                    script += "}, --" + itorValue->second->_Desc + "\n";
                }
                else {
                    switch (itorValue->second->_Info._ValueType) {
                    case eValueType::Int: {
                        script += SafeString::Int64ToString(itorValue->second->_Int) + ", --" + itorValue->second->_Desc + "\n";
                        break;
                    }
                    case eValueType::Float: {
                        script += SafeString::FloatToString(itorValue->second->_Float) + ", --" + itorValue->second->_Desc + "\n";
                        break;
                    }
                    case eValueType::String: {
                        script += "\'" + std::string(itorValue->second->_String) + "\', --" + itorValue->second->_Desc + "\n";
                        break;
                    }
                    case eValueType::Bool: {
                        script += SafeString::BoolToString(itorValue->second->_BoolValue) + ", --" + itorValue->second->_Desc + "\n";
                        break;
                    }
                    case eValueType::Enum: {
                        script += "function() return AutoConfig.Enum." + itorValue->second->_Info._TypeString + "." + itorValue->second->_ValueString + ", --" + itorValue->second->_Desc + " end\n";
                        break;
                    }
                    case eValueType::Const: {
                        script += itorValue->second->_Info._TypeString + "." + itorValue->second->_ValueString + ", --" + itorValue->second->_Desc + "\n";
                        break;
                    }
                    case eValueType::Script: {
                        script += std::string(itorValue->second->_String) + ", --" + itorValue->second->_Desc + "\n";
                        break;
                    }
                    default:
                        XASSERT(false, "wtf");
                        break;
                    }
                }
            }

            script += "    },\n\n";
        }

        return script;
    }

    __forceinline std::string CreateExcelDataScript(const std::string& path, ExcelData* ed) {
        std::string script = "AutoConfig." + ed->_Def->_Name + " = {\n";

        for (int row = 0; row < ed->_Values.size(); row++) {
            std::string key;
            std::string s = "{";
            for (int column = 0; column < ed->_Values[row].size(); column++) {
                if (ed->_Def->_MemberInfos[column]._IsKey) {
                    switch (ed->_Def->_MemberInfos[column]._TypeInfo._ValueType) {
                    case eValueType::Int:
                        key = "[" + ed->_Values[row][column]->_ValueString + "]";
                        break;
                    case eValueType::String: {
                        key = "[\'" + ed->_Values[row][column]->_ValueString + "\']";
                        break;
                    default:
                        XASSERT(false, "wtf");
                        return "";
                    }
                    }
                }

                s += ed->_Def->_MemberInfos[column]._Name + " = ";

                if (ed->_Values[row][column]->_Info._IsArray) {
                    s += "{";
                    std::vector<std::string> ret;
                    int count = SafeString::Split(ed->_Values[row][column]->_ValueString, ARRAY_SPLIT_TAG, ret);
                    switch (ed->_Values[row][column]->_Info._ValueType) {
                    case eValueType::Int:
                    case eValueType::Float:
                    case eValueType::Bool: {
                        for (int i = 0; i < count; i++) {
                            s += ret[i] + ", ";
                        }
                        break;
                    }
                    case eValueType::String: {
                        for (int i = 0; i < count; i++) {
                            s += "\'" + ret[i] + "\', ";
                        }
                        break;
                    }
                    case eValueType::Enum: {
                        for (int i = 0; i < count; i++) {
                            s += "AutoConfig.Enum." + ed->_Values[row][column]->_Info._TypeString + "." + ret[i] + ", ";
                        }
                        break;
                    }
                    case eValueType::Const: {
                        for (int i = 0; i < count; i++) {
                            s += "AutoConfig.Const." + ed->_Values[row][column]->_Info._TypeString + "." + ret[i] + ", ";
                        }
                        break;
                    }
                    case eValueType::Table: {
                        for (int i = 0; i < count; i++) {
                            s += "function() return AutoConfig." + ret[i] + " end, ";
                        }
                        break;
                    }
                    }
                    s += "}, ";
                }
                else {
                    switch (ed->_Values[row][column]->_Info._ValueType) {
                    case eValueType::Int:
                    case eValueType::Float:
                    case eValueType::Bool: {
                        s += ed->_Values[row][column]->_ValueString + ", ";
                        break;
                    }
                    case eValueType::String: {
                        s += "\'" + ed->_Values[row][column]->_ValueString + "\', ";
                        break;
                    }
                    case eValueType::Enum: {
                        s += "AutoConfig.Enum." + ed->_Values[row][column]->_Info._TypeString + "." + ed->_Values[row][column]->_ValueString + ", ";
                        break;
                    }
                    case eValueType::Const: {
                        s += "AutoConfig.Const." + ed->_Values[row][column]->_Info._TypeString + "." + ed->_Values[row][column]->_ValueString + ", ";
                        break;
                    }
                    case eValueType::Table: {
                        s += "function() return AutoConfig." + ed->_Values[row][column]->_ValueString + " end, ";
                        break;
                    }
                    case eValueType::Script: {
                        s += ed->_Values[row][column]->_ValueString + ", ";
                        break;
                    }
                    }
                }
            }

            if (row == ed->_Values.size() - 1) {
                s += "}\n";
            }
            else {
                s += "}, \n";
            }

            script += +"    " + key + " = " + s;
        }

        script += "}\n";

        SaveFile(path + "/Config/" + ed->_Def->_Name + ".lua", script);
        std::string json = SafeString::Replace(s_MiniStudioJsonTemplate, "${Name}", ed->_Def->_Name);
        SaveFile(path + "/Config/" + ed->_Def->_Name + ".json", json);
        return script;
    }

    void CreateExcelDataScripts(const std::string& path) {
        const std::map<std::string, ExcelData*>& eds = ExcelData::GetExcelDataMap();
        for (auto itor = eds.begin(); itor != eds.end(); itor++) {
            CreateExcelDataScript(path, itor->second);
        }
    }

    bool CreateLuaScripts(const std::string& path) {
        XFile file;
        if (!file.Open(SafeSystem::File::GetDirectoryOfCurrentExe() + "/MiniStudioJsonTemplate.json")) {
            XASSERT(false, "wtf");
            return false;
        }

        s_MiniStudioJsonTemplate = file.ReadToString();

        std::string configDefineScript = "AutoConfig = {}\n";
        configDefineScript += "AutoConfig.Enum = {\n";
        configDefineScript += CreateLuaEnumScript(path);
        configDefineScript += "}\n\n";
        configDefineScript += "AutoConfig.Const = {\n";
        configDefineScript += CreateLuaConstScript(path);
        configDefineScript += "}\n";
        SaveFile(path + "/Config/ConfigDefine.lua", configDefineScript);

        CreateExcelDataScripts(path);

        s_ConfigScript = "require(script.ConfigDefine)\n";
        s_ConfigScript += "local scripts = script.Children\n";
        s_ConfigScript += "for _, v in ipairs(scripts) do\n";
        s_ConfigScript += "    if v.Name ~= 'ConfigDefine' then\n";
        s_ConfigScript += "        require(v)\n";
        s_ConfigScript += "    end\n";
        s_ConfigScript += "end\n";
        s_ConfigScript += "print('Load Config Success')\n\n";
        SaveFile(path + "/Config.lua", s_ConfigScript);

        return true;
    }

}
