#ifndef __RobotScene_h__
#define __RobotScene_h__

#include "Header.h"

class RobotScene : public IRobotScene {
public:
    virtual ~RobotScene() {}

    RobotScene(const UInt64 sceneId, const AutoConfig::Struct::Scene* const config, ITileLayer* layer) : IRobotScene(sceneId, config, layer) {}
};

#endif //__RobotScene_h__
