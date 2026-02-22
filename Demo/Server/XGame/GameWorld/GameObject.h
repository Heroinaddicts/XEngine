#ifndef __GameObject_h__
#define __GameObject_h__

#include "Header.h"

class Attribute;
class IScene;
class GameObject : public IGameObject {
public:
    virtual ~GameObject();

    const char* const _Name;
    const eGameObjectType _Type;
    const UInt64 _Guid;
    const Int64 _Config;
    const eGameObjectType* const _Types;
    const int _TypesCount;

    const char* const _File;
    const int _Line;

    bool _Inited;

    // Í¨¹ý IGameObject ¼Ì³Ð
    UInt64 Guid() const override;
    Int64 ConfigID() const override;
    eGameObjectType Type() const override;
    bool Is(const eGameObjectType type) const override;

    __forceinline const Vector2 Position() const override { return _Postion; }
    __forceinline void SetPosition(const float x, const float z) override { _Postion.x = x; _Postion.y = z; }
    __forceinline float Angle() const override { return _Angle; }
    __forceinline void SetAngle(const float angle) override { _Angle = angle; }

    __forceinline int AttributeCount() const override { return _AttributeCount; }
    const IAttribute* GetAttribute(const int index) const override;

    UInt32 GetUInt32(const int index, const char* file, const int line) const override;
    bool SetUInt32(const int index, const UInt32 value, const bool trigger, const char* file, const int line) override;
    Int32 GetInt32(const int index, const char* file, const int line) const override;
    bool SetInt32(const int index, const Int32 value, const bool trigger, const char* file, const int line) override;
    UInt64 GetUInt64(const int index, const char* file, const int line) const override;
    bool SetUInt64(const int index, const UInt64 value, const bool trigger, const char* file, const int line) override;
    Int64 GetInt64(const int index, const char* file, const int line) const override;
    bool SetInt64(const int index, const Int64 value, const bool trigger, const char* file, const int line) override;
    float GetFloat(const int index, const char* file, const int line) const override;
    bool SetFloat(const int index, const float value, const bool trigger, const char* file, const int line) override;
    bool GetBool(const int index, const char* file, const int line) const override;
    bool SetBool(const int index, const bool value, const bool trigger, const char* file, const int line) override;
    const std::string& GetString(const int index, const char* file, const int line) const override;
    bool SetString(const int index, const std::string& value, const bool trigger, const char* file, const int line) override;
    const Vector2 GetVector2(const int index, const char* file, const int line) const override;
    bool SetVector2(const int index, const Vector2& value, const bool trigger, const char* file, const int line) override;
    const Vector3 GetVector3(const int index, const char* file, const int line) const override;
    bool SetVector3(const int index, const Vector3& value, const bool trigger, const char* file, const int line) override;

#define CREATE_GAME_OBJECT(name, type, guid, config, attrCount, fInitialize, file, line) GameObject::Create(##name, type, guid, config, attrCount, tableCount, fInitialize, file, line);
    static GameObject* Create(
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
    );
    static void Release(GameObject*& go);
protected:
    GameObject(
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
    );

    friend XPool<GameObject>;

private:
    Vector2 _Postion;
    float _Angle;

    Attribute** const _Attributes;
    const int _AttributeCount;
};

#endif //__GameObject_h__
