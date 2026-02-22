#ifndef __ITaskManager_h__
#define __ITaskManager_h__

#include "iModule.h"
UsingXEngine;

class ITaskManager : public Api::iModule {
public:
    virtual ~ITaskManager() {}

    virtual void AnalyzeNewTask(const UInt64 account) = 0;
    virtual void OnEquipWear(const UInt64 account, const UInt64 equipGuid) = 0;
};

#endif //__ITaskManager_h__
