using Spine;
using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;
using XEngine;
using XGame;
using XUtils;
using static XUtils.XAStar;

public class GameTilemap : MonoBehaviour, IGameTilemap
{
    [SerializeField]
    Tilemap _Groud;

    GameObject _Builds;

    private float _NodeSizeX;//格子宽高
    private float _NodeSizeY;
    private int _MapHeight;//地图宽高
    private int _MapWidth;
    public float[,][] _TileDataTable;//移动消耗

    XAStar _AStar;

    private Transform _Transform;
    private MeshRenderer _MeshRenderer;

    public Material _BuildsMaterial;

    [Header("遮挡设置")]
    [Range(0.1f, 1.0f)] public float baseOpacity = 0.1f;  // 遮挡时的透明度
    [Range(1.0f, 5.0f)] public float occlusionRange = 2.5f;
    [Range(0.1f, 2.0f)] public float occlusionThreshold = 0.3f;

    private Transform _PlayerTransform;
    private List<Transform> _BuildsLab = new List<Transform>();

    Dictionary<string, Tilemap> _Layers = new Dictionary<string, Tilemap>();

    void Awake()
    {
        InitMapInfoForTileMap();
        _NodeSizeX = _Groud.cellSize.x;
        _NodeSizeY = _Groud.cellSize.y;
        _TileDataTable = new float[_MapWidth, _MapHeight][];

        if (_MapWidth > 0 && _MapHeight > 0)
        {
            _AStar = new XAStar(
                (Vector2Int grid) =>
                {
                    Vector3 WorldPos = GridToWorldIsometric(grid);
                    return CanMove(WorldPos);
                },
                (Vector2Int grid) => GridToWorldIsometric(grid),
                (Vector2 pos) => WorldToGridIsometric(pos)
            );
        }
        SetBuildsMaterial();
    }

    void Start()
    {
        for(int i=0; i < transform.childCount; i++)
        {
            Transform child = transform.GetChild(i);
            Tilemap tilemap = child.GetComponent<Tilemap>();

            if (null != tilemap)
            {
                _Layers.Add(child.name, tilemap);
            }
        }
    }

    void FixedUpdate()
    {
        UpdateBuildingOcclusion();
    }

    public bool CanMove(in Vector2 pos, in string ground = null)
    {
        Vector3 WorldPos = new Vector3(pos.x, pos.y, _Groud.transform.position.z);
        Vector3Int cellPos = _Groud.WorldToCell(WorldPos);

        if(null == ground)
        {
            return _Groud.HasTile(cellPos);
        } else
        {
            Tilemap tilemap = null;
            if (_Layers.TryGetValue(ground, out tilemap))
            {
                return tilemap.HasTile(cellPos);
            }
        }

        return false;
    }

    public void FindPath(in Vector2 start, in Vector2 end, fXStarPathResult callback)
    {
        _AStar.FindPath(start, end, callback);
    }


    public bool Raycast(in Vector2 origin, in float angle, in float distance, ref Vector2 hit)
    {
        Int64 tick = SafeSystem.Time.GetMillisecondTick();

        Vector2 dir = new Vector2(Mathf.Cos(angle * Mathf.Deg2Rad), Mathf.Sin(angle * Mathf.Deg2Rad));
        float step = 0.1f;
        float traveled = 0f;

        Vector2 pos = origin;
        while (traveled <= distance)
        {
            Vector2Int cellPos = WorldToGridIsometric(pos);

            if (!_Groud.HasTile(new Vector3Int(cellPos.x, cellPos.y, 0)))
            {
                hit = pos;
                XApi.Trace($"Raycast use tick {SafeSystem.Time.GetMillisecondTick() - tick}");
                return true;
            }

            pos += dir * step;
            traveled += step;
        }

        XApi.Trace($"Raycast use tick {SafeSystem.Time.GetMillisecondTick() - tick}");
        return false;
    }

    public Vector2Int WorldToGridIsometric(Vector2 WorldPos)
    {
        float gridX = (WorldPos.x / (_NodeSizeX / 2) + WorldPos.y / (_NodeSizeY / 2)) / 2;
        float gridY = (WorldPos.y / (_NodeSizeY / 2) - WorldPos.x / (_NodeSizeX / 2)) / 2;

        return new Vector2Int(Mathf.FloorToInt(gridX), Mathf.FloorToInt(gridY));
    }

