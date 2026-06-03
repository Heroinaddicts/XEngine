#include "Header.h"
#include "Launcher.h"
#include "SafeString.h"
#include "SafeSystem.h"
#include "XDumper.h"

#include "AutoEnum.h"
#include "AutoConst.h"
#include "AutoStruct.h"
#ifdef WIN32
class AttachDumper {
public:
    AttachDumper() {
        XEngine::XDumper::GetInstance().SetDumpFilename((SafeSystem::Time::GetCurrentTimeString("Dump/%4d_%02d_%02d_%02d_%02d_%02d").append(".dmp")).c_str());
    }
};
static AttachDumper dumper;
#endif //WIN32

std::map<std::string, AutoConfigEnum*> g_AutoConfigEnumMap;
std::map<std::string, AutoConfigConst*> g_AutoConfigConstMap;
std::map<std::string, const std::string> g_CodeTemplates;
std::map<std::string, const AutoConfigStruct*> g_AutoConfigData;

std::map<std::string, const  Excel::Sheet*> g_DataExcel;

int main(int argc, const char** argv) {
    Launcher::GetInstance()->ParseLauncherArgs(argc, argv);
    if (Launcher::GetInstance()->GetLauncherArg("Pause")) {
        getchar();
    }

    //     std::string xmlPath = Launcher::GetInstance()->GetLauncherArg("Xml");
    // 

    std::string templatePath = Launcher::GetInstance()->GetLauncherArg("Template");
    {
        std::vector<std::string> paths, names;
        SafeSystem::File::GetFileInDirectory(templatePath, ".template", paths, names);
        for (int i = 0; i < names.size(); i++)
            g_CodeTemplates.insert({ names[i], SafeSystem::File::LoadFileAsString(paths[i]) });
    }

    std::string excelPath = Launcher::GetInstance()->GetLauncherArg("Excel");
    Excel::Book* excel = new Excel::Book(excelPath + "/AutoConfig.xls");
    ReadAutoEnum(excel->sheet(0));
    ReadAutoConst(excel->sheet(1));
    std::vector<std::string> names, paths;
    SafeSystem::File::GetFileInDirectory(excelPath, ".xls", paths, names);
    for (int i = 0; i < paths.size(); i++) {
        if (names[i] != "AutoConfig") {
            Excel::Book* excel = new Excel::Book(paths[i]);
            g_DataExcel.insert({ names[i], excel->sheet(0) });
        }
    }
    GeneratAutoStruct();

    std::string autoConfigString = g_CodeTemplates["AutoConfig"];
    SafeString::Replace2(autoConfigString, "{AutoEnums}", GeneratAutoEnumCode());
    SafeString::Replace2(autoConfigString, "{AutoConsts}", GeneratAutoConstCode());
    SafeString::Replace2(autoConfigString, "{AutoConfigs}", GeneratAutoStructCode());

    std::string interfaceCode;
    for (auto i = g_AutoConfigData.begin(); i != g_AutoConfigData.end(); i++) {
        interfaceCode += g_CodeTemplates["Interface"];
        const AutoConfigStruct* pStruct = i->second;
        SafeString::Replace2(interfaceCode, "{StructName}", pStruct->_Name);
    }
    SafeString::Replace2(autoConfigString, "{Interfaces}", interfaceCode);

    std::string cppCodePath = Launcher::GetInstance()->GetLauncherArg("CppCode");
    SafeSystem::File::SaveStringToFile(cppCodePath, autoConfigString);


    std::string autoConfigDataString = g_CodeTemplates["AutoConfigData"];
    {
        std::string autoConfigDataInterfacesString, autoConfigDataMembersString, autoConfigDataInitializesString;
        for (auto itor = g_AutoConfigData.begin(); itor != g_AutoConfigData.end(); itor++) {
            const AutoConfigStruct* pStruct = itor->second;
            autoConfigDataInterfacesString += g_CodeTemplates["AutoConfigDataInterface"];
            SafeString::Replace2(autoConfigDataInterfacesString, "{StructName}", pStruct->_Name);

            autoConfigDataMembersString += g_CodeTemplates["AutoConfigDataMember"];
            SafeString::Replace2(autoConfigDataMembersString, "{StructName}", pStruct->_Name);

            for (int i = 0; i < pStruct->_Rows.size(); i++) {
                autoConfigDataInitializesString += g_CodeTemplates["AutoConfigDataInitializ"];
                SafeString::Replace2(autoConfigDataInitializesString, "{StructName}", pStruct->_Name);
                SafeString::Replace2(autoConfigDataInitializesString, "{Key}", pStruct->_Rows[i][0]->_Value);

                std::string autoConfigDataInitializArgsString;
                for (int column = 0; column < pStruct->_Rows[i].size(); column++) {
                    const AutoConfigStruct::ColumnValue* pColumnValue = pStruct->_Rows[i][column];
                    autoConfigDataInitializArgsString
                        += GetValueCode(pColumnValue->_Info->_Type, pColumnValue->_Value, SafeString::Replace(pColumnValue->_Info->_TypeString, "[]", "")) + (column != pStruct->_Rows[i].size() - 1 ? ", " : "");
                }
                SafeString::Replace2(autoConfigDataInitializesString, "{AutoConfigDataInitializArgs}", autoConfigDataInitializArgsString);
            }
        }
        SafeString::Replace2(autoConfigDataString, "{AutoConfigDataInterfaces}", autoConfigDataInterfacesString);
        SafeString::Replace2(autoConfigDataString, "{AutoConfigDataMembers}", autoConfigDataMembersString);
        SafeString::Replace2(autoConfigDataString, "{AutoConfigDataInitializes}", autoConfigDataInitializesString);
    }

    std::string cppDataCodePath = Launcher::GetInstance()->GetLauncherArg("CppDataCode");
    SafeSystem::File::SaveStringToFile(cppDataCodePath, autoConfigDataString);
    return 0;
}
