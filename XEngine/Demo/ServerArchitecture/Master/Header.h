#ifndef __header_h__
#define __header_h__

#include "iMaster.h"
#include "iSession.h"
#include "architecture.pb.h"
#include "tinyxml/tinyxml2.h"

#include "tools.h"

#include <map>
#include <set>

using namespace architecture;

class masterSession;
typedef std::map<int, masterSession *> id_session_map;
typedef std::map<std::string, std::set<masterSession *>> name_sessions_map;

extern id_session_map g_id_session_map;
extern name_sessions_map g_name_sessions_map;

extern std::string g_master_ip;
extern int32 g_master_port;

extern int32 g_send_pipe_size;
extern int32 g_recv_pipe_size;

namespace timer {
    namespace id {
        enum {
            broadcast_noder_info = 1,
        };
    }

    namespace config {
        const static int32 broadcast_noder_info_interval = 3000;
    }
}

#define invalid_id -1

typedef std::map<std::string, uint8> noder_type_index_map;
extern noder_type_index_map g_noder_type_index_map;

extern tcore::api::iCore * g_core;

#endif //__header_h__
