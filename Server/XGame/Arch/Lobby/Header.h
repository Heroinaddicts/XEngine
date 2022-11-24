#ifndef __Header_h__
#define __Header_h__

#include "iLobby.h"
#include "iConnecter.h"

#include <unordered_map>

using namespace XEngine;
using namespace XEngine::Api;

extern iEngine* g_Engine;
class Lobby;
extern Lobby* g_Lobby;
extern iConnecter* g_Connecter;

/*
 *  std::unordered_map<���ID, std::map<�������ڵ���, ��Ӧ���ڷ������ڵ�ID>>
 *  ����<100023114, std::map<"Gate", 10001>>
*/
extern std::unordered_map<unsigned_int64, std::map<std::string, int>> g_PlayerLocation;

#endif //__Header_h__
