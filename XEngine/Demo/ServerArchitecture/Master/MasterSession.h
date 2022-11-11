#ifndef __MasterSession_h__
#define __MasterSession_h__

#include "Header.h"

class MasterSession : public iSession<MasterSession>, public Api::iTimer {
public:
    virtual ~MasterSession() {}
    MasterSession() : _area(invalid_id), _id(invalid_id), _name(""), _ip(""), _port(0) {}

    virtual void onConnected(api::iCore* core);
    virtual void onDisconnect(api::iCore* core);
    virtual void onConnectFailed(api::iCore* core);

    static void onReport(const oReport& body, api::iCore* core, MasterSession* session, const int64 context);
    static void onNoderInited(const oNoderInited& body, api::iCore* core, MasterSession* session, const int64 context);

    virtual void onStart(api::iCore* core, const int32 id, const api::iContext& context, const int64 tick) {}
    virtual void onTimer(api::iCore* core, const int32 id, const api::iContext& context, const int64 tick);
    virtual void onEnd(api::iCore* core, const int32 id, const api::iContext& context, bool nonviolent, const int64 tick) {}
    virtual void onPause(api::iCore* core, const int32 id, const api::iContext& context, const int64 tick) {}
    virtual void onResume(api::iCore* core, const int32 id, const api::iContext& context, const int64 tick) {}

    int _area;
    int _id;
    std::string _name;
    std::string _ip;
    int _port;
    uint64 _idfa;
};

#endif //__Master_h__
