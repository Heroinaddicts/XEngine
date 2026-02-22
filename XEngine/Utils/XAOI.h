#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
#include <cmath>
#include <cstdint>
#include "Vector2.h"

namespace XEngine {

    __forceinline static UInt64 XAoi2DCalcGridIndex(const int x, const int y) {
        return static_cast<UInt64>(static_cast<UInt32>(x)) << 32 | static_cast<UInt64>(static_cast<UInt32>(y));
    }

    __forceinline static UInt64 XAoi2DCalcGridIndex(const float x, const float y) {
        return XAoi2DCalcGridIndex(static_cast<int>(std::floor(x)), static_cast<int>(std::floor(y)));
    }

    __forceinline static const Vector2Int XAoi2DGetIndex2D(const UInt64 index) {
        int x = static_cast<int>(static_cast<UInt32>(index >> 32)); // 高32位转int（自动处理符号）
        int y = static_cast<int>(static_cast<UInt32>(index & 0xFFFFFFFF)); // 低32位转int
        return Vector2Int(x, y);
    }

    class XAoi2D {
    public:
        virtual ~XAoi2D() {
            for (auto itor = _Grids.begin(); itor != _Grids.end(); itor++) {
                XPOOL_RELEASE(s_GridPool, itor->second);
            }

            for (auto itor = _Entitys.begin(); itor != _Entitys.end(); itor++) {
                XPOOL_RELEASE(s_EntityPool, itor->second);
            }
        }

        const UInt64 _AoiID;
        const float _GridSize;

        XAoi2D(const UInt64 id, float gridSize) : _AoiID(id), _GridSize(gridSize) {
            _Temp.reserve(1024);
        }


        struct Entity;
        struct Grid {
            const UInt64 _Index;
            const Vector2Int _Index2d;

            __forceinline void InsertEntity(Entity* const entity) {
                if (_Entitys.find(entity) != _Entitys.end()) {
                    XASSERT(false, "wtf");
                    return;
                }
                //DEBUG(nullptr, "Grid %llx InsertEntity entity %llu", this, entity->_Guid);
                _Entitys.insert(entity);
            }

            __forceinline void RemoveEntity(Entity* const entity) {
                if (_Entitys.end() == _Entitys.find(entity)) {
                    XASSERT(false, "wtf");
                    return;
                }

                //DEBUG(nullptr, "Grid %llx RemoveEntity entity %llu", this, entity->_Guid);
                _Entitys.erase(entity);
            }

            __forceinline const std::unordered_set<Entity*>& GetEntitys() const { return _Entitys; }

            Grid(const Vector2Int& index) :
                _Index(XAoi2DCalcGridIndex(index.x, index.y)), _Index2d(index) {
            }

        private:
            std::unordered_set<Entity*> _Entitys;
        };

        struct Entity {
            const UInt64 _Guid;
            int _ViewRange;
            Grid* _Grid;

            Entity(const UInt64 guid, const int viewRange, Grid* const grid) : _Guid(guid), _ViewRange(viewRange), _Grid(grid) {}

            __forceinline void InsertIntoVisual(Entity* const entity) {
                if (_Visual.end() != _Visual.find(entity->_Guid)) {
                    XASSERT(false, "wtf");
                    return;
                }

                _Visual.insert(entity->_Guid);
            }

            __forceinline void RemoveFromVisual(Entity* const entity) {
                if (_Visual.end() == _Visual.find(entity->_Guid)) {
                    XASSERT(false, "wtf");
                    return;
                }

                _Visual.erase(entity->_Guid);
            }

            __forceinline const std::unordered_set<UInt64>& GetVisual() const { return _Visual; }
        private:
            std::unordered_set<UInt64> _Visual;
        };

        __forceinline Grid* QueryGrid(const float x, const float y) {
            const UInt64 gridIndex = XAoi2DCalcGridIndex(x / _GridSize, y / _GridSize);
            auto itor = _Grids.find(gridIndex);
            if (_Grids.end() != itor)
                return itor->second;
            return nullptr;
        }

        __forceinline Grid* QueryGrid(const int x, const int y) {
            auto itor = _Grids.find(XAoi2DCalcGridIndex(x, y));
            if (_Grids.end() == itor) {
                return nullptr;
            }
            return itor->second;
        }

