#ifndef __TileLayer_h__
#define __TileLayer_h__

#include "Header.h"

class TileLayer : public ITileLayer {
public:
    virtual ~TileLayer() {}

    TileLayer(ITilemap* tilemap, const std::string& name, const Vector2& cellSize, const std::unordered_set<Vector2Int>& tiles);
    bool HasGrid(const float x, const float y) const override;
    bool HasGrid(const Vector2Int& grid) const override;
    void FindPath(const Vector2& start, const Vector2& end, const fReturnPath& callback) override;
    bool Raycast(const Vector2& origin, const float angle, const float distance, Vector2& hit) override;

    Vector2 GridToWorld(const Vector2Int& grid) const override;
    Vector2Int WorldToGrid(const float x, const float y) const override;
private:
    std::unordered_set<Vector2Int> _Tiles;
    XAStar _AStar;
};

#endif __TileLayer_h__