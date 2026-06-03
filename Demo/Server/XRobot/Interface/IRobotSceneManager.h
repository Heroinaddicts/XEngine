#ifndef __IRobotSceneManager_h__
#define __IRobotSceneManager_h__

#include "iModule.h"
#include "AutoConfig.h"
UsingXEngine;

class ITileLayer;
class IRobotScene {
public:
    virtual ~IRobotScene() {}

    const UInt64 _SceneID;
    const AutoConfig::Struct::Scene* const _Config;
    ITileLayer* const _GroundTileLayer;

    IRobotScene(const UInt64 sceneId, const AutoConfig::Struct::Scene* const config, ITileLayer* tileLayout) : _SceneID(sceneId), _Config(config), _GroundTileLayer(tileLayout) {}
};

class IRobotSceneManager : public Api::iModule {
public:
    virtual ~IRobotSceneManager() {}

    virtual IRobotScene* QuerySceneByEntityGuid(const UInt64 sceneId) const = 0;
    virtual IRobotScene* QueryCharacterScene(const UInt64 characterId) const = 0;
};

#endif //__IRobotSceneManager_h__
