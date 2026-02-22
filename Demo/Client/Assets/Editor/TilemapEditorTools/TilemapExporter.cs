using UnityEngine;
using UnityEditor;
using UnityEngine.Tilemaps;
using System.IO;
using System.Xml;

public static class TilemapExporter
{
    private const string InputDir = "Assets/Resources/Tilemap/Prefabs";
    private const string OutputDir = "Assets/../../Server/Bin/Windows/Debug/Tilemap/";

    [MenuItem("Tools/Tilemap/批量导出 Isometric Tilemap 为 XML")]
    public static void ExportAll()
    {
        string absOutput = Path.GetFullPath(OutputDir);
        if (!Directory.Exists(absOutput))
            Directory.CreateDirectory(absOutput);

        string[] guids = AssetDatabase.FindAssets("t:Prefab", new[] { "Assets/Resources/Tilemap/Prefabs" });
        if (guids.Length == 0)
        {
            Debug.LogWarning($"⚠️ 没有在 Tilemap/Prefabs 下找到任何 Prefab！");
            return;
        }

        foreach (string guid in guids)
        {
            GameObject prefab = AssetDatabase.LoadAssetAtPath<GameObject>(AssetDatabase.GUIDToAssetPath(guid));
            if (null != prefab)
            {
                ExportTilemapPrefab(prefab, absOutput);
            }
        }

        AssetDatabase.Refresh();
    }

    private static void ExportTilemapPrefab(GameObject prefab, string outPath)
    {
        Grid grid = prefab.GetComponentInChildren<Grid>();
        if (grid != null && (grid.cellLayout == GridLayout.CellLayout.Isometric || grid.cellLayout == GridLayout.CellLayout.IsometricZAsY))
        {
            Tilemap[] tilemaps = grid.GetComponentsInChildren<Tilemap>(true);
            if (tilemaps == null || tilemaps.Length == 0)
                return;

            outPath = Path.Combine(outPath, prefab.name + ".xml");

            var settings = new XmlWriterSettings
            {
                Indent = true,
                IndentChars = "  ",
                Encoding = new System.Text.UTF8Encoding(true)
            };

            using (var writer = XmlWriter.Create(outPath, settings))
            {
                writer.WriteStartDocument(true);
                writer.WriteStartElement("Tilemap");
                writer.WriteAttributeString("Name", prefab.name);

                foreach (var tm in tilemaps)
                {
                    ExportTilemapNode(writer, tm, grid.cellSize);
                }

                writer.WriteEndElement(); // TilemapPrefab
                writer.WriteEndDocument();
            }

            Debug.Log($"✅ 导出 Tilemap：{prefab.name} -> {outPath}");
        }
    }

    private static void ExportTilemapNode(XmlWriter writer, Tilemap tm, in Vector3 cellSize)
    {
        writer.WriteStartElement("Layer");
        writer.WriteAttributeString("Name", tm.name);
        writer.WriteAttributeString("CellSizeX", System.Math.Round(cellSize.x, 2).ToString("G9"));
        writer.WriteAttributeString("CellSizeY", System.Math.Round(cellSize.y, 2).ToString("G9"));

        BoundsInt bounds = tm.cellBounds;
        foreach (var pos in bounds.allPositionsWithin)
        {
            TileBase tile = tm.GetTile(pos);
            if (tile == null) continue;

            writer.WriteStartElement("Tile");
            writer.WriteAttributeString("X", pos.x.ToString());
            writer.WriteAttributeString("Y", pos.y.ToString());
            writer.WriteEndElement(); // Tile
        }

        writer.WriteEndElement(); // Layer
    }
}
