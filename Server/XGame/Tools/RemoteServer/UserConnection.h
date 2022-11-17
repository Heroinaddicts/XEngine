#ifndef __UserConnection_h__
#define __UserConnection_h__

#include "Header.h"

class UserConnection : public iTcpSession {
public:
	virtual ~UserConnection() {}

	// Í¨¹ý iTcpSession ¼Ì³Ð
	virtual int OnReceive(const char* content, const int size) override;
	virtual void OnConnected() override;
	virtual void OnDisconnect() override;
	virtual void OnConnectFailed() override;
};

#endif //__UserConnection_h__
