#ifndef __RemoteProto_h__
#define __RemoteProto_h__

#pragma pack(push, 1)
struct ProtoHeader {
	unsigned short len;
	unsigned short id;
};
#pragma pack(pop)

enum AgentProto {

};

enum ServerToAgentProto {
	RequestNewConnectioon = 0,
	ServerForwardToAgent,
	UserConnectionLost
};

enum UserProto {

};

enum ServerToUserProto {

};

#define CONNECTION_BUFF_LEN 1024 * 1024 * 10

#endif //__RemoteProto_h__
