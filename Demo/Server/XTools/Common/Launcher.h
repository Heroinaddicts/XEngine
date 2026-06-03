#ifndef __Launcher_h__
#define __Launcher_h__

#include <string>
#include "SafeString.h"
UsingXEngine;

class Launcher {
public:
    virtual ~Launcher() {}

    static Launcher* GetInstance();
    void ParseLauncherArgs(int argc, const char** argv);
    const char* GetLauncherArg(const std::string& name) const;

    std::string GetLauncherArgString(const std::string& name) const {
        return GetLauncherArg(name);
    }

    UInt64 GetLauncherArgUInt64(const std::string& name) const {
        return SafeString::StringToUInt64(GetLauncherArg(name));
    }

    Int64 GetLauncherArgInt64(const std::string& name) const {
        return SafeString::StringToInt64(GetLauncherArg(name));
    }

    UInt32 GetLauncherArgUInt32(const std::string& name) const {
        return SafeString::StringToUInt32(GetLauncherArg(name));
    }

    Int32 GetLauncherArgInt32(const std::string& name) const {
        return SafeString::StringToInt32(GetLauncherArg(name));
    }

private:
    Launcher() {};
};

#define GetLauncherParm(Type, name) Launcher::GetInstance()->GetLauncherArg##Type(name);

#endif //__Launcher_h__
