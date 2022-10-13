using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using UnityEngine;

namespace Tools
{
    public static class Time
    {
        public static Int64 GetMillisecond()
        {
            var ts = DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return Convert.ToInt64(ts.TotalMilliseconds);
        }
    }
    public static class File
    {
        public static XmlDocument LoadXml(string path, string name)
        {
            string filePath = path + name;
            XmlDocument doc = new XmlDocument();

            using (FileStream fs = new FileStream(filePath, FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.Write))
            {
                StreamReader sw = new StreamReader(fs);
                string str = sw.ReadToEnd();
                doc.LoadXml(str);
            }

            return doc;
        }
    }


    public static class Date
    {
        public static Int64 GetCurrentMillisecond()
        {
            return (DateTime.Now.ToUniversalTime().Ticks - 621355968000000000) / 10000;
        }
    }

    public static class String
    {
        public static string Capitalize(in string str)
        {
            if (string.IsNullOrEmpty(str))
            {
                return str;
            }

            return char.ToUpper(str[0]) + str.Substring(1);
        }

        public static string BytesToString(in byte[] bytes)
        {
            return System.Text.Encoding.Default.GetString(bytes); ;
        }

        public static byte[] StringToBytes(in string str)
        {
            return System.Text.Encoding.Default.GetBytes(str);
        }
    }

    namespace Geometry
    {
        public static class G2D
        {
            public class Size
            {
                public float _width = 0.0f;
                public float _height = 0.0f;

                public Size(in float width = 0.0f, in float height = 0.0f)
                {
                    _width = width;
                    _height = height;
                }

                public static Size operator *(in Size s, in float magnification)
                {
                    return new Size(s._width * magnification, s._height * magnification);
                }
            }
            public class SquareInfo
            {
                public Vector2 _center;
                public Size _size;

                public SquareInfo(in Vector2 center, in Size size)
                {
                    _center = center;
                    _size = size;
                }

                public static SquareInfo operator *(in SquareInfo s, in float magnification)
                {
                    return new SquareInfo(s._center, s._size * magnification);
                }
            }

            public static Vector2 CalcVector2ByAngleAndDistance(float angle, float distance)
            {
                return new Vector2(
//                     TrigonometricCache.GetInstance().CalcCos(angle) * distance,
//                     TrigonometricCache.GetInstance().CalcSin(angle) * distance
                );
            }

            public static bool CheckPointInSquare(in Vector2 point, in SquareInfo square)
            {
                if (point.x > (square._center.x - square._size._width / 2.0f)
                    && point.x < (square._center.x + square._size._width / 2.0f)
                    && point.y > (square._center.y - square._size._height / 2.0f)
                    && point.y < (square._center.y + square._size._height / 2.0f))
                {
                    return true;
                }

                return false;
            }

            public static Vector2 RandomPointInSquare(in SquareInfo square)
            {
                return new Vector2(
                    UnityEngine.Random.Range(square._center.x - square._size._width / 2.0f, square._center.x + square._size._width / 2.0f),
                    UnityEngine.Random.Range(square._center.y - square._size._height / 2.0f, square._center.y + square._size._height / 2.0f)
                );
            }
        }
    }
}
