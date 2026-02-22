#ifndef __Combat_h__
#define __Combat_h__

#include "Header.h"

class Combat : public ICombat {
public:
    virtual ~Combat() {}
private:
    // Í¨¹ý ICombat ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

};

#endif //__Combat_h__
