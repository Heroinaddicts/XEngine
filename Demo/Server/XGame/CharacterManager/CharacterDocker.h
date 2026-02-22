#ifndef __CharacterDocker_h__
#define __CharacterDocker_h__

#include "Header.h"

class CharacterDocker : public ICharacterDocker {
public:
    virtual ~CharacterDocker() {}

    static CharacterDocker* CreateCharacterDocker(const UInt64 account, const char* file = __FILE__, const int line = __LINE__);
    static CharacterDocker* QueryCharacterDocker(const UInt64 account);
    static void ReleaseCharacterDocker(const UInt64 account, const char* file, const int line);

    using fCharacterDockerLoaded = std::function<void(const bool)>;
    void Load(const fCharacterDockerLoaded& callback);

    void RegistCharacterEvent(const eCharacterEvent ev, const fCharacterEvent& callback, const char* file = __FILE__, const int line = __LINE__) override;
    // Í¨¹ý ICharacterDocker ¼Ì³Ð
    __forceinline IGameObject* CreateCharacters(const Int64 configId) override {
        return __CreateCharacters(configId);
    }

    IGameObject* __CreateCharacters(const Int64 configId);
    IGameObject* DeployCharacter(const UInt64 characterId) override;
    IGameObject* GetDeployCharacter() const override;

    __forceinline const std::map<UInt64, IGameObject*>& GetCharacters() const { return _Characters; }

    void TriggerLoaded();

    void SendCharacterInfosToClient();

    void SaveDocker();

    void SaveCharacterData(IGameObject* const character);

    __forceinline void TriggerEvent(const eCharacterEvent ev, IGameObject* const character) {
        _Events.Trigger(ev, this, character);
    }

private:
    friend XPool<CharacterDocker>;
    CharacterDocker(const UInt64 account, const char* file, const int line) : ICharacterDocker(account, file, line), _DeployCharacterID(INVALID_GAMEOBJECT_ID) {}

    static void __LoadCharacterDockerData__(CharacterDocker* docker, const UInt64 account, const fCharacterDockerLoaded& callback);

private:
    std::map<UInt64, IGameObject*> _Characters;
    XEventPool<eCharacterEvent, ICharacterDocker* const, IGameObject* const> _Events;
    UInt64 _DeployCharacterID;
};

#endif //__CharacterDocker_h__
