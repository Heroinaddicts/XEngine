#include "GameObject.h"
#include "Attribute.h"

static XPool<GameObject> s_Pool;

GameObject* GameObject::Create(
    const char* const name,
    const eGameObjectType type,
    const UInt64 guid,
    const int config,
    const int attrCount,
    void(*fInitialize)(GameObject* const go, Attribute** const attributes),
    const eGameObjectType* const types,
    const int typesCount,
    const char* file,
    const int line
) {
    return XPOOL_CREATE(s_Pool, name, type, guid, config, attrCount, fInitialize, types, typesCount, file, line);
}

void GameObject::Release(GameObject*& go) {
    XPOOL_RELEASE(s_Pool, go);
    go = nullptr;
}

GameObject::GameObject(
    const char* const name,
    const eGameObjectType type,
    const UInt64 guid,
    const int config,
    const int attrCount,
    void(*fInitialize)(GameObject* const go, Attribute** const attributes),
    const eGameObjectType* const types,
    const int typesCount,
    const char* file,
    const int line
) : _Name(name),
_Type(type),
_Guid(guid),
_Config(config),
_AttributeCount(attrCount),
_Attributes(txnew Attribute* [attrCount]),
_Postion(0, 0),
_Angle(0),
_Types(types),
_TypesCount(typesCount),
_Inited(false),
_File(file),
_Line(line) {
    fInitialize(this, _Attributes);
}

GameObject::~GameObject() {
    for (int i = 0; i < _AttributeCount; i++) {
        Attribute::Release(_Attributes[i]);
    }
    txdel[] _Attributes;
}

UInt64 GameObject::Guid() const {
    return _Guid;
}

Int64 GameObject::ConfigID() const {
    return _Config;
}

eGameObjectType GameObject::Type() const {
    return _Type;
}

bool GameObject::Is(const eGameObjectType type) const {
    return UnitCommon::IsType(_Type, type);
}

#define GET_GO_ATTRIBUTE(TYPE) \
    XASSERT(index < _AttributeCount && index >= 0, "Attribute Index %d overflow %s:%d", index, file, line); \
    if (index >= _AttributeCount || index < 0) { \
        ErrorLog(g_Engine, "GameObject %s Attribute Index %d overflow %s:%d", _Name, index, file, line); \
        return 0; \
    } \
    return _Attributes[index]->Get##TYPE(file, line);

#define SET_GO_ATTRIBUTE(TYPE)  \
    XASSERT(index < _AttributeCount && index >= 0, "Attribute Index %d overflow %s:%d", index, file, line); \
    if (index >= _AttributeCount || index < 0) { \
        ErrorLog(g_Engine, "GameObject %s Attribute Index %d overflow %s:%d", _Name, index, file, line); \
        return false; \
    } \
    return _Attributes[index]->Set##TYPE(value, trigger, file, line);


const IAttribute* GameObject::GetAttribute(const int index) const {
    XASSERT(index >= 0 && index < _AttributeCount, "wtf");
    if (index >= 0 && index < _AttributeCount) {
        return _Attributes[index];
    }

    return nullptr;
}

UInt32 GameObject::GetUInt32(const int index, const char* file, const int line) const {
    GET_GO_ATTRIBUTE(UInt32);
}

bool GameObject::SetUInt32(const int index, const UInt32 value, const bool trigger, const char* file, const int line) {
    SET_GO_ATTRIBUTE(UInt32);
}

Int32 GameObject::GetInt32(const int index, const char* file, const int line) const {
    GET_GO_ATTRIBUTE(Int32);
}

bool GameObject::SetInt32(const int index, const Int32 value, const bool trigger, const char* file, const int line) {
    SET_GO_ATTRIBUTE(Int32);
}

UInt64 GameObject::GetUInt64(const int index, const char* file, const int line) const {
    GET_GO_ATTRIBUTE(UInt64);
}

bool GameObject::SetUInt64(const int index, const UInt64 value, const bool trigger, const char* file, const int line) {
    SET_GO_ATTRIBUTE(UInt64);
}

Int64 GameObject::GetInt64(const int index, const char* file, const int line) const {
    GET_GO_ATTRIBUTE(Int64);
}

bool GameObject::SetInt64(const int index, const Int64 value, const bool trigger, const char* file, const int line) {
    SET_GO_ATTRIBUTE(Int64);
}

float GameObject::GetFloat(const int index, const char* file, const int line) const {
    GET_GO_ATTRIBUTE(Float);
}

bool GameObject::SetFloat(const int index, const float value, const bool trigger, const char* file, const int line) {
    SET_GO_ATTRIBUTE(Float);
}

bool GameObject::GetBool(const int index, const char* file, const int line) const {
    GET_GO_ATTRIBUTE(Bool);
}

bool GameObject::SetBool(const int index, const bool value, const bool trigger, const char* file, const int line) {
    SET_GO_ATTRIBUTE(Bool);
}

const std::string& GameObject::GetString(const int index, const char* file, const int line) const {
    GET_GO_ATTRIBUTE(String);
}

bool GameObject::SetString(const int index, const std::string& value, const bool trigger, const char* file, const int line) {
    SET_GO_ATTRIBUTE(String);
}

const Vector2 GameObject::GetVector2(const int index, const char* file, const int line) const {
    GET_GO_ATTRIBUTE(Vector2);
}

bool GameObject::SetVector2(const int index, const Vector2& value, const bool trigger, const char* file, const int line) {
    XASSERT(index < _AttributeCount && index >= 0, "Attribute Index %d overflow %s:%d", index, file, line);
    if (index >= _AttributeCount || index < 0) {
        ErrorLog(g_Engine, "GameObject %s Attribute Index %d overflow %s:%d", _Name, index, file, line);
        return false;
    }
    return _Attributes[index]->SetVector2(value, trigger, file, line);
}

const Vector3 GameObject::GetVector3(const int index, const char* file, const int line) const {
    GET_GO_ATTRIBUTE(Vector3);
}

bool GameObject::SetVector3(const int index, const Vector3& value, const bool trigger, const char* file, const int line) {
    SET_GO_ATTRIBUTE(Vector3);
}
