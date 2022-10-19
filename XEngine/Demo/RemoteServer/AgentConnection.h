#ifndef __AgentConnection_h__
#define __AgentConnection_h__

#include "Header.h"

class AgentConnection : public iTcpSession {
public:
	virtual ~AgentConnection() {}

	// ͨ�� iTcpSession �̳�
	virtual int OnReceive(const char* content, const int size) override;
	virtual void OnConnected() override;
	virtual void OnDisconnect() override;
	virtual void OnConnectFailed() override;
};


#endif //__AgentConnection_h__
