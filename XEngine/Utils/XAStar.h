#ifndef __XAStar_h__
#define __XAStar_h__

#include <vector>
#include <functional>
#include <cmath>
#include "Vector2.h"
#include "SafeMemory.h"
#include "XPool.h"

namespace XEngine {
    typedef std::function<bool(const Vector2Int&)> fXAStarCheckGrid;
    typedef std::function<Vector2(const Vector2Int&)> fXAStarGridToWorld;
    typedef std::function<Vector2Int(const Vector2&)> fXAStarWorldToGrid;
    typedef std::function<void(const std::vector<Vector2>&)> fXAStarFindPathResult;
    typedef std::function<bool(const Vector2&, const Vector2&)> fXAStarRaycast;

#define StraightNeighborV 10
#define DiagonalNeighborV 14

    class XAStar {
    public:
        virtual ~XAStar() {}
        XAStar(const fXAStarCheckGrid& check, const fXAStarGridToWorld& g2w, const fXAStarWorldToGrid& w2g, const fXAStarRaycast& raycast, const Vector2Int& grid)
            : _FunCheckGrid(check), _FunGridToWorld(g2w), _FunWorldToGrid(w2g), _FunRaycast(raycast) {
            _Open.reserve(2048);
            _Closed.reserve(2048);
            _Path.reserve(1024);
        }

        __forceinline void FindPath(const Vector2& start, const Vector2& end, const fXAStarFindPathResult& ret) {
            _Open.clear();
            _Closed.clear();
            _Path.clear();

            const Vector2Int startGrid = _FunWorldToGrid(start);
            const Vector2Int endGrid = _FunWorldToGrid(end);

            if (!_FunCheckGrid(startGrid) || !_FunCheckGrid(endGrid)) {
                ret({});
                return;
            }

            if (!_FunRaycast(start, end)) {
                ret({ end });
                return;
            }

            Node* current = nullptr;
            _Open.push_back(Node::Create(startGrid));
            while (!_Open.empty()) {
                auto current_it = _Open.begin();
                current = *current_it;

                for (auto it = _Open.begin(); it != _Open.end(); it++) {
                    auto node = *it;
                    if (node->GetScore() < current->GetScore()) {
                        current = node;
                        current_it = it;
                    }
                }

                if (current->_Grid == endGrid) {
                    break;
                }

                _Closed.push_back(current);
                _Open.erase(current_it);

                for (UInt32 i = eNeighbor::__Start__; i < eNeighbor::__Count__; i++) {
                    Vector2Int newGrid(current->_Grid + s_NeighborDiff[i]);
                    if (!_FunCheckGrid(newGrid) || FindNodeOnList(_Closed, newGrid)) {
                        continue;
                    }

                    UInt32 totalCost = current->_G + ((eNeighbor::Top == i || eNeighbor::Bottom == i || eNeighbor::Right == i || eNeighbor::Left == i) ? StraightNeighborV : DiagonalNeighborV);

                    Node* successor = FindNodeOnList(_Open, newGrid);
                    if (successor == nullptr) {
                        successor = Node::Create(newGrid, current);
                        successor->_G = totalCost;
                        successor->_H = Heuristic(successor->_Grid, endGrid);
                        _Open.push_back(successor);
                    }
                    else if (totalCost < successor->_G) {
                        successor->_Parent = current;
                        successor->_G = totalCost;
                    }
                }
            }

            while (current != nullptr) {
                Vector2 pos = _FunGridToWorld(current->_Grid);
                if (_Path.size() >= 2) {
                    Vector2 pos1 = _Path[_Path.size() - 2];
                    Vector2 pos2 = _Path[_Path.size() - 1];

                    if (std::fabs((pos2 - pos1).Angle() - (pos - pos2).Angle()) < 0.001f) {
                        _Path.pop_back();
                    }
                }
                _Path.push_back(pos);
                current = current->_Parent;
            }

            for (auto it = _Open.begin(); it != _Open.end(); it++) {
                Node::Release(*it);
            }
            _Open.clear();

            for (auto it = _Closed.begin(); it != _Closed.end(); it++) {
                Node::Release(*it);
            }
            _Closed.clear();

            if (_Path.size() >= 2) {
                if (!_FunRaycast(start, _Path[_Path.size() - 2])) {
                    _Path.pop_back();
                }
            }

            std::reverse(_Path.begin(), _Path.end());
            ret(_Path);
            _Path.clear();

            Node::PrintNodeCount();
        }

    private:
        enum eNeighbor {
            __Start__ = 0,
            TopLeft = __Start__,
            Top = 1,
            TopRight = 2,
            Left = 3,
            Right = 4,
            BottomLeft = 5,
            Bottom = 6,
            BottomRight = 7,

            __Count__ = 8
        };

        inline const static Vector2Int s_NeighborDiff[eNeighbor::__Count__] = {
            {-1, 1},  {0, 1}, {1, 1},
            {-1, 0},          {1, 0},
            {-1, -1}, {0, -1},{1, -1}
        };

        __forceinline Vector2Int GetDelta(const Vector2Int& source, const Vector2Int& target) {
            return{ abs(source.x - target.x),  abs(source.y - target.y) };
        }

        __forceinline UInt32 Heuristic(const Vector2Int& source, const Vector2Int& target) {
            Vector2Int delta = GetDelta(source, target);
            return static_cast<UInt32>(DiagonalNeighborV * std::max(delta.x, delta.y));
        }

    private:

        struct Node {
            virtual ~Node() { s_NodeCount--; }

            UInt32 _G, _H;
            const Vector2Int _Grid;
            Node* _Parent;
            __forceinline UInt32 GetScore() { return _G + _H; }

            static Node* Create(const Vector2Int& grid, Node* parent = nullptr) {
                return XPOOL_CREATE(s_Pool, grid, parent);
            }

            static void Release(Node* node) {
                XPOOL_RELEASE(s_Pool, node);
            }

            __forceinline static void PrintNodeCount() {
                //printf("PrintNodeCount %d\n", s_NodeCount);
            }

        private:
            friend XPool<Node>;
            Node(const Vector2Int& grid, Node* parent) : _G(0), _H(0), _Grid(grid), _Parent(parent) {
                s_NodeCount++;
            }

            inline static XPool<Node> s_Pool;
            inline static int s_NodeCount = 0;
        };

        typedef std::vector<Node*> Nodes;
        typedef std::vector<Vector2> PathList;

        __forceinline Node* FindNodeOnList(const Nodes& nodes, const Vector2Int& grid) {
            for (int i = 0; i < nodes.size(); i++) {
                if (nodes[i]->_Grid == grid) {
                    return nodes[i];
                }
            }

            return nullptr;
        }

    private:
        const fXAStarCheckGrid _FunCheckGrid;
        const fXAStarGridToWorld _FunGridToWorld;
        const fXAStarWorldToGrid _FunWorldToGrid;
        const fXAStarRaycast _FunRaycast;

        Nodes _Open, _Closed;
        PathList _Path;
    };
}

#endif // __XAStar_h__
