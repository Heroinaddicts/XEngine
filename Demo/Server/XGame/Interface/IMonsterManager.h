#ifndef __IMonsterManager_h__
#define __IMonsterManager_h__

#include "iModule.h"
#include "Vector2.h"
UsingXEngine;
#include <functional>

class IGameObject;
class IScene;
class IMonsterManager : public Api::iModule {
public:
    virtual ~IMonsterManager() {}

    typedef std::function<void(IGameObject* const)> fInitMonster;
    virtual IGameObject* CreateMonster(IScene* const scene, const Int64 configId, const Vector2& pos, const fInitMonster& initer) = 0;
};

#endif //__IMonsterManager_h__
