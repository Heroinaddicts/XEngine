using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Launcher
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Name;
        public readonly AutoEnum.ePosition _LauncherPositionType;
        public readonly float[] _PositionOffset;
        public readonly bool _RecycleWithDeath;
        public readonly bool _RotateWithHost;
        public readonly Int64 _FireDelay;
        public readonly Int64 _FireCount;
        public readonly Int64 _FireInterval;
        public readonly string _Resource;
        public readonly Int64[] _BulletConfigID;
        public Launcher(Int64 ID, string Name, AutoEnum.ePosition LauncherPositionType, float[] PositionOffset, bool RecycleWithDeath, bool RotateWithHost, Int64 FireDelay, Int64 FireCount, Int64 FireInterval, string Resource, Int64[] BulletConfigID)
        {
            _ID = ID;
            _Name = Name;
            _LauncherPositionType = LauncherPositionType;
            _PositionOffset = PositionOffset;
            _RecycleWithDeath = RecycleWithDeath;
            _RotateWithHost = RotateWithHost;
            _FireDelay = FireDelay;
            _FireCount = FireCount;
            _FireInterval = FireInterval;
            _Resource = Resource;
            _BulletConfigID = BulletConfigID;
        }
        public readonly static ReadOnlyDictionary<Int64, Launcher> Datas = new ReadOnlyDictionary<Int64, Launcher>(
            new Dictionary<Int64, Launcher>() {
                {1, new Launcher(1, "弓箭1", AutoEnum.ePosition.TraceHost, new float[] {0f,1f}, true, true, 200, 1, 1000, "xxxx", new Int64[] {11001})},
                {2, new Launcher(2, "手雷", AutoEnum.ePosition.TraceHost, new float[] {0f,1f}, true, true, 200, 1, 1000, "xxxx", new Int64[] {11002})},
                {3, new Launcher(3, "跟踪弹", AutoEnum.ePosition.TraceHost, new float[] {0f,1f}, true, true, 200, 1, 1000, "xxxx", new Int64[] {11003})},
                {4, new Launcher(4, "测试小哥踢腿普攻", AutoEnum.ePosition.TraceHost, new float[] {0.35f,0.45f}, true, true, 150, 1, 1100, "xxxx", new Int64[] {11004,11031})},
                {5, new Launcher(5, "测试小哥声波攻击", AutoEnum.ePosition.TraceHost, new float[] {0.85f,0.15f}, true, true, 200, 1, 1000, "xxxx", new Int64[] {11002})},
                {6, new Launcher(6, "测试小哥血阵", AutoEnum.ePosition.WorldPosition, new float[] {0f,0f}, true, true, 335, 1, 1000, "xxxx", new Int64[] {11001})},
                {7, new Launcher(7, "测试小哥血滴", AutoEnum.ePosition.TraceHost, new float[] {-1.5f,0.3f}, true, true, 20, 1, 1000, "xxxx", new Int64[] {11005})},
                {8, new Launcher(8, "测试小哥普攻1", AutoEnum.ePosition.TraceHost, new float[] {2.85f,1.75f}, true, true, 20, 1, 600, "xxxx", new Int64[] {11006})},
                {9, new Launcher(9, "测试小哥普攻2", AutoEnum.ePosition.TraceHost, new float[] {2.85f,1.75f}, true, true, 20, 1, 1000, "xxxx", new Int64[] {11007})},
                {10, new Launcher(10, "测试小哥普攻3", AutoEnum.ePosition.TraceHost, new float[] {2.85f,1.75f}, true, true, 300, 1, 1000, "xxxx", new Int64[] {11008})},
                {11, new Launcher(11, "地刺", AutoEnum.ePosition.WorldPosition, new float[] {0f,0f}, true, true, 10, 1, 1000, "xxxx", new Int64[] {11009})},
                {12, new Launcher(12, "九头蛇柏扫人", AutoEnum.ePosition.TraceHost, new float[] {0f,0f}, true, true, 10, 1, 1000, "xxxx", new Int64[] {11010})},
                {13, new Launcher(13, "九头蛇柏扫人", AutoEnum.ePosition.TraceHost, new float[] {0f,0f}, true, true, 10, 1, 1000, "xxxx", new Int64[] {11011})},
                {14, new Launcher(14, "单纯抬头", AutoEnum.ePosition.TraceHost, new float[] {0f,0f}, true, true, 10, 1, 1000, "xxxx", new Int64[] {11012})},
                {15, new Launcher(15, "蛇头打人", AutoEnum.ePosition.WorldPosition, new float[] {0f,1f}, true, true, 10, 1, 1000, "xxxx", new Int64[] {11014})},
                {16, new Launcher(16, "藤曼抓人", AutoEnum.ePosition.WorldPosition, new float[] {0f,0f}, true, true, 10, 1, 1000, "xxxx", new Int64[] {11013})},
                {17, new Launcher(17, "藤曼下垂", AutoEnum.ePosition.WorldPosition, new float[] {0f,0f}, true, true, 10, 1, 1000, "xxxx", new Int64[] {11015})},
                {18, new Launcher(18, "九头蛇柏扫A", AutoEnum.ePosition.TraceHost, new float[] {-4.5f,0f}, true, true, 1050, 1, 1000, "xxxx", new Int64[] {11016})},
                {19, new Launcher(19, "九头蛇柏扫B", AutoEnum.ePosition.TraceHost, new float[] {-4.5f,-2.5f}, true, true, 1180, 1, 1000, "xxxx", new Int64[] {11010})},
                {20, new Launcher(20, "九头蛇柏扫人C", AutoEnum.ePosition.TraceHost, new float[] {4.5f,0f}, true, true, 1050, 1, 1000, "xxxx", new Int64[] {11017})},
                {21, new Launcher(21, "九头蛇柏扫人D", AutoEnum.ePosition.TraceHost, new float[] {4.5f,-2.5f}, true, true, 1050, 1, 1000, "xxxx", new Int64[] {11011})},
                {22, new Launcher(22, "尸鳖普攻", AutoEnum.ePosition.TraceHost, new float[] {0.35f,0.25f}, true, true, 225, 1, 600, "xxxx", new Int64[] {11018})},
                {23, new Launcher(23, "血尸普攻", AutoEnum.ePosition.TraceHost, new float[] {1.35f,0.35f}, true, true, 1000, 1, 1000, "xxxx", new Int64[] {11019})},
                {24, new Launcher(24, "潘子普攻", AutoEnum.ePosition.TraceHost, new float[] {0.75f,2.56f}, true, true, 600, 1, 500, "xxxx", new Int64[] {11020})},
                {25, new Launcher(25, "潘子剑雨", AutoEnum.ePosition.WorldPosition, new float[] {0f,0f}, true, true, 1280, 1, 1000, "xxxx", new Int64[] {11021})},
                {26, new Launcher(26, "爆裂箭", AutoEnum.ePosition.TraceHost, new float[] {1.05f,2.25f}, true, true, 950, 10, 50, "xxxx", new Int64[] {11022,11027,11037})},
                {27, new Launcher(27, "加buff潘子", AutoEnum.ePosition.TraceHost, new float[] {0f,0f}, true, true, 10, 1, 100, "xxxx", new Int64[] {11023})},
                {28, new Launcher(28, "潘子后撤普攻", AutoEnum.ePosition.TraceHost, new float[] {0.4f,1f}, true, true, 450, 3, 50, "xxxx", new Int64[] {11020})},
                {29, new Launcher(29, "爆裂箭", AutoEnum.ePosition.TraceHost, new float[] {0f,2.25f}, true, true, 400, 3, 150, "xxxx", new Int64[] {11024,11025,11026,11028,11029,11030,11042,11043,11044})},
                {30, new Launcher(30, "实际小哥声波攻击", AutoEnum.ePosition.TraceHost, new float[] {0.85f,0.15f}, true, true, 200, 3, 300, "xxxx", new Int64[] {11032})},
                {31, new Launcher(31, "爆裂箭2", AutoEnum.ePosition.TraceHost, new float[] {1.05f,2.25f}, true, true, 1000, 2, 300, "xxxx", new Int64[] {11033,11038})},
                {32, new Launcher(32, "爆裂箭3", AutoEnum.ePosition.TraceHost, new float[] {1.05f,2.25f}, true, true, 1050, 2, 300, "xxxx", new Int64[] {11034,11039})},
                {33, new Launcher(33, "爆裂箭4", AutoEnum.ePosition.TraceHost, new float[] {1.05f,2.25f}, true, true, 1100, 2, 300, "xxxx", new Int64[] {11035,11040})},
                {34, new Launcher(34, "爆裂箭5", AutoEnum.ePosition.TraceHost, new float[] {1.05f,2.25f}, true, true, 1150, 2, 300, "xxxx", new Int64[] {11036,11041})},
            }
        );
        public static Launcher Get(Int64 id) {
            Launcher config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
