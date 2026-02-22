#ifndef __Client_h__
#define __Client_h__

#include "Header.h"

class Client : public IClient, public Api::iTimer {
public:
    virtual ~Client();

    static Client* Create();
    static Client* Query(const UInt64 clientId);
    static Client* QueryByAccount(const UInt64 account);
    static void Release(Client* client);
    static void ReleaseAll();

    // Í¨¹ý IClient ¼Ì³Ð
    __forceinline UInt64 ClientID() const override { return _ClientID; }
    __forceinline UInt64 Account() const override { return _Account; }
    __forceinline bool IsLogined() const override { return _IsLogined; }

private:
    Client();
    friend XPool<Client>;

    void OnConnected() override;
    void OnDisconnected() override;
    int OnReceive(const void* const content, const int size) override;

    static void RelateAccountClient(const UInt64 account, Client* const client);
    static void KickOldClient(const UInt64 account);

    int __OnReceive__(const UInt64 clientId, const void* const content, const int size);


    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

private:
    const UInt64 _ClientID;
    UInt64 _Account;
    bool _IsLogined;
    eAccountType _Type;

    UInt64 _LastRecvTick;
};

#endif //__Client_h__
