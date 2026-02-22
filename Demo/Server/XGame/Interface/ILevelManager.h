#ifndef __ILevelManager_h__
#define __ILevelManager_h__

#include "iModule.h"
UsingXEngine;

class IGameObject;
class ILevelManager : public Api::iModule {
public:
    virtual ~ILevelManager() {}

    enum class eEvent {
        LevelUp
    };

    typedef void(*fEvent)(IGameObject* const, const UInt64 oldLevel, const UInt64 newLevel);
    virtual void RegistEvent(const eEvent ev, const fEvent fun, const char * file = __FILE__, const int line = __LINE__) = 0;
};

#endif //__ILevelManager_h__
