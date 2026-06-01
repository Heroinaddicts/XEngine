#ifndef __TestMemory_h__
#define __TestMemory_h__

#include "Header.h"

class TestMemory : public ITestMemory {
public:
    virtual ~TestMemory() {}

private:
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;
};

#endif //__TestMemory_h__
