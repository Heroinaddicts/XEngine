#ifndef __GameEntitySync_h__
#define __GameEntitySync_h__

#include "Header.h"

class GameEntitySync : public IGameEntitySync {
public:
    virtual ~GameEntitySync() {}
private:
    // Í¨¹ý IGameEntitySync ¼Ì³Ð
    bool Initialize(Api::iEngine* const engine) override;
    bool Launch(Api::iEngine* const engine) override;
    bool Destroy(Api::iEngine* const engine) override;

private:
    static void OnUInt32AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const UInt32 oldValue, const UInt32 newValue);
    static void OnInt32AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Int32 oldValue, const Int32 newValue);
    static void OnUInt64AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const UInt64 oldValue, const UInt64 newValue);
    static void OnInt64AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Int64 oldValue, const Int64 newValue);
    static void OnFloatAttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue);
    static void OnBoolAttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const bool oldValue, const bool newValue);
    static void OnStringAttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const std::string& oldValue, const std::string& newValue);
    static void OnVector2AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Vector2& oldValue, const Vector2& newValue);
    static void OnVector3AttributeChanged(IGameObject* const go, const int index, const bool sync, const bool share, const Vector3& oldValue, const Vector3& newValue);

    static void OnEntityStateChanged(const UInt64 guid, const AutoConfig::Enum::eState state, const Vector2& pos, const float angle);
};

#endif //__GameEntitySync_h__
