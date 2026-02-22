#ifndef __Config_h__
#define __Config_h__

#include "Header.h"

class Config : public IConfig {
public:
    virtual ~Config() {}
private:
    // 通过 IConfig 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    // 通过 IConfig 继承
    const AutoConfig::IAutoConfig* GetAutoConfig() const override;                                                                      

};
                                                                  
#endif //__Config_h__
