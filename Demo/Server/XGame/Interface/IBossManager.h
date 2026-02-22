#ifndef __IBossManager_h__
#define __IBossManager_h__

#include "iModule.h"
UsingXEngine;
#include "AutoConfig.h"
#include "Vector2.h"
#include <functional>

class IGameObject;
class IScene;

enum eBossState {
    Normal = 0,
    Enraged1 = 1,
    Enraged2 = 2,
};

class IBoss {
public:
    virtual ~IBoss() {}

    const AutoConfig::Enum::eBoss _Type;
    IGameObject* _BossGo;
    eBossState _BossState;

    IBoss(const AutoConfig::Enum::eBoss type, IGameObject* go, eBossState state) : _Type(type),_BossGo(go), _BossState(state){}
};

class IHydraCypress : public IBoss {
public:                                                                                                                                                                                                                                                            
    IHydraCypress() :IBoss(AutoConfig::Enum::eBoss::HydraCypress, nullptr, eBossState::Normal) {}

    virtual IGameObject* CreateBossHydraCypress(IScene* const scene, const Int64 configId, const Vector2& pos) = 0;
    virtual ~IHydraCypress() {}
};

class IBossManager : public Api::iModule {
public:
    virtual ~IBossManager() {}

    typedef std::function<void(IGameObject* const)> fInitBoss;
    virtual IGameObject* CreateBoss(IScene* const scene, const Int64 configId, const Vector2& pos, const fInitBoss& initer) = 0;
};

#endif //__IBossManager_h__
