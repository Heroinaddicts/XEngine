#ifndef __GameWorld_h__
#define __GameWorld_h__


#include "Header.h"

class GameWorld : public IGameWorld {
public:
    virtual ~GameWorld() {}

private:
    // 通过 IGameWorld 继承
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

    void SetGuidAlloca(UInt64(*fun)()) override;
    void RegistGameObjectEvent(const eGameObjectType type, const eGameWorldEvent ev, fGameObjectEvent callback) override;
    IGameObject* CreateGameObject(
        const eGameObjectType type,
        const int config,
        UInt64 guid = AUTO_GUID,
        fInitializeGameObject initer = nullptr,
        const char* file = __FILE__,
        const int line = __LINE__
    ) override;

    IGameObject* QueryGameObject(const UInt64 guid) const override;

    void ReleaseGameObject(IGameObject*& go, const char* file, const int line) override;
    UInt64(*_fGuidAlloca)();

    __forceinline UInt64 GenerateGuid() override {
        XASSERT(_fGuidAlloca != nullptr, "GuidAlloca is null");
        return _fGuidAlloca();
    }

    // 通过 IGameWorld 继承
    void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fUInt32AttributeChangeJudge& judge) override;
    void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fInt32AttributeChangeJudge& judge) override;
    void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fUInt64AttributeChangeJudge& judge) override;
    void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fInt64AttributeChangeJudge& judge) override;
    void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fFloatAttributeChangeJudge& judge) override;
    void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fBoolAttributeChangeJudge& judge) override;
    void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fStringAttributeChangeJudge& judge) override;
    void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fVector2AttributeChangeJudge& judge) override;
    void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fVector3AttributeChangeJudge& judge) override;

    void RegistGameObjectAttributeChanged(const eGameObjectType type, const fUInt32AttributeChanged& changed) override;
    void RegistGameObjectAttributeChanged(const eGameObjectType type, const fInt32AttributeChanged& changed) override;
    void RegistGameObjectAttributeChanged(const eGameObjectType type, const fUInt64AttributeChanged& changed) override;
    void RegistGameObjectAttributeChanged(const eGameObjectType type, const fInt64AttributeChanged& changed) override;
    void RegistGameObjectAttributeChanged(const eGameObjectType type, const fFloatAttributeChanged& changed) override;
    void RegistGameObjectAttributeChanged(const eGameObjectType type, const fStringAttributeChanged& changed) override;
    void RegistGameObjectAttributeChanged(const eGameObjectType type, const fBoolAttributeChanged& changed) override;
    void RegistGameObjectAttributeChanged(const eGameObjectType type, const fVector2AttributeChanged& changed) override;
    void RegistGameObjectAttributeChanged(const eGameObjectType type, const fVector3AttributeChanged& changed) override;
};

#endif //__GameWorld_h__
