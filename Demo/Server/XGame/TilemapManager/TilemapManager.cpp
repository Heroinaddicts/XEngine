#include "TilemapManager.h"
#include "IsometricTilemap.h"
#include <fstream>
#include <sstream>
#include <JsonCpp/json.h>
#include "TilemapManager.h"

Api::iEngine* g_Engine = nullptr;

bool TilemapManager::Initialize(Api::iEngine* const engine) {
    g_Engine = engine;
    return true;
}

bool TilemapManager::Launch(Api::iEngine* const engine) {
    return true;
}

bool TilemapManager::Destroy(Api::iEngine* const engine) {
    return true;
}

void TilemapManager::LoadIsometricTilemap(const std::string& path, const fIIsometricTilemapReturn& ret, const bool sync) {
    if (sync) {
        ret(IsometricTilemap::CreateWithXml(path));
    }
    else {
        g_Engine->GetTaskApi()->PushTask(Api::eTaskProcessorType::SeparateThread,
            [=](Api::iTaskContext& context) {
                context.Set(IsometricTilemap::CreateWithXml(path));
                return 0;
            },
            [=](const Int64 code, const Api::iTaskContext& context) {
                ret(context.Get<IsometricTilemap>());
            }
        );
    }
}
