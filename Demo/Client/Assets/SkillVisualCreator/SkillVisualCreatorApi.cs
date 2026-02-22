using System;
using UnityEngine;

namespace SkillVisual
{
    public enum eSkillVisual
    {
        Sector = 0,
        Rectangle = 1,
        Circle = 2
    }

    public abstract class ISkillVisual : MonoBehaviour
    {
        public abstract float Angle { get; set; }
        public abstract eSkillVisual Type { get; }

    }

    public interface ISkillVisualCreator
    {
        ISkillVisual CreateSectorVisual(in float radius, in float angle);
        ISkillVisual CreateRectangleVisual(in float width, in float height);
        ISkillVisual CreateCircleVisual(in float radius);
    }

    public static class Api
    {
        public static ISkillVisual CreateSectorVisual(in float radius, in float angle)//创建扇形
        {
            return s_Creator.CreateSectorVisual(radius, angle);
        }

        public static ISkillVisual CreateRectangleVisual(in float width, in float height)
        {
            return s_Creator.CreateRectangleVisual(width, height);
        }

        public static ISkillVisual CreateCircleVisual(in float radius)
        { 
            return s_Creator.CreateCircleVisual(radius);
        }

        static ISkillVisualCreator s_Creator = new SkillVisualCreator();
    }
}
