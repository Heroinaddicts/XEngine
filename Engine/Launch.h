#ifndef __Launch_h__
#define __Launch_h__

#include <map>
#include <vector>
#include <string>

namespace XEngine {
    class Launch {
    public:
        virtual ~Launch() {}

        static Launch* GetInstance();

        bool AnalysisLaunchParameters(const int argc, const char** args, const char** env);
        const char* GetLaunchParameter(const std::string& name) const;

        const char* GetName() const;
        const std::vector<std::string>& GetModuleNames() const;

    private:
        Launch() : _Name("XEngine") {}

    private:
        std::map<std::string, std::string> _ParameterMap;
        std::vector<std::string> _ModuleNames;
        std::string _Name;
    };
}

#endif //__Launch_h__
