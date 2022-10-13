#include "ObjLoader.h"
#include "SafeSystem.h"

iEngine* g_engine = nullptr;

bool ObjLoader::Initialize(iEngine* const engine) {
    g_engine = engine;

    std::string file = SafeSystem::File::GetApplicationPath() + "/Env/Config/Objs";
    std::vector<std::string> paths, names;

    int count = SafeSystem::File::GetFileInDirectory(file, ".obj", paths, names);
    for (int i = 0; i < count; i++) {
        std::string path = paths[i];
        X3DObj* obj = xnew X3DObj();
        if (obj->Load(path)) {
            TRACE(engine, "Load 3D Obj %s success", path.c_str());
            _obj_map.insert(std::make_pair(path, obj));
        }
        else {
            xdel obj;
        }
    }

    return true;
}

bool ObjLoader::Launch(iEngine* const engine) {
    return true;
}

bool ObjLoader::Destroy(iEngine* const engine) {
    return true;
}

const X3DObj* ObjLoader::Get3DObj(const std::string& path) {
    auto itor = _obj_map.find(path);
    if (_obj_map.end() == itor) {
        return nullptr;
    }

    return itor->second;
}
