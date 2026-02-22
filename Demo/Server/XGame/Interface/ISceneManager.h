#ifndef __ISceneManager_h__
#define __ISceneManager_h__

#include "iModule.h"
#include "AutoConfig.h"
#include "Vector2.h"
#include "ITilemapManager.h"
#include "Game.pb.h"
#include <functional>
#include <unordered_set>

UsingXEngine;

class ITeam;
class IGameObject;
class IScene {
public:
    virtual ~IScene() {}

    enum class eAoiEvent {
        Appear,
        Disappear
    };
    typedef void(*fAoiEvent)(IScene* const, const UInt64 guid, const std::vector<UInt64>&);
    virtual void RegistAoiEvent(const eAoiEvent ev, const fAoiEvent callback, const char* file = __FILE__, const int line = __LINE__) = 0;

    enum class eEvent {
        Enter = 0,
        Leave = 1
    };

    typedef void(*fEvent)(IScene* const, const UInt64 guid);
    virtual void RegistEvent(const eEvent ev, const fEvent callback, const char* file = __FILE__, const int line = __LINE__) = 0;

    virtual const AutoConfig::Struct::Scene* Config() const = 0;
    virtual UInt64 SceneID() const = 0;
	virtual ITileLayer* GroundTileLayer() const = 0;
	virtual ITileLayer* FindTileLayer(const std::string & name) const = 0;

    using fAoiSingle = std::function<void(IScene* const, const UInt64 guid, const UInt64 other)>;

    virtual void EnterScene(ITeam* const team) = 0;
    virtual void EnterScene(const UInt64 guid, const fAoiSingle& appearFun = nullptr, const bool reset = true) = 0;
    virtual void ExitScene(const UInt64 guid, const fAoiSingle& disappearFun = nullptr) = 0;

    typedef std::function<void(const std::unordered_set<UInt64>&)> fVisualCallback;
    virtual void GetVisual(const UInt64 guid, const fVisualCallback& callback) const = 0;

    typedef std::function<void(const std::vector<UInt64>&)> fGridVisualCallback;
    virtual void GetVisual(const Vector2& pos, const float radius, const fGridVisualCallback& callback) = 0;

    bool CanMove(const Vector2& pos) {
        return CanMove(pos.x, pos.y);
    }
    virtual bool CanMove(const float x, const float y) const = 0;
    virtual void Broadcast(const eServerID id, const ::google::protobuf::Message& pb) = 0;
};

#define INVALID_SCENE_GUID 0

class ISceneManager : public XEngine::Api::iModule {
public:
    virtual ~ISceneManager() {}

    enum class eSceneEvent {
        Create = 1,
        Release = 2
    };
    typedef void(*fSceneEvent)(IScene* const);
    virtual void RegistSceneEvent(const eSceneEvent ev, const fSceneEvent& callback) = 0;

    typedef std::function<void(IScene* const scene)> fCreateCopySceneCallback;
    virtual void CreateCopyScene(const Int64 configId, const fCreateCopySceneCallback& callback) = 0;
    virtual void ReleaseCopyScene(IScene* const scene) = 0;

    virtual IScene* QueryCopyScene(IGameObject* const character) const = 0;
    virtual IScene* QueryPublicScene(const Int64 configId) const = 0;
    virtual IScene* QuerySceneByEntityGuid(const UInt64 guid) const = 0;
    virtual IScene* QuerySceneBySceneID(const UInt64 sceneGuid) const = 0;
};

#endif //__ISceneManager_h__
