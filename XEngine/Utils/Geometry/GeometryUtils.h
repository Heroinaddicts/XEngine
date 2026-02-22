#ifndef __GeometryUtils_h__
#define __GeometryUtils_h__

#include "Geometry.h"
#include "Vector2.h"

namespace XEngine {
    class GeometryUtils
    {
    public:
        __forceinline static float ToRadians(float degrees)
        {
            return degrees * XPI / 180.0f;
        }

        __forceinline static float ToDegrees(float radians)
        {
            return radians * 180.0f / XPI;
        }

        __forceinline static float NormalizeAngle(float angle)
        {
            angle = std::fmod(angle, 360.0f);
            return angle < 0.0f ? angle + 360.0f : angle;
        }

        //检查圆与线段的相交
        __forceinline static bool CheckCircleIntersectLine(const Vector2& circleCenter, float radius, const Vector2& lineStart, const Vector2& lineEnd)
        {
            Vector2 lineDir = lineEnd - lineStart;
            float lineLength = lineDir.Length();

            if (lineLength < Epsilon) {
                float dist = (circleCenter - lineStart).Length();
                return dist <= radius + Epsilon;
            }

            Vector2 lineDirNormalized = lineDir.Normalized();
            Vector2 startToCenter = circleCenter - lineStart;

            float projection = startToCenter.Dot(lineDirNormalized);

            Vector2 closestPoint;
            if (projection <= 0) {
                closestPoint = lineStart;
            }
            else if (projection >= lineLength) {
                closestPoint = lineEnd;
            }
            else {
                closestPoint = lineStart + lineDirNormalized * projection;
            }

            float distance = (circleCenter - closestPoint).Length();
            return distance <= radius + Epsilon;
        }

        // 判断角度是否在两个角度之间（考虑跨越0度的情况）
        __forceinline static bool isAngleBetween(float angle, float start, float end)
        {
            angle = NormalizeAngle(angle);
            start = NormalizeAngle(start);
            end = NormalizeAngle(end);

            if (end < start) {
                return (angle >= start) || (angle <= end);
            }

            return (angle >= start) && (angle <= end);
        }

        __forceinline static bool IsPointInSetor(const Vector2& point, const Vector2& ceneter, const float radius, const float startAngle, const float endAngle)
        {
            Vector2 pointDir(point.x - ceneter.x, point.y - ceneter.y);
            float distance = pointDir.Length();

            if (distance > radius + Epsilon)
            {
                return false;
            }

            if (distance < Epsilon)
            {
                return true;
            }

            float cosStart = cosf(startAngle);
            float sinStart = sinf(startAngle);

            float cosEnd = cosf(endAngle);
            float sinEnd = sinf(endAngle);

            float crossStart = sinStart * pointDir.y - cosStart * pointDir.x;
            float crossEnd = sinEnd * pointDir.y - cosEnd * pointDir.x;

            return crossStart >= -Epsilon && crossEnd <= Epsilon;
        }

        __forceinline static bool IsPointInRectangle(const Vector2& point, const Vector2& rectCenter, float width, float height, float rotation)
        {
            Vector2 localPoint = point - rectCenter;

            float cosAngle = cosf(-rotation);
            float sinAngle = sinf(-rotation);

            float rotatedX = localPoint.x * cosAngle - localPoint.y * sinAngle;
            float rotatedY = localPoint.x * sinAngle + localPoint.y * sinAngle;

            return rotatedX >= -width * 0.5f && rotatedX <= width * 0.5f && rotatedY >= -height * 0.5f && rotatedY <= height * 0.5f;
        }

        __forceinline static float CalculateAngleBetweenPoints(const Vector2& startPos, const Vector2& targetPos)
        {
            float dx = targetPos.x - startPos.x;
            float dy = targetPos.y - startPos.y;
            float angleRad = std::atan2(dy, dx);
            float angleDeg = angleRad * 180.0 / XPI;
            if (angleDeg < 0) {
                angleDeg += 360.0;
            }
            return angleDeg;
        }