        __forceinline Grid* CreateGrid(const float x, const float y) {
            Vector2Int index2d(std::floor(x / _GridSize), std::floor(y / _GridSize));
            const UInt64 gridIndex = XAoi2DCalcGridIndex(index2d.x, index2d.y);
            auto itor = _Grids.find(gridIndex);
            if (_Grids.end() != itor) {
                XASSERT(false, "wtf");
                return nullptr;
            }

            Grid* grid = XPOOL_CREATE(s_GridPool, index2d);
            XASSERT(grid->_Index == gridIndex, "wtf");
            _Grids.insert({ gridIndex, grid });
            return grid;
        }

        using fCallback = std::function<void(const std::vector<UInt64>&)>;
        using fSingleCallback = std::function<void(const UInt64 neighbor)>;
        bool Insert(UInt64 guid, const Vector2& pos, int viewRange, const fSingleCallback& onAppear) {
            if (_Entitys.find(guid) != _Entitys.end()) {
                XASSERT(false, "guid %llu already in this aoi", guid);
                return false;
            }

            Grid* grid = QueryGrid(pos.x, pos.y);
            if (nullptr == grid) {
                grid = CreateGrid(pos.x, pos.y);
            }

            Entity* const entity = XPOOL_CREATE(s_EntityPool, guid, viewRange, grid);
            Vector2Int index2d(std::floor(pos.x / _GridSize), std::floor(pos.y / _GridSize));
            for (int x = index2d.x - viewRange; x <= index2d.x + viewRange; x++) {
                for (int y = index2d.y - viewRange; y <= index2d.y + viewRange; y++) {
                    Grid* const neighborGrid = QueryGrid(x, y);
                    if (neighborGrid) {
                        for (auto itor = neighborGrid->GetEntitys().begin(); itor != neighborGrid->GetEntitys().end(); itor++) {
                            (*itor)->InsertIntoVisual(entity);
                            entity->InsertIntoVisual(*itor);
                            onAppear((*itor)->_Guid);
                        }
                    }
                }
            }

            grid->InsertEntity(entity);
            _Entitys.insert({ entity->_Guid, entity });
            return true;
        }

        void Remove(UInt64 guid, const fSingleCallback& onDisappear) {
            auto itor = _Entitys.find(guid);
            if (_Entitys.end() == itor) {
                XASSERT(false, "wtf");
                return;
            }

            Entity* const entity = itor->second;
            for (auto it = entity->GetVisual().begin(); it != entity->GetVisual().end(); it++) {
                QueryEntity(*it)->RemoveFromVisual(entity);
                onDisappear(QueryEntity(*it)->_Guid);
            }

            entity->_Grid->RemoveEntity(entity);
            if (entity->_Grid->GetEntitys().empty()) {
                _Grids.erase(entity->_Grid->_Index);
                XPOOL_RELEASE(s_GridPool, entity->_Grid);
            }
            _Entitys.erase(itor);
            XPOOL_RELEASE(s_EntityPool, entity);
        }

