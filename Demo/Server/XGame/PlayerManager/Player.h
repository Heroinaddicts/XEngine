#ifndef __Player_h__
#define __Player_h__

#include "Header.h"

class Player : public IPlayer {
public:
    virtual ~Player() {}

    const UInt64 _Account;

    static Player* CreatePlayer(const UInt64 account);
    static Player* QueryPlayer(const UInt64 account);
    static void ReleasePlayer(Player* player);

    // Í¨¹ý IPlayer ¼Ì³Ð

    __forceinline ePlayerStatus Status() const {
        return _Status;
    }

    __forceinline void SetStatus(const ePlayerStatus status) {
        _Status = status;
    }

    __forceinline void SetDeployCharacterID(const UInt64 characterId) {
        _DeployCharacterID = characterId;
        Save();
    }
    __forceinline UInt64 GetDeployCharacterID() const override { return _DeployCharacterID; }

    __forceinline UInt64 AccountID() const override { return _Account; }

    __forceinline const std::string Nickname() const override { return _Nickname; }
    __forceinline UInt32 Gold() const override { return _Gold; }
    __forceinline UInt32 Diamond() const override { return _Diamond; }

    __forceinline const Vector3& Position() const override { return _Position; }
    __forceinline void SetPosition(const float x, const float y, const float z) override { _Position.x = x; _Position.y = y; _Position.z = z; }

    __forceinline void SetNickname(const std::string& name) { _Nickname = name; }
    __forceinline void SetGold(const UInt32 gold) override { _Gold = gold; }
    __forceinline void SetDiamond(const UInt32 diamond) override { _Diamond = diamond; }

    void Save();

    void SendMessage(const UInt16 messageId, const void* data, const int size) const override;
private:
    friend XPool<Player>;
    Player(const UInt64 account);

private:
    ePlayerStatus _Status;

    std::string _Nickname;
    UInt32 _Gold;
    UInt32 _Diamond;
    Vector3 _Position;

    UInt64 _DeployCharacterID;
};

#endif //__Player_h__
