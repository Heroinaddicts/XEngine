#include "Header.h"
#include <map>
#include <string>
#include "SafeString.h"
#include "SafeSystem.h"
#include "XFile.h"

using namespace XEngine;

static std::map<std::string, std::string> s_ParameterMap;

int main(int argc, const char** args, const char** env) {
    for (int i = 1; i < argc; ++i) {
        if (strncmp(args[i], "--", 2) == 0) {
            const char* start = args[i] + 2;
            const char* equal = strstr(start, "=");
            if (equal != nullptr) {
                std::string name(start, equal);
                std::string val(equal + 1);
                s_ParameterMap[name] = val;
            }
            else if (strlen(args[i]) > 2) {
                s_ParameterMap[args[i] + 2] = "";
            }
        }
        else {
            return false;
        }
    }

    std::string header_h_file_content = header_h;
    std::string interface_h_file_content = interface_h;
    std::string template_h_file_content = template_h;
    std::string template_cpp_file_content = template_cpp;
    std::string dllexport_cpp_content = dllexport_cpp;
    std::string cmake_file_content = cmakelists;

    SafeString::Replace(header_h_file_content, "{name}", s_ParameterMap["module_name"]);
    SafeString::Replace(interface_h_file_content, "{name}", s_ParameterMap["module_name"]);
    SafeString::Replace(template_h_file_content, "{name}", s_ParameterMap["module_name"]);
    SafeString::Replace(template_cpp_file_content, "{name}", s_ParameterMap["module_name"]);
    SafeString::Replace(dllexport_cpp_content, "{name}", s_ParameterMap["module_name"]);
    SafeString::Replace(cmake_file_content, "{name}", s_ParameterMap["module_name"]);
    SafeString::Replace(cmake_file_content, "{auther}", s_ParameterMap["auther"]);
    SafeString::Replace(cmake_file_content, "{date}", SafeSystem::Time::GetCurrentTimeString());
    SafeString::Replace(cmake_file_content, "{group}", s_ParameterMap["group"]);

    std::string path = SafeSystem::File::GetApplicationPath();
    auto itor = s_ParameterMap.find("path");
    if (s_ParameterMap.end() != itor) {
        path += "/" + itor->second;
    }

    std::string module_dir = path + "/" + s_ParameterMap["group"] + "/" + s_ParameterMap["module_name"];
    if (SafeSystem::File::FolderExists(module_dir)) {
        XASSERT(false, "%s is exists", module_dir.c_str());
        return false;
    }

    std::string interface_dir = path + "/Interface";

    SafeSystem::File::CreateFolder(path);
    SafeSystem::File::CreateFolder(interface_dir);
    SafeSystem::File::CreateFolder(module_dir);

    SafeSystem::File::DelFile(module_dir + "/Header.h");
    XFile header_h_file;
    if (!header_h_file.Open(module_dir + "/Header.h")) {
        XASSERT(false, "wtf");
        return false;
    }
    header_h_file << header_h_file_content;

    SafeSystem::File::DelFile(interface_dir + "/i" + s_ParameterMap["module_name"] + ".h");
    XFile interface_h_file;
    if (!interface_h_file.Open(interface_dir + "/i" + s_ParameterMap["module_name"] + ".h")) {
        XASSERT(false, "wtf");
        return false;
    }
    interface_h_file << interface_h_file_content;

    SafeSystem::File::DelFile(module_dir + "/" + s_ParameterMap["module_name"] + ".h");
    XFile template_h_file;
    if (!template_h_file.Open(module_dir + "/" + s_ParameterMap["module_name"] + ".h")) {
        XASSERT(false, "wtf");
        return false;
    }
    template_h_file << template_h_file_content;

    SafeSystem::File::DelFile(module_dir + "/" + s_ParameterMap["module_name"] + ".cpp");
    XFile template_cpp_file;
    if (!template_cpp_file.Open(module_dir + "/" + s_ParameterMap["module_name"] + ".cpp")) {
        XASSERT(false, "wtf");
        return false;
    }
    template_cpp_file << template_cpp_file_content;

    SafeSystem::File::DelFile(module_dir + "/DllExport.cpp");
    XFile dllexport_cpp_file;
    if (!dllexport_cpp_file.Open(module_dir + "/DllExport.cpp")) {
        XASSERT(false, "wtf");
        return false;
    }
    dllexport_cpp_file << dllexport_cpp_content;

    SafeSystem::File::DelFile(module_dir + "/CMakeLists.txt");
    XFile cmake_file;
    if (!cmake_file.Open(module_dir + "/CMakeLists.txt")) {
        XASSERT(false, "wtf");
        return false;
    }
    cmake_file << cmake_file_content;

    XFile cmakelists;
    if (!cmakelists.Open(path + "/" + s_ParameterMap["group"] + "/CMakeLists.txt")) {
        XASSERT(false, "wtf");
        return false;
    }

    std::string add_subdirectory = "\nADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/{gourp}/{name})";
    SafeString::Replace(add_subdirectory, "{name}", s_ParameterMap["module_name"]);
    SafeString::Replace(add_subdirectory, "{gourp}", s_ParameterMap["group"]);
    cmakelists << add_subdirectory;

    cmakelists.Save();
    cmakelists.Close();

    header_h_file.Save();
    header_h_file.Close();

    interface_h_file.Save();
    interface_h_file.Close();

    template_h_file.Save();
    template_h_file.Close();

    template_cpp_file.Save();
    template_cpp_file.Close();

    dllexport_cpp_file.Save();
    dllexport_cpp_file.Close();

    cmake_file.Save();
    cmake_file.Close();
    return 0;
}