        void UpdatePosition(const UInt64 guid, const Vector2& pos, const fCallback& onAppear, const fCallback& onDisappear) {
            auto itor = _Entitys.find(guid);
            if (_Entitys.end() == itor) {
                XASSERT(false, "wtf");
                return;
            }

            Entity* const entity = itor->second;
            Vector2Int newGridIndex2d(std::floor(pos.x / _GridSize), std::floor(pos.y / _GridSize));
            if (entity->_Grid->_Index2d == newGridIndex2d) {
                return;
            }

            _Temp.clear();
            for (int oldX = entity->_Grid->_Index2d.x - entity->_ViewRange; oldX <= entity->_Grid->_Index2d.x + entity->_ViewRange; oldX++) {
                for (int oldY = entity->_Grid->_Index2d.y - entity->_ViewRange; oldY <= entity->_Grid->_Index2d.y + entity->_ViewRange; oldY++) {
                    if (oldX < newGridIndex2d.x - entity->_ViewRange
                        || oldX > newGridIndex2d.x + entity->_ViewRange
                        || oldY < newGridIndex2d.y - entity->_ViewRange
                        || oldY > newGridIndex2d.y + entity->_ViewRange) {

                        //当entity所站的格子变了，则将在老格子中但是不在新的viewRange范围内的格子内的所有人都跟当前entity双向剔除视野
                        Grid* const grid = QueryGrid(oldX, oldY);
                        if (grid) {
                            for (auto itOtherEntity = grid->GetEntitys().begin(); itOtherEntity != grid->GetEntitys().end(); itOtherEntity++) {
                                if ((*itOtherEntity) != entity) {
                                    _Temp.push_back((*itOtherEntity)->_Guid);
                                    (*itOtherEntity)->RemoveFromVisual(entity);
                                    entity->RemoveFromVisual((*itOtherEntity));
                                }
                            }
                        }
                    }
                }
            }
            onDisappear(_Temp);
            _Temp.clear();
            for (int newX = newGridIndex2d.x - entity->_ViewRange; newX <= newGridIndex2d.x + entity->_ViewRange; newX++) {
                for (int newY = newGridIndex2d.y - entity->_ViewRange; newY <= newGridIndex2d.y + entity->_ViewRange; newY++) {
                    if (newX< entity->_Grid->_Index2d.x - entity->_ViewRange
                        || newX > entity->_Grid->_Index2d.x + entity->_ViewRange
                        || newY < entity->_Grid->_Index2d.y - entity->_ViewRange
                        || newY > entity->_Grid->_Index2d.y + entity->_ViewRange) {

                        //当entity所站的格子变了，则不在老格子里但是在新格子里的entity加入视野
                        Grid* const grid = QueryGrid(newX, newY);
                        if (grid) {
                            for (auto itOtherEntity = grid->GetEntitys().begin(); itOtherEntity != grid->GetEntitys().end(); itOtherEntity++) {
                                if ((*itOtherEntity) != entity) {
                                    _Temp.push_back((*itOtherEntity)->_Guid);
                                    (*itOtherEntity)->InsertIntoVisual(entity);
                                    entity->InsertIntoVisual((*itOtherEntity));
                                }
                            }
                        }
                    }
                }
            }


            entity->_Grid->RemoveEntity(entity);
            if (entity->_Grid->GetEntitys().empty()) {
                _Grids.erase(entity->_Grid->_Index);
                XPOOL_RELEASE(s_GridPool, entity->_Grid);
            }

            Grid* newGrid = QueryGrid(newGridIndex2d.x, newGridIndex2d.y);
            if (nullptr == newGrid) {
                newGrid = CreateGrid(pos.x, pos.y);
            }

            entity->_Grid = newGrid;
            newGrid->InsertEntity(entity);
            onAppear(_Temp);
        }

        void UpdateEntityViewRange(const UInt64 guid, const int viewRange, const fCallback& onAppear, const fCallback& onDisappear) {

        }

        using fQueryVisualCallback = std::function<void(const std::unordered_set<UInt64>&)>;
        void QueryVisual(UInt64 guid, const fQueryVisualCallback& callback) const {
            auto itor = _Entitys.find(guid);
            if (_Entitys.end() == itor) {
                XASSERT(false, "wtf");
                return;
            }

            callback(itor->second->GetVisual());
        }

        using fQueryGridVisualCallback = std::function<void(const std::vector<UInt64>& visual)>;
        void GetGridsVisual(const Vector2& pos, const float radius, const fQueryGridVisualCallback& callback) {
            const int offset = std::ceil(radius / _GridSize) * 2;
            Vector2Int index(std::floor(pos.x / _GridSize), std::floor(pos.y / _GridSize));

            _Temp.clear();
            for (int x = index.x - offset; x <= index.x + offset; x++) {
                for (int y = index.y - offset; y <= index.y + offset; y++) {
                    Grid* const grid = QueryGrid(x, y);
                    if (grid) {
                        for (auto it = grid->GetEntitys().begin(); it != grid->GetEntitys().end(); it++) {
                            _Temp.push_back((*it)->_Guid);
                        }
                    }
                }
            }

            callback(_Temp);
        }

    private:
        __forceinline Entity* QueryEntity(const UInt64 guid) const {
            auto it = _Entitys.find(guid);
            if (_Entitys.end() == it) {
                XASSERT(false, "wtf");
                return nullptr;
            }

            return it->second;
        }

    private:
        std::unordered_map<UInt64, Grid*> _Grids;
        std::unordered_map<UInt64, Entity*> _Entitys;

        inline static XPool<Grid> s_GridPool;
        inline static XPool<Entity> s_EntityPool;

        std::vector<UInt64> _Temp;
    };
}
