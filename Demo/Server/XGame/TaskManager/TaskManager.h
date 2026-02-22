#ifndef __TaskManager_h__
#define __TaskManager_h__

#include "Header.h"

class TaskManager : public ITaskManager {
public:
    virtual ~TaskManager() {}
private:
    // Í¨¹ý IBackpack ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    static void OnNewPlayerCreated(IPlayer* const player, const UInt64 account);
    static void OnPlayerCreated(IPlayer* const player, const UInt64 account);
    static void OnPlayerRelease(IPlayer* const player, const UInt64 account);

    static void OnAcceptTaskReq(IPlayer* const player, const UInt64 account, const oAcceptTaskreq& req);
    static void OnSubmitTaskReq(IPlayer* const player, const UInt64 account, const oSubmitTaskReq& req);
    static void OnCompleteTask(IPlayer* const player, const UInt64 account, const oCompleteTask& req);
    static void OnNpcContact(IPlayer* const player, const UInt64 account, const oNpcContact& req);

    void SaveTaskData(const UInt64 account);
    void TaskStatusChange(const UInt64 account, const Int64 taskId, const AutoConfig::Enum::eTaskEvent stauts);


    void AnalyzeNewTask(const UInt64 account) override;
    void OnEquipWear(const UInt64 account, const UInt64 equipGuid) override;
};

#endif //__TaskManager_h__