        __forceinline static bool IsCircleIntersectRectangle(
            const Vector2& circleCenter, float radius,
            const Vector2& rectCenter, float width, float height, float rotation)
        {
            // 1. 将圆心转换到以矩形中心为原点的全局坐标系
            Vector2 localCircle = circleCenter - rectCenter;

            // 2. 计算旋转角度的余弦/正弦（rotation必须是弧度）
            float cosAngle = cosf(rotation);
            float sinAngle = sinf(rotation);

            // 3. 将圆心顺时针旋转rotation角度，转换到矩形局部坐标系（无旋转）
            // 目的：把旋转后的矩形"掰直"成轴对齐矩形，简化碰撞检测
            float rotatedX = localCircle.x * cosAngle + localCircle.y * sinAngle;
            float rotatedY = -localCircle.x * sinAngle + localCircle.y * cosAngle;

            // 4. 计算矩形的半宽、半高（轴对齐）
            float halfWidth = width * 0.5f;
            float halfHeight = height * 0.5f;

            // 5. 找到矩形内离旋转后圆心最近的点（轴对齐矩形的最近点）
            float closestX = fmaxf(-halfWidth, fminf(rotatedX, halfWidth));
            float closestY = fmaxf(-halfHeight, fminf(rotatedY, halfHeight));

            // 6. 计算圆心到最近点的距离平方（避免开根号，提升性能）
            float distanceX = rotatedX - closestX;
            float distanceY = rotatedY - closestY;
            float distanceSquared = distanceX * distanceX + distanceY * distanceY;

            // 7. 距离平方小于等于半径平方则相交（包含内切/内含的情况）
            return distanceSquared <= radius * radius;
        }


        //判断圆形是否与扇形相交
        __forceinline static bool IsCircleIntersectSetor(const Vector2& circleCenter, float circleRadius, const Vector2& sectorCenter, float sectorRadius, float startAngle, float endAngle)
        {
            Vector2 centerDir = circleCenter - sectorCenter;
            float centerDistance = centerDir.Length();

            if (centerDistance <= circleRadius + Epsilon)
            {
                return IsPointInSetor(circleCenter, sectorCenter, sectorRadius, startAngle, endAngle);
            }

            if (centerDistance > sectorRadius + circleRadius + Epsilon)
            {
                return false;
            }

            float startRad = ToRadians(startAngle);
            float endRad = ToRadians(endAngle);

            Vector2 edgeStartDir(cosf(startRad), sinf(startRad));
            Vector2 edgeEndDir(cosf(endRad), sinf(endRad));

            bool intersectEdge1 = CheckCircleIntersectLine(circleCenter, circleRadius,
                sectorCenter, sectorCenter + edgeStartDir * sectorRadius);

            bool intersectEdge2 = CheckCircleIntersectLine(circleCenter, circleRadius,
                sectorCenter, sectorCenter + edgeEndDir * sectorRadius);

            if (intersectEdge1 || intersectEdge2)
            {
                return true;
            }

            float circleAngleRad = atan2f(centerDir.y, centerDir.x);
            float circleAngleDeg = ToDegrees(circleAngleRad);
            circleAngleDeg = NormalizeAngle(circleAngleDeg);

            bool isInAngleRange = isAngleBetween(circleAngleDeg, startAngle, endAngle);

            if (isInAngleRange) {
                if (centerDistance <= sectorRadius + circleRadius + Epsilon) {
                    return true;
                }
            }
            else {
                Vector2 sectorStartPoint = sectorCenter + edgeStartDir * sectorRadius;
                Vector2 sectorEndPoint = sectorCenter + edgeEndDir * sectorRadius;

                float distToStart = (circleCenter - sectorStartPoint).Length();
                float distToEnd = (circleCenter - sectorEndPoint).Length();

                if (distToStart <= circleRadius + Epsilon || distToEnd <= circleRadius + Epsilon) {
                    return true;
                }

                Vector2 nearestPointOnArc;
                if (isAngleBetween(circleAngleDeg, startAngle, endAngle)) {
                    // 圆心的投影点在角度范围内，最近点在弧上
                    nearestPointOnArc = sectorCenter + centerDir.Normalized() * sectorRadius;
                }
                else {
                    // 最近点是扇形的一个端点
                    float distToStartSq = (circleCenter - sectorStartPoint).LengthSquared();
                    float distToEndSq = (circleCenter - sectorEndPoint).LengthSquared();

                    nearestPointOnArc = (distToStartSq < distToEndSq) ? sectorStartPoint : sectorEndPoint;
                }

                float distToArcSq = (circleCenter - nearestPointOnArc).LengthSquared();
                if (distToArcSq <= circleRadius * circleRadius + Epsilon) {
                    return true;
                }
            }

            return false;
        }

