#ifndef __SkillBullet_h__
#define __SkillBullet_h__

#include "Header.h"

class SkillBullet : public ISkillBullet, public Api::iTimer {
public:
	virtual ~SkillBullet() {}

private:
	
	UInt64 AddBullet(IGameObject* const launcher, const Int64 bulletID) override;
	void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

	void OnBulletDamage(IGameObject* const bullet, IGameObject* const target);
	void OnBulletRelease(IGameObject* bullet);
	void OnBulletDamageCheck(IGameObject* const bullet, IGameObject* const target, bool& isBulletRelease);

	bool BulletCollisionCheck(IGameObject* const bullet, IGameObject* const go);

	static void OnSceneCreate(IScene* const scene);
	static void OnSceneRelease(IScene* const scene);
private:
	bool Initialize(Api::iEngine* const engine) override;
	bool Launch(Api::iEngine* const engine) override;
	bool Destroy(Api::iEngine* const engine) override;
};


#endif // !__SkillBullet_h__
