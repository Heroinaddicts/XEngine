#include "Header.h"
#include "Enum.h"
#include "Const.h"
#include "ExcelData.h"
#include "Launcher.h"
#include "SafeString.h"
#include "SafeSystem.h"

#include "LuaCreator.h"
#include "CSharpCreator.h"
#include "XDumper.h"
#ifdef WIN32
class AttachDumper {
public:
    AttachDumper() {
        XEngine::XDumper::GetInstance().SetDumpFilename((SafeSystem::Time::GetCurrentTimeString("Dump/%4d_%02d_%02d_%02d_%02d_%02d").append(".dmp")).c_str());
    }
};
static AttachDumper dumper;
#endif //WIN32
int main(int argc, const char** argv) {
    Launcher::GetInstance()->ParseLauncherArgs(argc, argv);
    if (Launcher::GetInstance()->GetLauncherArg("Pause")) {
        getchar();
    }

    std::string excelPath = SafeSystem::File::GetDirectoryOfCurrentExe() + "/" + Launcher::GetInstance()->GetLauncherArg("Excel");

    Excel::Book* excel = new Excel::Book(excelPath + "/AutoConfig.xls");
    ExcelConfiger::Enum::LoadEnum(excel->sheet(0));
    ExcelConfiger::Const::LoadConst(excel->sheet(1));

    std::vector<std::string> names, paths;
    SafeSystem::File::GetFileInDirectory(excelPath, ".xls", paths, names);

    for (int i = 0; i < paths.size(); i++) {
        if (names[i] != "AutoConfig") {
            excel = new Excel::Book(paths[i]);
            ExcelConfiger::ExcelData::LoadExcel(names[i], excel->sheet(0));
        }
    }

    if (Launcher::GetInstance()->GetLauncherArg("LuaScripts")) {
        ExcelConfiger::CreateLuaScripts(SafeSystem::File::GetDirectoryOfCurrentExe() + "/" + Launcher::GetInstance()->GetLauncherArg("LuaScripts"));
    }

    if (Launcher::GetInstance()->GetLauncherArg("CSharpScripts")) {
        ExcelConfiger::CreateCSharpScripts(SafeSystem::File::GetDirectoryOfCurrentExe() + "/" + Launcher::GetInstance()->GetLauncherArg("CSharpScripts"));
    }

    return 0;
}
