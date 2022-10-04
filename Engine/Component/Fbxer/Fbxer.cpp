#include "Fbxer.h"

namespace XEngine {
    FbxManager* g_sdk_manager = nullptr;
    FbxScene* g_scene = nullptr;


    iFbxer* Fbxer::GetInstance() {
        static Fbxer static_fbxer;
        return &static_fbxer;
    }

    bool Fbxer::Initialize(Api::iEngine* const engine) {
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

        // Create the entity that hold the whole Scene
        g_scene = FbxScene::Create(g_sdk_manager, "");
        return true;
    }

    bool Fbxer::Launch(Api::iEngine* const engine) {
        return true;
    }

    void Fbxer::Release(Api::iEngine* const engine) {
    }

    void Fbxer::EarlyUpdate(Api::iEngine* const engine) {
    }

    void Fbxer::Update(Api::iEngine* const engine) {
    }

    void Fbxer::FixedUpdate(Api::iEngine* const engine) {
    }

    void Fbxer::LaterUpdate(Api::iEngine* const engine) {
    }
}
