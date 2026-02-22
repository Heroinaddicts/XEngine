#ifndef __IConfig_h__
#define __IConfig_h__

#include "iModule.h"
#include "AutoConfig.h"
UsingXEngine;

#define GetConfig(config, Name, id) config->GetAutoConfig()->Query##Name##Config(id);

class IConfig : public Api::iModule {
public:
    virtual ~IConfig() {}

    virtual const AutoConfig::IAutoConfig* GetAutoConfig() const = 0;
};

#endif //__IConfig_h__
