#ifndef __ServerConnection_h__
#define __ServerConnection_h__

#include "Header.h"

class ServerConnection : public iTcpSession, public iTimer {
public:
	virtual ~ServerConnection() {}

	// 通过 iTcpSession 继承
	virtual int OnReceive(const char* content, const int size) override;
	virtual void OnConnected() override;
	virtual void OnDisconnect() override;
	virtual void OnConnectFailed() override;

	// 通过 iTimer 继承
	virtual void OnStart(const int id, void* const context, const int64 timestamp) override;
	virtual void OnTimer(const int id, void* const context, const int64 timestamp) override;
	virtual void OnEnd(const int id, void* const context, bool nonviolent, const int64 timestamp) override;
	virtual void OnPause(const int id, void* const context, const int64 timestamp) override;
	virtual void OnResume(const int id, void* const context, const int64 timestamp) override;
};

#endif //__ServerConnection_h__
