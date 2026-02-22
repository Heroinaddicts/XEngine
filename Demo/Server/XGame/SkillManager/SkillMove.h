
#ifndef __SkillMove_h__
#define __SkillMove_h__

#include "Header.h"

class SkillMove : public ISkillMove, public Api::iTimer {
public:
	virtual ~SkillMove() {}

	void OnTimer(const int id, const UInt64 context, const Int64 timestamp);

private:

	static void OnGameEntityCreate(IGameObject* const go);
	static void OnGameEntityRelease(IGameObject* const go);

	static void OnFsmEvent(const IFsm::eFsmEvent ev, const UInt64 guid, const AutoConfig::Enum::eState initState);
	static bool OnExitShiftJudge(const UInt64 guid, const AutoConfig::Enum::eState, const Vector2&, const float);

	static void OnCantControllerRefChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Int32 oldValue, const Int32 newValue);

	void OnMoveDamage(IGameObject* const cast, IGameObject* const target, MoveData* const moveData);
	void OnMoveRelease(IGameObject* cast);
	void OnMoveDamageCheck(IGameObject* const cast, IGameObject* const target, bool& isMoveRelease, MoveData* const moveData);
	UInt64 AddMove(IGameObject* const cast, const UInt64 skillGuid, const Int64 skillConfig, const Int64 moveID, const float angle = NULL, const UInt64 targetGuid = INVALID_GAMEOBJECT_ID) override;

	bool Initialize(Api::iEngine* const engine) override;
	bool Launch(Api::iEngine* const engine) override;
	bool Destroy(Api::iEngine* const engine) override;
};

#endif // !__SkillMove_h__
