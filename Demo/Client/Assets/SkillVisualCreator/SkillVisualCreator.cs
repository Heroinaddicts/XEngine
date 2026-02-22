using UnityEngine;

namespace SkillVisual
{
    class SkillVisualCreator : ISkillVisualCreator
    {
        public ISkillVisual CreateCircleVisual(in float radius)
        {
            GameObject circleGO = new GameObject("CircleVisual");
            CircleVisual circleVisual = circleGO.AddComponent<CircleVisual>();

            MeshFilter meshFilter = circleGO.AddComponent<MeshFilter>();
            MeshRenderer meshRenderer = circleGO.AddComponent<MeshRenderer>();
            Mesh mesh = new Mesh();

            int segments = Mathf.Max(10, Mathf.CeilToInt(radius * 2f)); //最小为10段
            segments = Mathf.Min(segments, 64);

            Vector3[] vertices = new Vector3[segments + 1];
            Vector2[] uv = new Vector2[segments + 1];

            vertices[0] = Vector3.zero;//中心点
            uv[0] = new Vector2(0.5f, 0.5f);
            for (int i = 0; i < segments; i++)
            {
                float angle = 2f * Mathf.PI * i / segments;
                float x = Mathf.Cos(angle) * radius;
                float z = Mathf.Sin(angle) * radius;

                vertices[i + 1] = new Vector3(x, 0, z);
                uv[i + 1] = new Vector2((x / radius + 1) * 0.5f, (z / radius + 1) * 0.5f);
            }
            int[] triangles = new int[segments * 3];
            for (int i = 0; i < segments; i++)
            {
                triangles[i * 3] = 0;           // 中心点
                triangles[i * 3 + 1] = i + 1;   // 当前点
                triangles[i * 3 + 2] = (i + 1) % segments + 1; // 下一个点
            }
            mesh.Clear();
            mesh.vertices = vertices;
            mesh.uv = uv;
            mesh.triangles = triangles;
            mesh.RecalculateNormals();
            meshFilter.mesh = mesh;

            return circleVisual;
        }

        public ISkillVisual CreateRectangleVisual(in float width, in float height)
        {
            GameObject rectangleGO = new GameObject("RectangleVisual");
            RectangleVisual rectangleVisual = rectangleGO.AddComponent<RectangleVisual>();

            MeshFilter meshFilter = rectangleGO.AddComponent<MeshFilter>();
            MeshRenderer meshRenderer = rectangleGO.AddComponent<MeshRenderer>();
            Mesh mesh = new Mesh();

            Vector3[] vertices = new Vector3[4];
            Vector2[] uv = new Vector2[4];

            float halfwidth = width * 0.5f;
            float halfheight = height * 0.5f;

            vertices[0] = new Vector3(-halfwidth, 0, -halfheight);//顶点坐标
            vertices[1] = new Vector3(halfwidth, 0, -halfheight);
            vertices[2] = new Vector3(halfwidth, 0, halfheight);
            vertices[3] = new Vector3(-halfwidth, 0, halfheight);

            uv[0] = new Vector2(0, 0);//uv坐标
            uv[1] = new Vector2(1, 0);
            uv[2] = new Vector2(1, 1);
            uv[3] = new Vector2(0, 1);

            int[] triangles = new int[6];//三角形索引
            triangles[0] = 0;//两个三角形拼一个矩形
            triangles[1] = 1;
            triangles[2] = 2;
            triangles[3] = 0;
            triangles[4] = 2;
            triangles[5] = 3;

            mesh.Clear();
            mesh.vertices = vertices;
            mesh.uv = uv;
            mesh.triangles = triangles;
            mesh.RecalculateNormals();

            meshFilter.mesh = mesh;
            return rectangleVisual;
            // GameObject rectangleGO = new GameObject("RectangleVisual");
            // RectangleVisual rectangleVisual = rectangleGO.AddComponent<RectangleVisual>();
            // MeshFilter meshFilter = rectangleGO.AddComponent<MeshFilter>();
            // MeshRenderer meshRenderer = rectangleGO.AddComponent<MeshRenderer>();
            // Mesh mesh = new Mesh();

            // float halfSideLength = width * 0.5f; // 矩形“目标边”的半长（对应世界X轴）
            // float rectDepth = height;            // 矩形的深度（对应世界Z轴）

            // // 顶点：让X轴方向的边中心对齐原点（脚底）
            // Vector3[] vertices = new Vector3[4];
            // vertices[0] = new Vector3(-halfSideLength, 0, 0);  // 边的左端点
            // vertices[1] = new Vector3(halfSideLength, 0, 0);   // 边的右端点（中心是原点）
            // vertices[2] = new Vector3(halfSideLength, 0, rectDepth); // 矩形右上角
            // vertices[3] = new Vector3(-halfSideLength, 0, rectDepth); // 矩形左上角

            // // UV和三角形索引不变
            // Vector2[] uv = new Vector2[4];
            // uv[0] = new Vector2(0, 0);
            // uv[1] = new Vector2(1, 0);
            // uv[2] = new Vector2(1, 1);
            // uv[3] = new Vector2(0, 1);

            // int[] triangles = new int[6];
            // triangles[0] = 0;
            // triangles[1] = 1;
            // triangles[2] = 2;
            // triangles[3] = 0;
            // triangles[4] = 2;
            // triangles[5] = 3;

            // mesh.Clear();
            // mesh.vertices = vertices;
            // mesh.uv = uv;
            // mesh.triangles = triangles;
            // mesh.RecalculateNormals();
            // meshFilter.mesh = mesh;

            // return rectangleVisual;
        }

        public ISkillVisual CreateSectorVisual(in float radius, in float angle)
        {
            GameObject scetorGO = new GameObject("SectorVisual");
            SectorVisual sectorVisual = scetorGO.AddComponent<SectorVisual>();

            //sectorVisual.Angle = angle;

            MeshFilter meshFilter = scetorGO.AddComponent<MeshFilter>();
            MeshRenderer meshRenderer = scetorGO.AddComponent<MeshRenderer>();
            Mesh mesh = new Mesh();

            float clampedAngle = Mathf.Clamp(angle, 1f, 360f);
            int segments = Mathf.Max(4, Mathf.CeilToInt(clampedAngle / 4f));//每4度一个分段最少四个确定弧线

            Vector3[] vertices = new Vector3[segments + 2];
            Vector2[] uv = new Vector2[segments + 2];

            vertices[0] = Vector3.zero;
            uv[0] = new Vector2(0.5f, 0.5f);//中心点坐标

            float halfangle = clampedAngle * 0.5f;
            for (int i = 0; i < segments; i++)
            {
                float currentAngle = Mathf.Deg2Rad * (-halfangle + (clampedAngle * i / segments));
                float x = Mathf.Cos(currentAngle) * radius;//圆弧坐标
                float z = Mathf.Sin(currentAngle) * radius;

                vertices[i + 1] = new Vector3(x, 0, z);
                uv[i + 1] = new Vector2((x / radius + 1) * 0.5f, (z / radius + 1) * 0.5f);
            }

            int[] triangles = new int[segments * 3];
            for (int i = 0; i < segments; i++)
            {
                triangles[i * 3] = 0;//中心点
                triangles[i * 3 + 1] = i + 1;//目前的点
                triangles[i * 3 + 2] = i + 2;//下一个点
            }

            mesh.Clear();
            mesh.vertices = vertices;
            mesh.uv = uv;
            mesh.triangles = triangles;
            mesh.RecalculateNormals();

            meshFilter.mesh = mesh;

            return sectorVisual;
        }
    }
}
