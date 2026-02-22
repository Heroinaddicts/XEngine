using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
#endif

[ExecuteInEditMode]
public class BulletVisualization : MonoBehaviour
{
    [Header("Bullet Configuration")]
    public int bulletConfigId;

    [Header("Visualization Settings")]
    public bool showVisualization = true;
    public Color pathColor = new Color(0f, 1f, 0f, 0.7f);
    public Color bulletBodyColor = new Color(1f, 1f, 0f, 0.5f);
    public Color bulletOutlineColor = new Color(1f, 0.8f, 0f, 1f);
    public float gizmoSize = 0.5f;

    private AutoConfig.Bullet bulletConfig;

    void OnDrawGizmos()
    {
        if (!showVisualization) return;

        UpdateBulletConfig();
        if (bulletConfig == null) return;

        DrawBulletVisualization();
    }

    void UpdateBulletConfig()
    {
        if (bulletConfigId > 0)
        {
            try
            {
                bulletConfig = AutoConfig.Bullet.Get(bulletConfigId);
            }
            catch
            {
                bulletConfig = null;
            }
        }
        else
        {
            bulletConfig = null;
        }
    }

    void DrawBulletVisualization()
    {
        // 绘制子弹当前位置
        DrawBulletAtPosition(transform.position, transform.right);

        // 根据子弹类型决定是否绘制路径
        switch (bulletConfig._Ballistics)
        {
            case AutoConfig.AutoEnum.eBallistics.Line:
                if (bulletConfig._Speed > 0)
                {
                    // 计算直线飞行终点
                    float lifetime = bulletConfig._LifeCycle / 1000f;
                    float distance = bulletConfig._Speed * lifetime;

                    // 考虑加速度的影响
                    if (bulletConfig._AddSpeed != 0)
                    {
                        // s = v0*t + 0.5*a*t^2
                        distance = bulletConfig._Speed * lifetime + 0.5f * bulletConfig._AddSpeed * lifetime * lifetime;
                    }

                    Vector3 direction = transform.right; // 默认向右
                    Vector3 targetPosition = transform.position + direction * distance;

                    // 绘制飞行路径
                    DrawTrajectory(transform.position, targetPosition);

                    // 在终点绘制子弹
                    //DrawBulletAtPosition(targetPosition, transform.right);
                }
                break;

            case AutoConfig.AutoEnum.eBallistics.Parabolic:
                // 抛物线类型子弹特殊处理
                // 这里简化处理，实际应该根据具体的目标点绘制曲线
                break;
        }
    }

    void DrawBulletAtPosition(Vector3 position, Vector3 direction)
    {
#if UNITY_EDITOR
        // 根据子弹的范围类型绘制不同形状
        switch (bulletConfig._RangeType)
        {
            case AutoConfig.AutoEnum.eRangeType.Circle:
                DrawCircleBullet(position, bulletConfig._Radius > 0 ? bulletConfig._Radius : gizmoSize);
                break;

            case AutoConfig.AutoEnum.eRangeType.Cube:
                // 矩形/立方体类型子弹
                float width = gizmoSize;
                float height = gizmoSize;

                // 如果配置了矩形尺寸，则使用配置的尺寸
                if (bulletConfig._RectangleSize != null && bulletConfig._RectangleSize.Length >= 2)
                {
                    width = bulletConfig._RectangleSize[1];
                    height = bulletConfig._RectangleSize[0];
                    //Debug.LogError($"使用配置尺寸绘制矩形子弹: 宽度={width}, 高度={height}");
                }
                //DrawRectangleBullet(position, width, height, direction);
                DrawRectangleBullet(position, height, width, direction);
                
                break;

            case AutoConfig.AutoEnum.eRangeType.Sector:
                // 扇形类型子弹
                float radius = bulletConfig._Radius > 0 ? bulletConfig._Radius : gizmoSize;
                float angle = bulletConfig._SectorAngle > 0 ? bulletConfig._SectorAngle : 30f;
                DrawSectorBullet(position, radius, angle, direction);
                break;
            case AutoConfig.AutoEnum.eRangeType.Ellipse:
                // 椭圆形类型子弹
                float ellipseWidth = gizmoSize;
                float ellipseHeight = gizmoSize;

                ellipseWidth = bulletConfig._Radius;
                ellipseHeight = bulletConfig._ShortRadius;

                DrawEllipseBullet(position, ellipseWidth, ellipseHeight, direction);
                break;


            default:
                // 默认绘制圆形子弹
                DrawCircleBullet(position, gizmoSize);
                break;
        }
#endif
    }

