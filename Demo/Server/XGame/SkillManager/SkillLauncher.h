#ifndef __SkillLauncher_h__
#define __SkillLauncher_h__

#include "Header.h"

class SkillLauncher :public ISkillLauncher, public Api::iTimer {
public:
	virtual ~SkillLauncher() {}

	void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

private:
	UInt64 AddLauncher(IGameObject* const cast, const Int64 launcherID, const float angle, const Vector2& targetPos, const Vector2& placePos, const UInt64 targetGuid) override;
private:
	static void OnLauncherTest(IPlayer* const player, const UInt64 account, const oLauncherTest& req);
	bool Initialize(Api::iEngine* const engine) override;
	bool Launch(Api::iEngine* const engine) override;
	bool Destroy(Api::iEngine* const engine) override;
	static void OnGameEntityCreate(IGameObject* const go);
	static void OnGameEntityRelease(IGameObject* const go);

	static void OnCharacterDockerLoaded(ICharacterDocker* const docker, IGameObject* const gameObject);

	static void OnCharacterLoaded(ICharacterDocker* const docker, IGameObject* const character);
	static void OnCharacterCreated(ICharacterDocker* const docker, IGameObject* const character);
	static void OnCharacterRelease(ICharacterDocker* const docker, IGameObject* const character);
};

#endif // !__SkillLauncher_h__