    public Vector2 GridToWorldIsometric(Vector2Int gridPos)
    {
        float worldX = (gridPos.x - gridPos.y) * _NodeSizeX / 2;
        float worldY = (gridPos.x + gridPos.y) * _NodeSizeY / 2 + _NodeSizeY / 2;

        return new Vector2(worldX, worldY);
    }

    public void InitMapInfoForTileMap()
    {
        if (_Groud == null)
        {
            //Debug.LogError("Tilemap未设置");
            return;
        }
        BoundsInt actualBounds;
        BoundsInt cellBounds = _Groud.cellBounds;

        int minX = int.MaxValue;
        int maxX = int.MinValue;
        int minY = int.MaxValue;
        int maxY = int.MinValue;

        bool foundTile = false;

        for (int x = cellBounds.x; x < cellBounds.x + cellBounds.size.x; x++)
        {
            for (int y = cellBounds.y; y < cellBounds.y + cellBounds.size.y; y++)
            {
                Vector3Int pos = new Vector3Int(x, y, 0);
                if (_Groud.HasTile(pos))
                {
                    foundTile = true;
                    minX = Mathf.Min(minX, x);
                    maxX = Mathf.Max(maxX, x);
                    minY = Mathf.Min(minY, y);
                    maxY = Mathf.Max(maxY, y);
                }
            }
        }
        if (!foundTile)
        {
            actualBounds = new BoundsInt(0, 0, 0, 0, 0, 0);//没找到就返回全0边界
        }
        actualBounds = new BoundsInt(minX, minY, 0, maxX - minX + 1, maxY - minY + 1, 1);

        if (actualBounds.size.x == 0 || actualBounds.size.y == 0)
        {
            Debug.LogWarning("Tilemap无有效地图,使用默认尺寸");
            this._MapWidth = 30;
            this._MapHeight = 20;
        }
        else
        {
            this._MapWidth = actualBounds.size.x;
            this._MapHeight = actualBounds.size.y;

            Debug.Log($"从实际tile获取地图尺寸: 宽{_MapWidth}, 高{_MapHeight}, " +
         $"实际边界: Position: ({actualBounds.x}, {actualBounds.y}), " +
         $"Size: ({actualBounds.size.x}, {actualBounds.size.y})");
            this._NodeSizeX = _Groud.cellSize.x;
            this._NodeSizeY = _Groud.cellSize.y;
        }
        VisualizeMapBounds(actualBounds);
        VisualizeWalkableTiles(actualBounds);
    }

    private void SetBuildsMaterial()
    {
        GameObject player = GameObject.FindGameObjectWithTag("Player");
        if (player != null)
        {
            _PlayerTransform = player.transform;
        }

        Transform builds = transform.Find("Build");
        if (builds != null)
        {
            _BuildsLab.Clear();
            for (int i = 0; i < builds.childCount; i++)
            {
                Transform child = builds.GetChild(i);
                SpriteRenderer renderer = child.GetComponent<SpriteRenderer>();
                if (renderer != null)
                {
                    Material instanceMaterial = new Material(_BuildsMaterial);

                    if (renderer.sprite != null)
                    {
                        instanceMaterial.SetTexture("_MainTexture", renderer.sprite.texture);

                        // instanceMaterial.SetTextureScale("_MainTexture", renderer.material.GetTextureScale("_MainTexture"));
                        // instanceMaterial.SetTextureOffset("_MainTexture", renderer.material.GetTextureOffset("_MainTexture"));
                    }

                    renderer.material = instanceMaterial;

                    _BuildsLab.Add(child);
                }

            }
        }
    }
    private void UpdateBuildingOcclusion()
    {
        if (_PlayerTransform == null || _BuildsMaterial == null)
        {
            return;
        }

        foreach (Transform build in _BuildsLab)
        {
            if (build == null)
            {
                continue;
            }
            float YDistance = build.position.y - _PlayerTransform.position.y;

            float actualDistance = Vector2.Distance(new Vector2(build.position.x, build.position.y),
                                                  new Vector2(_PlayerTransform.position.x, _PlayerTransform.position.y));

            if (YDistance < 0 && actualDistance < occlusionRange)
            {
                float distanceFactor = 1.0f - (actualDistance / occlusionRange);
                float finalOpacity = Mathf.Lerp(baseOpacity, 1.0f, distanceFactor);

                SpriteRenderer renderer = build.GetComponent<SpriteRenderer>();
                if (renderer != null && renderer.material != null)
                {
                    renderer.material.SetFloat("_BaseOpacity", finalOpacity);
                }
            }
            else
            {
                SpriteRenderer renderer = build.GetComponent<SpriteRenderer>();
                if (renderer != null && renderer.material != null)
                {
                    renderer.material.SetFloat("_BaseOpacity", 1.0f);
                }
            }
        }
    }


