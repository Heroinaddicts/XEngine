#ifndef __TilemapManager_h__
#define __TilemapManager_h__

#include "Header.h"

class TilemapManager : public ITilemapManager {
public:
    virtual ~TilemapManager() {}

    void LoadIsometricTilemap(const std::string& path, const fIIsometricTilemapReturn& ret, const bool sync) override;

private:
    // Í¨¹ý ITilemapManager ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;
};

#endif //__TilemapManager_h__