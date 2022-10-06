#include "MeshLoader.h"
#include <fstream>
#include <sstream>

namespace XEngine {
    FbxManager* g_sdk_manager = nullptr;
    FbxScene* g_scene = nullptr;

    iMeshLoader* MeshLoader::GetInstance() {
        static MeshLoader static_fbxer;
        return &static_fbxer;
    }

    bool MeshLoader::Initialize(Api::iEngine* const engine) {
        g_sdk_manager = FbxManager::Create();
        if (nullptr == g_sdk_manager) {
            XERROR(engine, "create fbx manager error");
            return false;
        }

        FbxIOSettings* ios = FbxIOSettings::Create(g_sdk_manager, IOSROOT);
        g_sdk_manager->SetIOSettings(ios);
        // Load plug-ins from the executable directory
        FbxString lExtension = "dll";
        FbxString lPath = FbxGetApplicationDirectory();
        g_sdk_manager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());

        FbxString path("D:/Github/XEngine/Bin/Windows/Debug/SA_Bld_OfficeWhite_03.fbx");
        // Create the entity that hold the whole Scene
        g_scene = FbxScene::Create(g_sdk_manager, path);

        int count = g_scene->GetGenericNodeCount();
        FbxNode* root = g_scene->GetRootNode();
        FbxString temp = g_scene->GetGlobalSettings().GetSystemUnit().GetScaleFactorAsString();
        return true;
    }

    bool MeshLoader::Launch(Api::iEngine* const engine) {
        return true;
    }

    void MeshLoader::Release(Api::iEngine* const engine) {

    }

    void MeshLoader::EarlyUpdate(Api::iEngine* const engine) {

    }

    void MeshLoader::Update(Api::iEngine* const engine) {

    }

    void MeshLoader::FixedUpdate(Api::iEngine* const engine) {

    }

    void MeshLoader::LaterUpdate(Api::iEngine* const engine) {

    }

    bool MeshLoader::LoadObjSync(const std::string& path, OUT Api::MeshData& data) {
        std::ifstream file(path);
        std::string line;
        int count = 0;
        while (getline(file, line)) {
            if (line.substr(0, 2) == "v ") {
                Vector3 v;
                count++;
                std::istringstream s(line.substr(2));
                s >> v.x >> v.y >> v.z;
                data.v.push_back(v);
            }
            else if (line.substr(0, 1) == "f") {
                count++;
                std::vector<Api::Face> fs;
                std::istringstream s(line.substr(2));
                std::string tmpdata[3];
                s >> tmpdata[0] >> tmpdata[1] >> tmpdata[2];
                for (int i = 0; i < 3; i++) {
                    Api::Face f;
                    std::istringstream sdata(tmpdata[i]);
                    std::string buffer;
                    int j = 0;
                    while (getline(sdata, buffer, '/'))
                    {
                        // 得到的u 就是25
                        f.u = atoi(buffer.c_str()) - 1;
                        break;
                    }
                    fs.push_back(f);
                }
                data.f.push_back(fs);
            }

        }
        file.close();
        return true;
    }
}
