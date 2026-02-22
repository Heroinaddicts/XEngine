using System;
using System.Collections.Generic;
using UnityEngine;

namespace XUtils
{
    public class XAStar
    {
        private class Node
        {
            public UInt32 _G;
            public UInt32 _H;
            public readonly Vector2Int _Grid;
            public Node _Parent;

            public UInt32 Score => _G + _H;

            public Node(in Vector2Int grid, in Node parent = null)
            {
                _Grid = grid;
                _G = 0;
                _H = 0;
                _Parent = parent;
            }
        }

        const int StraightNeighborV = 10;
        const int DiagonalNeighborV = 14;

        private enum eNeighbor
        {
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

        static readonly Vector2Int[] s_NeighborDiff = {  // 数组声明推荐写成 Vector2Int[] 形式（更规范）
            new Vector2Int(-1, 1),  new Vector2Int(0, 1),  new Vector2Int(1, 1),
            new Vector2Int(-1, 0),                        new Vector2Int(1, 0),
            new Vector2Int(-1, -1), new Vector2Int(0, -1), new Vector2Int(1, -1)
        };

        public delegate bool fXAStarCheckGrid(Vector2Int grid);
        public delegate Vector2 fXAStarGridToWorld(Vector2Int grid);
        public delegate Vector2Int fXAStarWorldToGrid(Vector2 world);
        public delegate void fXStarPathResult(List<Vector2> path);

        readonly fXAStarCheckGrid _FunCheckGrid;
        readonly fXAStarGridToWorld _FunGridToWorld;
        readonly fXAStarWorldToGrid _FunWorldToGrid;

        readonly List<Node> _Open = new List<Node>(2048);
        readonly List<Node> _Closed = new List<Node>(2048);
        readonly List<Vector2> _Path = new List<Vector2>(2048);

        Vector2Int GetDelta(in Vector2Int source, in Vector2Int target)
        {
            return new Vector2Int(Math.Abs(source.x - target.x), Math.Abs(source.y - target.y));
        }

        UInt32 Heuristic(in Vector2Int source, in Vector2Int target)
        {
            Vector2Int delta = GetDelta(source, target);
            return (UInt32)(DiagonalNeighborV * Math.Max(delta.x, delta.y));
        }
        static Node FindNodeOnList(in List<Node> list, in Vector2Int grid)
        {
            for (int i = 0; i < list.Count; i++)
            {
                if (list[i]._Grid == grid)
                {
                    return list[i];
                }
            }

            return null;
        }

        public XAStar(in fXAStarCheckGrid funCheckGrid, in fXAStarGridToWorld funGridToWorld, in fXAStarWorldToGrid funWorldToGrid)
        {
            _FunCheckGrid = funCheckGrid;
            _FunGridToWorld = funGridToWorld;
            _FunWorldToGrid = funWorldToGrid;
        }

        public void FindPath(in Vector2 start, in Vector2 end, in fXStarPathResult ret)
        {
            _Open.Clear();
            _Closed.Clear();
            _Path.Clear();

            Vector2Int startGrid = _FunWorldToGrid(start);
            Vector2Int endGrid = _FunWorldToGrid(end);

            if (!_FunCheckGrid(startGrid) || !_FunCheckGrid(endGrid))
            {
                ret(_Path);
                return;
            }

            Node current = null;
            _Open.Add(new Node(startGrid));
            while (0 != _Open.Count)
            {
                current = _Open[0];
                int index = 0;
                for (int i = 0; i < _Open.Count; i++)
                {
                    if (_Open[i].Score < current.Score)
                    {
                        index = i;
                        current = _Open[i];
                    }
                }

                if (current._Grid == endGrid)
                {
                    break;
                }

                _Closed.Add(current);
                _Open.RemoveAt(index);

                for (UInt32 i = (UInt32)eNeighbor.__Start__; i < (UInt32)eNeighbor.__Count__; i++)
                {
                    Vector2Int newGrid = current._Grid + s_NeighborDiff[i];

                    if (!_FunCheckGrid(newGrid) || FindNodeOnList(_Closed, newGrid) != null)
                    {
                        continue;
                    }

                    UInt32 totalCost = current._G;
                    if ((UInt32)eNeighbor.Top == i || (UInt32)eNeighbor.Bottom == i || (UInt32)eNeighbor.Right == i || (UInt32)eNeighbor.Left == i)
                    {
                        totalCost += StraightNeighborV;
                    }
                    else
                    {
                        totalCost += DiagonalNeighborV;
                    }


                    Node successor = FindNodeOnList(_Open, newGrid);
                    if (successor == null)
                    {
                        successor = new Node(newGrid, current);
                        successor._G = totalCost;
                        successor._H = Heuristic(successor._Grid, endGrid);
                        _Open.Add(successor);
                    }
                    else if (totalCost < successor._G)
                    {
                        successor._Parent = current;
                        successor._G = totalCost;
                    }
                }
            }

            while (current != null)
            {
                Vector2 pos = _FunGridToWorld(current._Grid);
                if (_Path.Count >= 2)
                {
                    Vector2 pos1 = _Path[_Path.Count - 2];
                    Vector2 pos2 = _Path[_Path.Count - 1];

                    if (Math.Abs(Vector2.Angle(pos2, pos1) - Vector2.Angle(pos, pos2)) < 0.001f)
                    {
                        _Path.RemoveAt(_Path.Count - 1);
                    }
                }
                _Path.Add(pos);
                current = current._Parent;
            }

            _Open.Clear();
            _Closed.Clear();
            _Path.Reverse();
            ret(_Path);
            _Path.Clear();
        }
    }
}
