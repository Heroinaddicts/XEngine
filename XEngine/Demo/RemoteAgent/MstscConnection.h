#ifndef __MstscConnection_h__
#define __MstscConnection_h__

#include "Header.h"

class MstscConnection : public iTcpSession {
public:
	virtual ~MstscConnection() {}

	// Í¨¹ý iTcpSession ¼Ì³Ð
	virtual int OnReceive(const char* content, const int size) override;
	virtual void OnConnected() override;
	virtual void OnDisconnect() override;
	virtual void OnConnectFailed() override;
};

#endif //__MstscConnection_h__
