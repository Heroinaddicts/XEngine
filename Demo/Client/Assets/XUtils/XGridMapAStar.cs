using System;
using System.Collections.Generic;
using UnityEngine;

namespace XUtils
{
    public class XGridMapAStar
    {
        public delegate bool fCheckGrid(in Vector2Int gridIndex);
        public delegate Vector2 fGridToWorldPosition(in Vector2Int grid);
        public delegate Vector2Int fWorldPositionToGrid(in Vector2 pos);

        private struct Node
        {
            public Vector2Int _Grid;
            public float _G;
            public float _H;
            public int _ParentIndex; // -1 表示无父节点
            public float _F => _G + _H;
        }

        private class NodeComparer : IComparer<int>
        {
            private readonly Node[] _Nodes;
            public NodeComparer(Node[] nodes) => _Nodes = nodes;
            public int Compare(int a, int b)
            {
                return _Nodes[a]._F.CompareTo(_Nodes[b]._F);
            }
        }

        private readonly fCheckGrid _CheckGridFunction;
        private readonly fGridToWorldPosition _GridToWorldPositionFunction;
        private readonly fWorldPositionToGrid _WorldPositionToGridFunction;

        private static readonly Vector2Int[] _Directions = new Vector2Int[]
        {
            new Vector2Int(1,0), new Vector2Int(-1,0),
            new Vector2Int(0,1), new Vector2Int(0,-1),
            new Vector2Int(1,1), new Vector2Int(-1,1),
            new Vector2Int(1,-1), new Vector2Int(-1,-1)
        };

        private readonly Node[] _Nodes;
        private int _NodeCount;
        private readonly XPriorityQueue<int> _OpenList;
        private readonly bool[,] _ClosedSet;
        private readonly NodeComparer _Comparer;

        private readonly int _MaxMapWidth;
        private readonly int _MaxMapHeight;

        public XGridMapAStar(
            in fCheckGrid checkFunc,
            in fGridToWorldPosition gridToWorldFunc,
            in fWorldPositionToGrid worldToGridFunc,
            int maxMapWidth = 500,
            int maxMapHeight = 500)
        {
            _CheckGridFunction = checkFunc;
            _GridToWorldPositionFunction = gridToWorldFunc;
            _WorldPositionToGridFunction = worldToGridFunc;

            _MaxMapWidth = maxMapWidth;
            _MaxMapHeight = maxMapHeight;

            int maxNodes = _MaxMapWidth * _MaxMapHeight;
            _Nodes = new Node[maxNodes];
            _Comparer = new NodeComparer(_Nodes);
            _OpenList = new XPriorityQueue<int>(_Comparer);
            _ClosedSet = new bool[_MaxMapWidth, _MaxMapHeight];
        }

        private static float Heuristic(Vector2Int a, Vector2Int b)
        {
            return Vector2Int.Distance(a, b);
        }

        public void FindPath(in Vector2 startPos, in Vector2 endPos, in Action<List<Vector2>> ret)
        {
            _OpenList.Clear();
            Array.Clear(_ClosedSet, 0, _ClosedSet.Length);
            _NodeCount = 0;

            var startGrid = _WorldPositionToGridFunction(startPos);
            var endGrid = _WorldPositionToGridFunction(endPos);

            _Nodes[_NodeCount] = new Node
            {
                _Grid = startGrid,
                _G = 0,
                _H = Heuristic(startGrid, endGrid),
                _ParentIndex = -1
            };
            _OpenList.Push(_NodeCount);
            _NodeCount++;

            int foundIndex = -1;

            while (_OpenList.Count > 0)
            {
                int currentIndex = _OpenList.Pop();
                Node current = _Nodes[currentIndex];

                if (current._Grid == endGrid)
                {
                    foundIndex = currentIndex;
                    break;
                }

                _ClosedSet[current._Grid.x, current._Grid.y] = true;

                foreach (var dir in _Directions)
                {
                    Vector2Int nextGrid = current._Grid + dir;

                    if (nextGrid.x < 0 || nextGrid.x >= _MaxMapWidth || nextGrid.y < 0 || nextGrid.y >= _MaxMapHeight)
                        continue;

                    if (_ClosedSet[nextGrid.x, nextGrid.y])
                        continue;

                    if (!_CheckGridFunction(nextGrid))
                        continue;

                    float cost = (dir.x != 0 && dir.y != 0) ? 1.414f : 1f;
                    float newG = current._G + cost;

                    int nextIndex = -1;
                    for (int i = 0; i < _NodeCount; i++)
                    {
                        if (_Nodes[i]._Grid == nextGrid)
                        {
                            nextIndex = i;
                            break;
                        }
                    }

                    if (nextIndex == -1)
                    {
                        nextIndex = _NodeCount;
                        _Nodes[_NodeCount] = new Node
                        {
                            _Grid = nextGrid,
                            _G = newG,
                            _H = Heuristic(nextGrid, endGrid),
                            _ParentIndex = currentIndex
                        };
                        _OpenList.Push(nextIndex);
                        _NodeCount++;
                    }
                    else
                    {
                        if (newG < _Nodes[nextIndex]._G)
                        {
                            Node n = _Nodes[nextIndex];
                            n._G = newG;
                            n._ParentIndex = currentIndex;
                            _Nodes[nextIndex] = n;
                            _OpenList.Push(nextIndex);
                        }
                    }
                }
            }

            var path = new List<Vector2>();
            if (foundIndex != -1)
            {
                int idx = foundIndex;
                while (idx != -1)
                {
                    path.Add(_GridToWorldPositionFunction(_Nodes[idx]._Grid));
                    idx = _Nodes[idx]._ParentIndex;
                }
                path.Reverse();
            }

            ret?.Invoke(path);
        }
    }
}
