#ifndef __IGameWorld_h__
#define __IGameWorld_h__

#include "iModule.h"
#include <functional>

#include "Game.pb.h"

#include "Geometry/Vector2.h"
#include "Geometry/Vector3.h"
#include "UnitCommon.h"
UsingUnitCommon;
UsingXEngine;
using namespace UnitCommon;

class IScene;
class IAttribute {
public:
    virtual ~IAttribute() {}

    const eAttributeType _Type;
    const std::string _Name;
    const int _Index;

    IAttribute(const eAttributeType type, const std::string& name, const int index) : _Type(type), _Name(name), _Index(index) {}

    virtual UInt32 GetUInt32(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetUInt32(const UInt32 value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual Int32 GetInt32(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetInt32(const Int32 value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual UInt64 GetUInt64(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetUInt64(const UInt64 value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual Int64 GetInt64(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetInt64(const Int64 value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual float GetFloat(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetFloat(const float value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual bool GetBool(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetBool(const bool value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual const std::string& GetString(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetString(const std::string& value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual Vector2 GetVector2(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetVector2(const Vector2& value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual Vector3 GetVector3(const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetVector3(const Vector3& value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
};

class IGameObject {
public:
    virtual ~IGameObject() {}

    virtual UInt64 Guid() const = 0;
    virtual Int64 ConfigID() const = 0;
    virtual eGameObjectType Type() const = 0;
    virtual bool Is(const eGameObjectType type) const = 0;

    virtual const Vector2 Position() const = 0;
    virtual void SetPosition(const float x, const float z) = 0;
    __forceinline void SetPosition(const Vector2 pos) {
        SetPosition(pos.x, pos.y);
    }

    virtual float Angle() const = 0;
    virtual void SetAngle(const float angle) = 0;

    virtual int AttributeCount() const = 0;
    virtual const IAttribute* GetAttribute(const int index) const = 0;

#define GetGameObjectAttr(GO, Type, index) \
    GO->Get##Type(index, __FILE__, __LINE__)

#define SetGameObjectAttr(GO, Type, index, value) \
    GO->Set##Type(index, value, __FILE__, __LINE__)

    virtual UInt32 GetUInt32(const int index, const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetUInt32(const int index, const UInt32 value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual Int32 GetInt32(const int index, const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetInt32(const int index, const Int32 value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual UInt64 GetUInt64(const int index, const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetUInt64(const int index, const UInt64 value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual Int64 GetInt64(const int index, const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetInt64(const int index, const Int64 value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual float GetFloat(const int index, const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetFloat(const int index, const float value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual bool GetBool(const int index, const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetBool(const int index, const bool value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual const std::string& GetString(const int index, const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetString(const int index, const std::string& value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual const Vector2 GetVector2(const int index, const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetVector2(const int index, const Vector2& value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual const Vector3 GetVector3(const int index, const char* file = __FILE__, const int line = __LINE__) const = 0;
    virtual bool SetVector3(const int index, const Vector3& value, const bool trigger = true, const char* file = __FILE__, const int line = __LINE__) = 0;

};

#define ATTRIBUTE_ANY -1
#define AUTO_GUID 0xFFFFFFFFFFFFFFFF
class IGameWorld : public XEngine::Api::iModule {
public:
    virtual ~IGameWorld() {}

    enum class eGameWorldEvent {
        Create,
        Release
    };

    virtual void SetGuidAlloca(UInt64(*fun)()) = 0;

    virtual UInt64 GenerateGuid() = 0;

    typedef void(*fGameObjectEvent)(IGameObject* const);
    virtual void RegistGameObjectEvent(const eGameObjectType type, const eGameWorldEvent ev, fGameObjectEvent callback) = 0;


    typedef bool(*fUInt32AttributeChangeJudge)(IGameObject* const go, const int index, const bool sync, const bool share, const UInt32 oldValue, const UInt32 newValue);
    typedef bool(*fInt32AttributeChangeJudge)(IGameObject* const go, const int index, const bool sync, const bool share, const Int32 oldValue, const Int32 newValue);
    typedef bool(*fUInt64AttributeChangeJudge)(IGameObject* const go, const int index, const bool sync, const bool share, const UInt64 oldValue, const UInt64 newValue);
    typedef bool(*fInt64AttributeChangeJudge)(IGameObject* const go, const int index, const bool sync, const bool share, const Int64 oldValue, const Int64 newValue);
    typedef bool(*fFloatAttributeChangeJudge)(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue);
    typedef bool(*fBoolAttributeChangeJudge)(IGameObject* const go, const int index, const bool sync, const bool share, const bool oldValue, const bool newValue);
    typedef bool(*fStringAttributeChangeJudge)(IGameObject* const go, const int index, const bool sync, const bool share, const std::string& oldValue, const std::string& newValue);
    typedef bool(*fVector2AttributeChangeJudge)(IGameObject* const go, const int index, const bool sync, const bool share, const Vector2& oldValue, const Vector2& newValue);
    typedef bool(*fVector3AttributeChangeJudge)(IGameObject* const go, const int index, const bool sync, const bool share, const Vector3& oldValue, const Vector3& newValue);

    virtual void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fUInt32AttributeChangeJudge& judge) = 0;
    virtual void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fInt32AttributeChangeJudge& judge) = 0;
    virtual void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fUInt64AttributeChangeJudge& judge) = 0;
    virtual void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fInt64AttributeChangeJudge& judge) = 0;
    virtual void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fFloatAttributeChangeJudge& judge) = 0;
    virtual void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fBoolAttributeChangeJudge& judge) = 0;
    virtual void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fStringAttributeChangeJudge& judge) = 0;
    virtual void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fVector2AttributeChangeJudge& judge) = 0;
    virtual void RegistGameObjectAttributeChangeJudge(const eGameObjectType type, const fVector3AttributeChangeJudge& judge) = 0;

    typedef void(*fUInt32AttributeChanged)(IGameObject* const go, const int index, const bool sync, const bool share, const UInt32 oldValue, const UInt32 newValue);
    typedef void(*fInt32AttributeChanged)(IGameObject* const go, const int index, const bool sync, const bool share, const Int32 oldValue, const Int32 newValue);
    typedef void(*fUInt64AttributeChanged)(IGameObject* const go, const int index, const bool sync, const bool share, const UInt64 oldValue, const UInt64 newValue);
    typedef void(*fInt64AttributeChanged)(IGameObject* const go, const int index, const bool sync, const bool share, const Int64 oldValue, const Int64 newValue);
    typedef void(*fFloatAttributeChanged)(IGameObject* const go, const int index, const bool sync, const bool share, const float oldValue, const float newValue);
    typedef void(*fBoolAttributeChanged)(IGameObject* const go, const int index, const bool sync, const bool share, const bool oldValue, const bool newValue);
    typedef void(*fStringAttributeChanged)(IGameObject* const go, const int index, const bool sync, const bool share, const std::string& oldValue, const std::string& newValue);
    typedef void(*fVector2AttributeChanged)(IGameObject* const go, const int index, const bool sync, const bool share, const Vector2& oldValue, const Vector2& newValue);
    typedef void(*fVector3AttributeChanged)(IGameObject* const go, const int index, const bool sync, const bool share, const Vector3& oldValue, const Vector3& newValue);

    virtual void RegistGameObjectAttributeChanged(const eGameObjectType type, const fUInt32AttributeChanged& changed) = 0;
    virtual void RegistGameObjectAttributeChanged(const eGameObjectType type, const fInt32AttributeChanged& changed) = 0;
    virtual void RegistGameObjectAttributeChanged(const eGameObjectType type, const fUInt64AttributeChanged& changed) = 0;
    virtual void RegistGameObjectAttributeChanged(const eGameObjectType type, const fInt64AttributeChanged& changed) = 0;
    virtual void RegistGameObjectAttributeChanged(const eGameObjectType type, const fFloatAttributeChanged& changed) = 0;
    virtual void RegistGameObjectAttributeChanged(const eGameObjectType type, const fStringAttributeChanged& changed) = 0;
    virtual void RegistGameObjectAttributeChanged(const eGameObjectType type, const fBoolAttributeChanged& changed) = 0;
    virtual void RegistGameObjectAttributeChanged(const eGameObjectType type, const fVector2AttributeChanged& changed) = 0;
    virtual void RegistGameObjectAttributeChanged(const eGameObjectType type, const fVector3AttributeChanged& changed) = 0;

    typedef std::function<void(IGameObject* const)> fInitializeGameObject;
    virtual IGameObject* CreateGameObject(
        const eGameObjectType type,
        const int config,
        UInt64 guid,
        fInitializeGameObject initer,
        const char* file = __FILE__,
        const int line = __LINE__
    ) = 0;

    virtual IGameObject* QueryGameObject(const UInt64 guid) const = 0;

    void ReleaseGameObjectByGuid(const UInt64 guid, const char* file = __FILE__, const int line = __LINE__) {
        IGameObject* go = QueryGameObject(guid);
        if (go) {
            ReleaseGameObject(go, file, line);
        }
    }

    virtual void ReleaseGameObject(IGameObject*& go, const char* file = __FILE__, const int line = __LINE__) = 0;
};

#define PackageGameObjectToPB(pb, go, state) { \
    pb->set_guid(go->Guid()); \
    pb->set_type((int)go->Type()); \
    pb->set_configid(go->ConfigID()); \
    pb->set_angle(go->Angle()); \
    if(state) \
        pb->set_state((int)g_Fsm->GetState(go->Guid())); \
    oVector2* pos = pb->mutable_position(); \
    pos->set_x(go->Position().x); \
    pos->set_y(go->Position().y); \
    for (int i = 0; i < go->AttributeCount(); i++) { \
        oAttribute* attr = pb->add_attributes(); \
        attr->set_index(i); \
        oValue* v = attr->mutable_value(); \
        v->set__type((int)go->GetAttribute(i)->_Type); \
        switch (go->GetAttribute(i)->_Type) { \
        case eAttributeType::__Int32__: { \
            v->set__int(go->GetAttribute(i)->GetInt32()); \
            break; \
        } \
        case eAttributeType::__UInt32__: { \
            v->set__uint(go->GetAttribute(i)->GetUInt32()); \
            break; \
        } \
        case eAttributeType::__Int64__: { \
            v->set__int(go->GetAttribute(i)->GetInt64()); \
            break; \
        } \
        case eAttributeType::__UInt64__: { \
            v->set__uint(go->GetAttribute(i)->GetUInt64()); \
            break; \
        } \
        case eAttributeType::__Float__: { \
            v->set__float(go->GetAttribute(i)->GetFloat()); \
            break; \
        } \
        case eAttributeType::__Bool__: { \
            v->set__bool(go->GetAttribute(i)->GetBool()); \
            break; \
        } \
        case eAttributeType::__String__: { \
            v->set__string(go->GetAttribute(i)->GetString()); \
            break; \
        } \
        case eAttributeType::__Vector2__: { \
            oVector2* v2 = v->mutable__vector2(); \
            v2->set_x(go->GetAttribute(i)->GetVector2().x); \
            v2->set_y(go->GetAttribute(i)->GetVector2().y); \
            break; \
        } \
        case eAttributeType::__Vector3__: { \
            oVector3* v3 = v->mutable__vector3(); \
            v3->set_x(go->GetAttribute(i)->GetVector3().x); \
            v3->set_y(go->GetAttribute(i)->GetVector3().y); \
            v3->set_z(go->GetAttribute(i)->GetVector3().z); \
            break; \
        } \
        default: \
            XASSERT(false, "wtf"); \
            break; \
        } \
    } \
}

#endif //__IGameWorld_h__