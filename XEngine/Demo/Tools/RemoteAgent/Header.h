#ifndef __Header_h__
#define __Header_h__

#include "iComponent.h"
#include "iNetApi.h"
#include "iTimerApi.h"
#include "SafeString.h"
#include "SafeSystem.h"
#include "XBuffer.h"

using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_engine;

#include "RemoteProto.h"

class MstscConnection;
class ServerConnection;

extern MstscConnection* g_mstscConnection;
extern ServerConnection* g_serverConnection;
extern XBuffer g_buffer;

extern const char* g_server_host;
extern int g_server_port;
extern int g_mstsc_port;
#endif //__Header_h__
