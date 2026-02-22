#ifndef __ITilemapManager_h__
#define __ITilemapManager_h__

#include <functional>
#include "iModule.h"
#include "Vector2.h"
#include "Vector3.h"
UsingXEngine;

enum class eTileType {
    Isometric = 3
};

class ITileLayer;
class ITilemap {
public:
    virtual ~ITilemap() {}

    ITilemap(const eTileType type) : _Type(type) {}
    const eTileType _Type;

    virtual ITileLayer* GetLayer(const std::string& name) const = 0;
};

class ITileLayer {
public:
    virtual ~ITileLayer() {}
    ITileLayer(ITilemap* tilemap, const std::string& name, const Vector2& cellSize) : _Tilemap(tilemap), _LayerName(name), _CellSize(cellSize) {}

    ITilemap* const _Tilemap;
    const std::string _LayerName;
    const Vector2 _CellSize;

    __forceinline bool HasGrid(const Vector2& pos) const {
        return HasGrid(pos.x, pos.y);
    }

    virtual bool HasGrid(const float x, const float y) const = 0;
    virtual bool HasGrid(const Vector2Int& grid) const = 0;

    typedef std::function<void(const bool succeed, const std::vector<Vector2>& path)> fReturnPath;
    virtual void FindPath(const Vector2& start, const Vector2& end, const fReturnPath& callback) = 0;

    virtual Vector2 GridToWorld(const Vector2Int& grid) const = 0;
    virtual Vector2Int WorldToGrid(const float x, const float y) const = 0;

    virtual bool Raycast(const Vector2& origin, const float angle, const float distance, Vector2& hit) = 0;
};

class IIsometricTilemap : public ITilemap {
public:
    virtual ~IIsometricTilemap() {}

    const std::string _Name;
    IIsometricTilemap(const eTileType type, const std::string& name) : ITilemap(type), _Name(name) {}

    virtual const std::unordered_map<std::string, ITileLayer* const>& Layers() const = 0;
};

class ITilemapManager : public Api::iModule {
public:
    virtual ~ITilemapManager() {}

    typedef std::function<void(IIsometricTilemap* handle)> fIIsometricTilemapReturn;
    virtual void LoadIsometricTilemap(const std::string& path, const fIIsometricTilemapReturn& ret, const bool sync = false) = 0;
};

#endif //__ITilemapManager_h__
