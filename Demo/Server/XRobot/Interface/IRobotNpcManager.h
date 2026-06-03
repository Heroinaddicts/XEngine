#ifndef __IRobotNpcManager_h__
#define __IRobotNpcManager_h__

#include "iModule.h"
#include "Vector2.h"
#include <unordered_map>
UsingXEngine;


class IRobotNpc {
public:
    virtual ~IRobotNpc() {}

    const UInt64 _ConfigID;
    const UInt64 _SceneID;

    virtual const Vector2& Position() const = 0;

    IRobotNpc(const UInt64 configId, const UInt64 sceneId) : _ConfigID(configId), _SceneID(sceneId) {}
};

class IRobotNpcManager : public Api::iModule {
public:
    virtual ~IRobotNpcManager() {}

    virtual const std::unordered_map<Int64, IRobotNpc* const>& GetNpc(const UInt64 sceneId) const = 0;
};

#endif //__IRobotNpcManager_h__
