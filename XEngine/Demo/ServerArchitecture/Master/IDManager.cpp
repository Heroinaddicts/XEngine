#include "idmanager.h"

static std::map<std::string, int> static_ids_type_map;

#define max_id 1000

idmanager & idmanager::getInstance() {
    static idmanager static_manager;
    return static_manager;
}

int idmanager::allocaID(const std::string & name, const std::string & ip, const int32 port) {
    std::string key = name + ip + tools::intAsString(port);
    {
        auto itor = _id_map.find(key);
        if (itor != _id_map.end()) {
            trace(g_core, "noder %s recovery id %u", name.c_str(), itor->second);
            return itor->second;
        }
    }
    
    uint8 type_index = invalid_id;
    {
        auto itor = g_noder_type_index_map.find(name);
        if (g_noder_type_index_map.end() == itor) {
            return invalid_id;
        }
        type_index = itor->second;
    }

    int id = invalid_id;
    {
        auto itor = static_ids_type_map.find(name);
        if (static_ids_type_map.end() == itor) {
            itor = static_ids_type_map.insert(std::make_pair(name, 1)).first;
        }
        if (itor->second >= max_id) {
            trace(g_core, "noder %s saturation", name.c_str());
            return invalid_id;
        }

        id = type_index * max_id + itor->second++;
    }

    _id_map.insert(make_pair(key, id));
    trace(g_core, "noder %s alloca id %u", name.c_str(), id);
    return id;
}