    public void ReleaseTilemap()
    {
        Destroy(this.gameObject);
    }
    private void VisualizeMapBounds(BoundsInt bounds)
    {
#if UNITY_EDITOR
        // 在Scene视图中绘制isometric视角的边界框，方便调试查看
        Vector3 cellSize = _Groud.cellSize;

        // 获取边界框的四个角点（isometric坐标系）
        Vector3Int bottomLeftCell = new Vector3Int(bounds.x, bounds.y, 0);
        Vector3Int bottomRightCell = new Vector3Int(bounds.x + bounds.size.x, bounds.y, 0);
        Vector3Int topLeftCell = new Vector3Int(bounds.x, bounds.y + bounds.size.y, 0);
        Vector3Int topRightCell = new Vector3Int(bounds.x + bounds.size.x, bounds.y + bounds.size.y, 0);

        // 转换为世界坐标（使用isometric转换）
        Vector3 bottomLeft = GridToWorldIsometric(new Vector2Int(bottomLeftCell.x, bottomLeftCell.y));
        Vector3 bottomRight = GridToWorldIsometric(new Vector2Int(bottomRightCell.x, bottomRightCell.y));
        Vector3 topLeft = GridToWorldIsometric(new Vector2Int(topLeftCell.x, topLeftCell.y));
        Vector3 topRight = GridToWorldIsometric(new Vector2Int(topRightCell.x, topRightCell.y));

        // 绘制边界框（isometric视角）
        Debug.DrawLine(bottomLeft, topLeft, Color.red, 100f);
        Debug.DrawLine(topLeft, topRight, Color.red, 100f);
        Debug.DrawLine(topRight, bottomRight, Color.red, 100f);
        Debug.DrawLine(bottomRight, bottomLeft, Color.red, 100f);

        // 绘制对角线，增强可视化效果
        Debug.DrawLine(bottomLeft, topRight, Color.yellow, 100f);
        Debug.DrawLine(bottomRight, topLeft, Color.yellow, 100f);

        // 在中心位置显示文字信息
        Vector3 center = GridToWorldIsometric(new Vector2Int(
            bounds.x + bounds.size.x / 2,
            bounds.y + bounds.size.y / 2));
        Debug.Log($"Isometric地图边界已可视化: 宽{bounds.size.x}, 高{bounds.size.y}");
#endif
    }
    private void VisualizeWalkableTiles(BoundsInt bounds)
    {
#if UNITY_EDITOR
        int walkableCount = 0;
        int nonWalkableCount = 0;

        // 遍历实际地图边界内的所有格子
        for (int x = bounds.x; x < bounds.x + bounds.size.x; x++)
        {
            for (int y = bounds.y; y < bounds.y + bounds.size.y; y++)
            {
                Vector3Int cellPos = new Vector3Int(x, y, 0);

                // 将单元格坐标转换为世界坐标用于可视化
                Vector3 worldPos = _Groud.CellToWorld(cellPos);
                // 将世界坐标稍微抬高一点，避免与地面重叠
                worldPos += Vector3.up * 0.01f;

                // 直接检查Tilemap上该位置是否有瓦片
                if (_Groud.HasTile(cellPos))
                {
                    // 在可行走的位置绘制绿色标记
                    Debug.DrawRay(worldPos, Vector3.up * 0.1f, Color.green, 100f);
                    walkableCount++;
                }
                else
                {
                    // 在不可行走的位置绘制红色标记
                    Debug.DrawRay(worldPos, Vector3.up * 0.1f, Color.red, 100f);
                    nonWalkableCount++;
                }
            }
        }
        Debug.Log($"已完成可行走区域可视化: 可行走{walkableCount}格，不可行走{nonWalkableCount}格。边界({bounds.x},{bounds.y}) 到 ({bounds.x + bounds.size.x - 1},{bounds.y + bounds.size.y - 1})");
#endif
    }
}
