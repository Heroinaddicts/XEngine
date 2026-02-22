using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Scene
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly AutoEnum.eSceneType _SceneType;
        public readonly string _Name;
        public readonly float[] _StartPoint;
        public readonly string _Preload;
        public readonly Int64[] _TaskNpc;
        public readonly Int64 _AdimitPeople;
        public readonly Int64 _AdmitGroup;
        public readonly Int64 _Prerequisite;
        public readonly Int64[] _Resource;
        public readonly Int64[] _RefreshRule;
        public readonly Int64 _Sound;
        public readonly Int64[] _TransPort;
        public readonly AutoEnum.eVisualSceneType _VisualSceneType;
        public readonly Int64 _DisappearTime;
        public readonly bool _IsPvP;
        public Scene(Int64 ID, AutoEnum.eSceneType SceneType, string Name, float[] StartPoint, string Preload, Int64[] TaskNpc, Int64 AdimitPeople, Int64 AdmitGroup, Int64 Prerequisite, Int64[] Resource, Int64[] RefreshRule, Int64 Sound, Int64[] TransPort, AutoEnum.eVisualSceneType VisualSceneType, Int64 DisappearTime, bool IsPvP)
        {
            _ID = ID;
            _SceneType = SceneType;
            _Name = Name;
            _StartPoint = StartPoint;
            _Preload = Preload;
            _TaskNpc = TaskNpc;
            _AdimitPeople = AdimitPeople;
            _AdmitGroup = AdmitGroup;
            _Prerequisite = Prerequisite;
            _Resource = Resource;
            _RefreshRule = RefreshRule;
            _Sound = Sound;
            _TransPort = TransPort;
            _VisualSceneType = VisualSceneType;
            _DisappearTime = DisappearTime;
            _IsPvP = IsPvP;
        }
        public readonly static ReadOnlyDictionary<Int64, Scene> Datas = new ReadOnlyDictionary<Int64, Scene>(
            new Dictionary<Int64, Scene>() {
                {20001, new Scene(20001, AutoEnum.eSceneType.Public, "潘家园", new float[] {0f,10f}, "PanJiaYuan", new Int64[] {1001,1002,1003,1004,1005,1006,1007,1008,1009,1010}, 999, 999, 6001, new Int64[] {4003}, new Int64[] {}, 4009, new Int64[] {10001}, AutoEnum.eVisualSceneType.Public, 0, false)},
                {20002, new Scene(20002, AutoEnum.eSceneType.BattleCopy, "测试地图", new float[] {-17.8f,-23.9f,8.43f,67f,40.1f,49.5f,83f,26f,28f,53.3f,46.3f,7.5f,5.2f,-10f}, "TestBattle", new Int64[] {}, 4, 16, 6001, new Int64[] {4003}, new Int64[] {5009,5008,5012,5013,5015,5016,5017,5018,5019,5020,5021,5022,5023,5024,5025,5026,5027,5028,5029,5030,5031}, 4010, new Int64[] {10002}, AutoEnum.eVisualSceneType.Entity, 1500000, true)},
            }
        );
        public static Scene Get(Int64 id) {
            Scene config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