        //判断圆形是否与椭圆相交
        __forceinline static bool IsCircleIntersectEllipse(const Vector2& circleCenter, float circleRadius, const Vector2& ellipseCenter, float ellipseWidth, float ellipseHeight, float ellipseRotation)
        {
            float a = ellipseWidth * 0.5f;
            float b = ellipseHeight * 0.5f;

            if (a < b)
            {
                std::swap(a, b);
            }

            Vector2 localCircle = circleCenter - ellipseCenter;

            float cosAngle = cosf(-ellipseRotation);
            float sinAngle = sinf(-ellipseRotation);

            float rotatedX = localCircle.x * cosAngle - localCircle.y * sinAngle;
            float rotatedY = localCircle.x * sinAngle + localCircle.y * cosAngle;

            // 检查圆形是否完全包含椭圆中心点
            if (rotatedX * rotatedX + rotatedY * rotatedY <= circleRadius * circleRadius + Epsilon) {
                return true;
            }

            float scaleX = 1.0f / a;
            float scaleY = 1.0f / b;

            float scaledCircleX = rotatedX * scaleX;
            float scaledCircleY = rotatedY * scaleY;

            float minScale = fminf(scaleX, scaleY);
            float maxScale = fmaxf(scaleX, scaleY);

            Vector2 scaledCircle(scaledCircleX, scaledCircleY);
            float scaledCircleLen = scaledCircle.Length();

            if (scaledCircleLen < Epsilon) {
                float scaledRadius = circleRadius * minScale;
                return 1.0f - scaledRadius <= Epsilon;
            }

            Vector2 dir = scaledCircle.Normalized();

            float cosTheta = dir.x;
            float sinTheta = dir.y;

            float a_prime = circleRadius * scaleX;
            float b_prime = circleRadius * scaleY;

            float ellipseRadiusInDir = 1.0f / sqrtf(cosTheta * cosTheta + sinTheta * sinTheta + Epsilon);

            float circleEllipseRadiusInDir = 1.0f / sqrtf(cosTheta * cosTheta / (a_prime * a_prime) +
                sinTheta * sinTheta / (b_prime * b_prime) + Epsilon);

            float distance = scaledCircleLen;

            float maxScaledRadius = circleRadius * maxScale;
            if (distance > 1.0f + maxScaledRadius + Epsilon) {
                return false;
            }

            float minScaledRadius = circleRadius * minScale;
            if (distance < 1.0f - minScaledRadius - Epsilon) {
                return true;
            }

            const int numSamples = 16;
            float minDistSq = std::numeric_limits<float>::max();

            for (int i = 0; i < numSamples; ++i) {
                float angle = 2.0f * XPI * i / numSamples;
                float ex = scaledCircleX + circleRadius * scaleX * cosf(angle);
                float ey = scaledCircleY + circleRadius * scaleY * sinf(angle);

                float distSq = ex * ex + ey * ey;
                if (distSq < minDistSq) {
                    minDistSq = distSq;
                }
            }

            return minDistSq <= 1.0f + Epsilon;
        }

        //判断圆形是否与圆形相交
        __forceinline static bool IsCircleIntersectCircle(const Vector2& center1, float radius1, const Vector2& center2, float radius2)
        {
            Vector2 diff = center1 - center2;
            float distanceSquared = diff.x * diff.x + diff.y * diff.y;
            float radiusSum = radius1 + radius2;
            float radiusSumSquared = radiusSum * radiusSum;

            return distanceSquared <= radiusSumSquared + Epsilon;
        }
    };
}

#endif //__GeometryUtils_h__