    void DrawCircleBullet(Vector3 position, float radius)
    {
#if UNITY_EDITOR
        // 绘制子弹主体（圆形表示子弹大小）
        Handles.color = bulletBodyColor;
        Handles.DrawSolidDisc(position, Vector3.forward, radius);

        // 绘制子弹轮廓
        Handles.color = bulletOutlineColor;
        Handles.DrawWireDisc(position, Vector3.forward, radius);

        // 绘制十字线辅助线
        Handles.color = new Color(1, 1, 1, 0.7f);
        Handles.DrawLine(position + Vector3.left * radius * 0.5f, position + Vector3.right * radius * 0.5f);
        Handles.DrawLine(position + Vector3.down * radius * 0.5f, position + Vector3.up * radius * 0.5f);
        // 绘制子弹中心
        Handles.color = Color.red;
        Handles.DrawSolidDisc(position, Vector3.forward, 0.1f);
        Debug.LogError("子弹圆心在" + position + "位置");
#endif
    }

    void DrawRectangleBullet(Vector3 position, float width, float height, Vector3 direction)
    {
#if UNITY_EDITOR
        // 计算矩形的四个角点
        Vector3 right = direction.normalized;
        Vector3 up = new Vector3(-right.y, right.x, 0); // 垂直方向

        Vector3[] corners = new Vector3[4];
        corners[0] = position - right * width * 0.5f - up * height * 0.5f;  // 左下
        corners[1] = position + right * width * 0.5f - up * height * 0.5f;  // 右下
        corners[2] = position + right * width * 0.5f + up * height * 0.5f;  // 右上
        corners[3] = position - right * width * 0.5f + up * height * 0.5f;  // 左上

        Handles.color = bulletBodyColor;
        Handles.DrawSolidRectangleWithOutline(corners,
            new Color(bulletBodyColor.r, bulletBodyColor.g, bulletBodyColor.b, 0.5f),
            bulletOutlineColor);

        Vector3 forwardPoint = position + right * width * 0.4f;
        Handles.color = bulletOutlineColor;
        Handles.DrawLine(position, forwardPoint);
#endif
    }

    void DrawSectorBullet(Vector3 position, float radius, float angle, Vector3 direction)
    {
#if UNITY_EDITOR
        // 绘制扇形子弹
        Handles.color = bulletBodyColor;
        Handles.DrawSolidArc(position, Vector3.forward,
            Quaternion.Euler(0, 0, -angle * 0.5f) * direction, angle, radius);

        // 绘制扇形轮廓
        Handles.color = bulletOutlineColor;
        Handles.DrawWireArc(position, Vector3.forward,
            Quaternion.Euler(0, 0, -angle * 0.5f) * direction, angle, radius);

        // 绘制两条边界线
        Vector3 leftBound = Quaternion.Euler(0, 0, -angle * 0.5f) * direction * radius;
        Vector3 rightBound = Quaternion.Euler(0, 0, angle * 0.5f) * direction * radius;
        Handles.DrawLine(position, position + leftBound);
        Handles.DrawLine(position, position + rightBound);

        // 绘制方向指示器
        Vector3 forwardPoint = position + direction.normalized * radius * 0.7f;
        Handles.DrawLine(position, forwardPoint);
#endif
    }
    void DrawEllipseBullet(Vector3 position, float width, float height, Vector3 direction)
    {
#if UNITY_EDITOR
        // 绘制椭圆形子弹
        Vector3 right = direction.normalized;
        Vector3 up = new Vector3(-right.y, right.x, 0); // 垂直方向

        // 创建椭圆形状的顶点
        int segments = 32; // 椭圆的分段数
        Vector3[] ellipsePoints = new Vector3[segments + 1];

        for (int i = 0; i <= segments; i++)
        {
            float angle = (float)i / segments * Mathf.PI * 2;
            float x = Mathf.Cos(angle) * width * 0.5f;
            float y = Mathf.Sin(angle) * height * 0.5f;

            // 使用方向向量旋转椭圆
            Vector3 point = position + right * x + up * y;
            ellipsePoints[i] = point;
        }

        // 绘制椭圆主体（填充）
        Handles.color = bulletBodyColor;
        for (int i = 0; i < segments; i++)
        {
            // 绘制从中心到边缘的三角形来填充椭圆
            Handles.DrawAAConvexPolygon(position, ellipsePoints[i], ellipsePoints[i + 1]);
        }

        // 绘制椭圆轮廓
        Handles.color = bulletOutlineColor;
        for (int i = 0; i < segments; i++)
        {
            Handles.DrawLine(ellipsePoints[i], ellipsePoints[i + 1]);
        }

        // 绘制方向指示器
        Vector3 forwardPoint = position + direction.normalized * width * 0.7f;
        Handles.DrawLine(position, forwardPoint);
#endif
    }


