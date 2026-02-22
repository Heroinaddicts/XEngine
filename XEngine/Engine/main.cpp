#pragma execution_character_set("utf-8")

#include "Launch.h"
#include "Engine.h"
#include "Logic/Logic.h"
#include "Log/Log.h"
#include "TimeWheel/TimeWheel.h"
#include "Http/Http.h"
#include "Mysql/Mysql.h"
#include "Task/TaskManager.h"

#ifdef WIN32
#include "Net/Windows/Net.h"
#endif //WIN32

#ifdef Linux
#include "Net/Linux/Net.h"
#endif //Linux
UsingXEngine;

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

int main(int argc, const char** args, const char** env) {
#ifdef _WIN32
#pragma comment(lib, "winmm.lib")
    timeBeginPeriod(1);
    // Windows 下设置控制台程序输出代码页为 UTF8
    auto old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);
#endif //_WIN32

    Launch* const launch = Launch::GetInstance();

    if (!launch->AnalysisLaunchParameters(argc, args, env)) {
        XASSERT(false, "AnalysisLaunchParameters error");
        return 0;
    }

    if (XEngine::Engine::GetInstance()->GetLaunchParameter("pause")) {
        getchar();
    }


    iLogic* const logic = Logic::GetInstance();
    iLog* const log = Log::GetInstance();
    iNet* const net = Net::GetInstance();
    iHttp* const http = Http::GetInstance();
    iTimeWheel* const time = XEngine::TimeWheel::GetInstance();
    //iPhysics* const physics = Physics::GetInstance();
    iMysql* mysql = Mysql::GetInstance();
    iTaskManager* const task = TaskManager::GetInstance();


    { // Initialize
        log->Initialize(XEngine::g_Engine);
        time->Initialize(XEngine::g_Engine);
        net->Initialize(XEngine::g_Engine);
        http->Initialize(XEngine::g_Engine);
        mysql->Initialize(XEngine::g_Engine);
        //physics->Initialize(XEngine::g_Engine);
        task->Initialize(XEngine::g_Engine);
        logic->Initialize(XEngine::g_Engine);
    }

    { // Launch
        log->Launch(XEngine::g_Engine);
        time->Launch(XEngine::g_Engine);
        net->Launch(XEngine::g_Engine);
        http->Launch(XEngine::g_Engine);
        mysql->Launch(XEngine::g_Engine);
        //physics->Launch(XEngine::g_Engine);
        task->Launch(XEngine::g_Engine);
        logic->Launch(XEngine::g_Engine);
    }

    UInt64 fixedUpdateTick = XEngine::SafeSystem::Time::GetMicroSecond();
    while (!XEngine::g_Engine->isShutdown()) {
        UInt64 use = XEngine::SafeSystem::Time::GetMicroSecond();
        log->EarlyUpdate(XEngine::g_Engine);
        time->EarlyUpdate(XEngine::g_Engine);
        net->EarlyUpdate(XEngine::g_Engine);
        http->EarlyUpdate(XEngine::g_Engine);
        mysql->EarlyUpdate(XEngine::g_Engine);
        //physics->EarlyUpdate(XEngine::g_Engine);
        task->EarlyUpdate(XEngine::g_Engine);
        logic->EarlyUpdate(XEngine::g_Engine);

        log->Update(XEngine::g_Engine);
        time->Update(XEngine::g_Engine);
        net->Update(XEngine::g_Engine);
        http->Update(XEngine::g_Engine);
        //physics->Update(XEngine::g_Engine);
        mysql->Update(XEngine::g_Engine);
        task->Update(XEngine::g_Engine);
        logic->Update(XEngine::g_Engine);

        time->LaterUpdate(XEngine::g_Engine);
        net->LaterUpdate(XEngine::g_Engine);
        http->LaterUpdate(XEngine::g_Engine);
        log->LaterUpdate(XEngine::g_Engine);
        mysql->LaterUpdate(XEngine::g_Engine);
        //physics->LaterUpdate(XEngine::g_Engine);
        task->LaterUpdate(XEngine::g_Engine);
        logic->LaterUpdate(XEngine::g_Engine);
        if (SafeSystem::Time::GetMicroSecond() - use < 100) {
            SafeSystem::Time::MillisecondSleep(1);
        }
    }

    { // Release
        logic->Release(XEngine::g_Engine);
        time->Release(XEngine::g_Engine);
        net->Release(XEngine::g_Engine);
        http->Release(XEngine::g_Engine);
        log->Release(XEngine::g_Engine);
        mysql->Release(XEngine::g_Engine);
        //physics->Release(XEngine::g_Engine);
        task->Release(XEngine::g_Engine);
    }

#ifdef _WIN32
    SetConsoleOutputCP(old_cp);
#endif
    return 0;
}
