#ifndef __LevelManager_h__
#define __LevelManager_h__

#include "Header.h"

class LevelManager : public ILevelManager {
public:
    virtual ~LevelManager() {}
private:
    // Í¨¹ý ILevelManager ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
	bool Destroy(Api::iEngine* const engine) override;
	void RegistEvent(const eEvent ev, const fEvent fun, const char * file, const int line) override;

private:
	static void OnMonsterBossEnterDeath(const UInt64, const AutoConfig::Enum::eState, const Vector2&, const float);
};

#endif //__LevelManager_h__