    void DrawTrajectory(Vector3 start, Vector3 end)
    {
#if UNITY_EDITOR
        // 绘制主路径线
        Handles.color = pathColor;
        Handles.DrawLine(start, end);

        // 获取子弹尺寸用于绘制路径宽度
        float maxWidth = gizmoSize;
        float maxHeight = gizmoSize;

        switch (bulletConfig._RangeType)
        {
            case AutoConfig.AutoEnum.eRangeType.Circle:
                maxWidth = maxHeight = bulletConfig._Radius > 0 ? bulletConfig._Radius : gizmoSize;
                break;

            case AutoConfig.AutoEnum.eRangeType.Cube:
                if (bulletConfig._RectangleSize != null && bulletConfig._RectangleSize.Length >= 2)
                {
                    maxWidth = bulletConfig._RectangleSize[1];
                    maxHeight = bulletConfig._RectangleSize[0];
                }
                break;

            case AutoConfig.AutoEnum.eRangeType.Sector:
                maxWidth = maxHeight = bulletConfig._Radius > 0 ? bulletConfig._Radius : gizmoSize;
                break;
        }

        // 绘制路径宽度表示（子弹大小的影响范围）
        Vector3 direction = (end - start).normalized;
        Vector3 perpendicular = new Vector3(-direction.y, direction.x, 0).normalized;

        // 绘制路径两侧的边界线
        Vector3 side1Start = start + perpendicular * maxWidth * 0.5f;
        Vector3 side1End = end + perpendicular * maxWidth * 0.5f;
        Vector3 side2Start = start - perpendicular * maxWidth * 0.5f;
        Vector3 side2End = end - perpendicular * maxWidth * 0.5f;

        Handles.color = new Color(pathColor.r, pathColor.g, pathColor.b, pathColor.a * 0.5f);
        Handles.DrawLine(side1Start, side1End);
        Handles.DrawLine(side2Start, side2End);

        // 绘制半透明区域表示影响范围
        Vector3[] quad = { side1Start, side1End, side2End, side2Start };
        Handles.color = new Color(pathColor.r, pathColor.g, pathColor.b, pathColor.a * 0.2f);
        Handles.DrawSolidRectangleWithOutline(quad,
            new Color(pathColor.r, pathColor.g, pathColor.b, 0.1f),
            new Color(pathColor.r, pathColor.g, pathColor.b, 0.3f));

        // 绘制箭头表示方向
        Vector3 arrowPosition = Vector3.Lerp(start, end, 0.8f);
        DrawArrow(arrowPosition, end, pathColor);
#endif
    }

    void DrawArrow(Vector3 from, Vector3 to, Color color)
    {
#if UNITY_EDITOR
        Handles.color = color;
        Handles.DrawLine(from, to);

        Vector3 direction = (to - from).normalized;
        Vector3 perpendicular = new Vector3(-direction.y, direction.x, 0).normalized;

        // 绘制箭头头部
        float headSize = 0.3f;
        Vector3 arrowHead1 = to - direction * headSize + perpendicular * headSize * 0.5f;
        Vector3 arrowHead2 = to - direction * headSize - perpendicular * headSize * 0.5f;

        Handles.DrawLine(to, arrowHead1);
        Handles.DrawLine(to, arrowHead2);
#endif
    }
}