using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

[ExecuteInEditMode]
public class BossRadiusVisualization : MonoBehaviour
{
    [Header("Boss Configuration")]
    public int bossConfigId;

    [Header("Visualization Settings")]
    public bool showVisualization = true;
    public Color radiusColor = new Color(1f, 0f, 0f, 0.7f);
    public Color radiusOutlineColor = new Color(1f, 0.5f, 0.5f, 1f);
    public float gizmoSize = 1.0f;

    private AutoConfig.MonsterBoss bossConfig;

    void OnDrawGizmos()
    {
        if (!showVisualization) return;

        UpdateBossConfig();
        if (bossConfig == null) return;

        DrawBossRadiusVisualization();
    }

    void UpdateBossConfig()
    {
        if (bossConfigId > 0)
        {
            try
            {
                bossConfig = AutoConfig.MonsterBoss.Get(bossConfigId);
            }
            catch
            {
                bossConfig = null;
            }
        }
        else
        {
            bossConfig = null;
        }
    }

    void DrawBossRadiusVisualization()
    {
        // 绘制Boss身体半径
        DrawRadiusAtPosition(transform.position);
    }

    void DrawRadiusAtPosition(Vector3 position)
    {
#if UNITY_EDITOR
        if (bossConfig != null)
        {
            float radius = bossConfig._MonsterBody;
            if (radius > 0)
            {
                // 绘制Boss身体半径（圆形表示范围）
                DrawCircleRadius(position, radius);
            }
        }
#endif
    }

    void DrawCircleRadius(Vector3 position, float radius)
    {
#if UNITY_EDITOR
        // 绘制范围主体（半透明圆盘）
        Handles.color = radiusColor;
        Handles.DrawSolidDisc(position, Vector3.forward, radius);

        // 绘制范围轮廓
        Handles.color = radiusOutlineColor;
        Handles.DrawWireDisc(position, Vector3.forward, radius);

        // 绘制十字线辅助线
        Handles.color = new Color(1, 1, 1, 0.7f);
        Handles.DrawLine(position + Vector3.left * radius * 0.5f, position + Vector3.right * radius * 0.5f);
        Handles.DrawLine(position + Vector3.down * radius * 0.5f, position + Vector3.up * radius * 0.5f);
#endif
    }
}