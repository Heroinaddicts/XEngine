#include "TileLayer.h"

bool TileLayer::HasGrid(const float x, const float y) const {
    return _Tiles.contains(this->WorldToGrid(x, y));
}

bool TileLayer::HasGrid(const Vector2Int& grid) const {
    return _Tiles.contains(grid);
}

void TileLayer::FindPath(const Vector2& start, const Vector2& end, const fReturnPath& callback) {
    _AStar.FindPath(start, end,
        [&](const std::vector<Vector2>& path) {
            callback(true, path);
        }
    );
}

#ifndef DEG_TO_RAD
#define DEG_TO_RAD(deg) ((deg) * 3.1415926535f / 180.0f)
#endif

#define RAYCAST_STEP (0.1f)

bool TileLayer::Raycast(const Vector2& origin, const float angle, const float distance, Vector2& hit) {
    UInt64 tick = SafeSystem::Time::GetMilliSecond();

    Vector2 dir(cos(DEG_TO_RAD(angle)), sin(DEG_TO_RAD(angle)));
    if (distance < 0)
    {
        dir = Vector2(-dir.x, -dir.y);
    }
    float step = 0.1f;
    float traveled = 0.0f;

    hit = origin;
    Vector2 pos = origin;
    while (traveled <= std::abs(distance)) {
        Vector2Int cellPos = WorldToGrid(pos.x, pos.y);
        if (!HasGrid(cellPos)) {
            return true;
        }

        hit = pos;
        pos += dir * step;
        traveled += step;
    }

    return false;
}

Vector2 TileLayer::GridToWorld(const Vector2Int& grid) const {
    float wx = (grid.x - grid.y) * (_CellSize.x * 0.5f);  // + Ë®Æ½Æ«ÒÆ
    float wy = (grid.x + grid.y) * (_CellSize.y * 0.5f) + (_CellSize.y * 0.5f);  // + ´¹Ö±Æ«ÒÆ
    return { wx, wy };
}

Vector2Int TileLayer::WorldToGrid(const float x, const float y) const {
    float halfW = _CellSize.x * 0.5f;
    float halfH = _CellSize.y * 0.5f;

    float gx = (x / halfW + y / halfH) * 0.5f;
    float gy = (y / halfH - x / halfW) * 0.5f;

    return { (int)std::floor(gx), (int)std::floor(gy) };
}

TileLayer::TileLayer(ITilemap* tilemap, const std::string& name, const Vector2& cellSize, const std::unordered_set<Vector2Int>& tiles)
    : ITileLayer(tilemap, name, cellSize), _Tiles(tiles),
    _AStar(
        [&](const Vector2Int& grid) {
            return this->HasGrid(grid);
        },
        [&](const Vector2Int& grid) {
            return this->GridToWorld(grid);
        },
        [&](const Vector2& world) {
            return this->WorldToGrid(world.x, world.y);
        },
        [&](const Vector2& start, const Vector2& end) {
            const float distance = (end - start).Length();
            const float angle = (end - start).Angle();
            Vector2 hit;
            return this->Raycast(start, angle, distance, hit);
        },
        tiles.empty() ? Vector2Int(0, 0) : *tiles.begin()
    )
{
}
