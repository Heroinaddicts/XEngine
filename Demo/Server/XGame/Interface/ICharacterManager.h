#ifndef __ICharacter_h__
#define __ICharacter_h__

#include "iModule.h"
#include "UnitCommon.h"
#include "Game.pb.h"
UsingXEngine;
UsingUnitCommon;

class IPlayer;
class IScene;
class ICharacterDocker {
public:
    virtual ~ICharacterDocker() {}

    const UInt64 _Account;
    const char* const _File;
    const int _Line;
    ICharacterDocker(const UInt64 account, const char* file, const int line) : _Account(account), _File(file), _Line(line) {}

    virtual IGameObject* CreateCharacters(const Int64 configId) = 0;
    virtual IGameObject* DeployCharacter(const UInt64 characterId) = 0;

    enum class eCharacterEvent {
        DefaultCharacterCreated = 0,
        CharacterCreated = 1,
        CharacterLoaded = 2,
        CharacterRelease = 3,
        CharacterUnDeployed = 4,
        CharacterDeployed = 5
    };

    typedef void(*fCharacterEvent)(ICharacterDocker* const docker, IGameObject* const character);
    virtual void RegistCharacterEvent(const eCharacterEvent ev, const fCharacterEvent& callback, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual IGameObject* GetDeployCharacter() const = 0;
};

class ICharacterManager : public Api::iModule {
public:
    virtual ~ICharacterManager() {}

    enum class eCharacterDockerEvent {
        CharacterDockerCreated = 0,
        CharacterDockerLoaded = 1,
        ChracterDockerRelease = 2,
    };

    virtual ICharacterDocker* CreateCharacterDocker(const UInt64 account, const char* file = __FILE__, const int line = __LINE__) = 0;
    virtual ICharacterDocker* QueryCharacterDocker(const UInt64 account) = 0;
    virtual void ReleaseCharacterDocker(const UInt64 account, const char* file = __FILE__, const int line = __LINE__) = 0;

    typedef void(*fCharacterDockerEvent)(ICharacterDocker*, IGameObject*);
    virtual void RegistCharacterDockerEvent(const eCharacterDockerEvent ev, const fCharacterDockerEvent callback, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual void SendProtobuf(const UInt64 characterId, const eServerID id, const ::google::protobuf::Message& pb) = 0;
    virtual void Broadcast(const UInt64 characterId, const eServerID id, const ::google::protobuf::Message& pb, const bool excludeSelf = false) = 0;
};


#endif //__ICharacter_h__
