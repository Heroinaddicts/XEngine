#ifndef __Scene_h__
#define __Scene_h__

#include "Header.h"

class Scene : public IScene, Api::iTimer {
public:
    virtual ~Scene() {}
    static Scene* CreateScene(const UInt64 guid, const AutoConfig::Struct::Scene* config, const ITilemap* tilemapHandle, const char* file = __FILE__, const int line = __LINE__);
    static void ReleaseScene(Scene* scene);

    const AutoConfig::Struct::Scene* const _Config;

    void RegistAoiEvent(const eAoiEvent ev, const fAoiEvent callback, const char* file, const int line) override;
    void RegistEvent(const eEvent ev, const fEvent callback, const char* file, const int line) override;
    // 通过 IScene 继承
    __forceinline const AutoConfig::Struct::Scene* Config() const override { return _Config; }
    __forceinline UInt64 SceneID() const override { return _SceneID; }
    __forceinline  ITileLayer* GroundTileLayer() const override { return _GroundLayer; }
    __forceinline ITileLayer* FindTileLayer(const std::string& name) const {
        return _Tilemap->GetLayer(name);
    }


    void EnterScene(ITeam* const team) override;
    void EnterScene(const UInt64 guid, const fAoiSingle& appearFun, const bool reset) override;
    void ExitScene(const UInt64 guid, const fAoiSingle& disappearFun) override;
    void GetVisual(const UInt64 guid, const fVisualCallback& callback) const override;
    void GetVisual(const Vector2& pos, const float radius, const fGridVisualCallback& callback) override;
    bool CanMove(const float x, const float y) const override;
    void Broadcast(const eServerID id, const ::google::protobuf::Message& pb) override;


private:
    friend XPool<Scene>;
    Scene(const UInt64 guid, const AutoConfig::Struct::Scene* config, const std::vector<Vector2>& enterPositions, const ITilemap* tilemapHandle, const char* file = __FILE__, const int line = __LINE__);
    void OnTimer(const int id, const UInt64 context, const Int64 timestamp) override;

    void EntityEnterScene(IGameObject* const character, const Vector2& position, const fAoiSingle& appearFun, const bool reset);
private:
    XAoi2D _Aoi;
    UInt64 _SceneID;

    const char* _File;
    const int _Line;

    std::unordered_set<UInt64> _Guids;
    const ITilemap* const _Tilemap;

    ITileLayer* const _GroundLayer;

    XEventPool<eAoiEvent, IScene* const, const UInt64, const std::vector<UInt64>&> _AoiEvents;
    XEventPool<eEvent, IScene* const, const UInt64> _Events;

    std::vector<UInt64> _BroadCastTemp;
    const std::vector<Vector2> _EnterPositions;
    UInt32 _EnterPositionIndex;

    bool _Relasing;
};

#endif //__Scene_h__
