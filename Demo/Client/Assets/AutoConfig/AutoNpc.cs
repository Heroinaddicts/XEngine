using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class Npc
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Name;
        public readonly AutoEnum.eNpcType _NpcType;
        public readonly Int64[] _NpcTalk;
        public readonly string _HeadIcon;
        public readonly string _Prefab;
        public readonly float[] _NpcPosition;
        public readonly Int64[] _NpcPositionChange;
        public readonly Int64 _TaskEvent;
        public readonly Int64 _TransPort;
        public readonly string _BubbleTalk;
        public Npc(Int64 ID, string Name, AutoEnum.eNpcType NpcType, Int64[] NpcTalk, string HeadIcon, string Prefab, float[] NpcPosition, Int64[] NpcPositionChange, Int64 TaskEvent, Int64 TransPort, string BubbleTalk)
        {
            _ID = ID;
            _Name = Name;
            _NpcType = NpcType;
            _NpcTalk = NpcTalk;
            _HeadIcon = HeadIcon;
            _Prefab = Prefab;
            _NpcPosition = NpcPosition;
            _NpcPositionChange = NpcPositionChange;
            _TaskEvent = TaskEvent;
            _TransPort = TransPort;
            _BubbleTalk = BubbleTalk;
        }
        public readonly static ReadOnlyDictionary<Int64, Npc> Datas = new ReadOnlyDictionary<Int64, Npc>(
            new Dictionary<Int64, Npc>() {
                {1001, new Npc(1001, "王胖子", AutoEnum.eNpcType.Normal, new Int64[] {11002}, "xx", "Spine/Character/Prefabs/TestCharacter", new float[] {-12f,15.2f,0f}, new Int64[] {10001,10002}, -1, -1, "xxx")},
                {1002, new Npc(1002, "王涛", AutoEnum.eNpcType.Normal, new Int64[] {11001}, "xx", "Spine/Character/Prefabs/TestCharacter", new float[] {-12f,53.71f,1f}, new Int64[] {10001,10002}, 31001, -1, "xxx")},
                {1003, new Npc(1003, "孙悟空1", AutoEnum.eNpcType.Normal, new Int64[] {11001}, "xx", "Spine/Character/Prefabs/TestCharacter", new float[] {29.37f,25.29f,1f}, new Int64[] {10001,10002}, 31002, 10001, "xxx")},
                {1004, new Npc(1004, "王胖子1", AutoEnum.eNpcType.Normal, new Int64[] {11001}, "xx", "Spine/Character/Prefabs/TestCharacter", new float[] {-107.1f,37.6f,0f}, new Int64[] {10001,10002}, -1, -1, "xxx")},
                {1005, new Npc(1005, "王涛1", AutoEnum.eNpcType.Normal, new Int64[] {11001}, "xx", "Spine/Character/Prefabs/TestCharacter", new float[] {-181.71f,44.8f,1f}, new Int64[] {10001,10002}, 31001, -1, "xxx")},
                {1006, new Npc(1006, "孙悟空2", AutoEnum.eNpcType.Normal, new Int64[] {11001}, "xx", "Spine/Character/Prefabs/TestCharacter", new float[] {-131.2f,52.5f,1f}, new Int64[] {10001,10002}, 31002, 10001, "xxx")},
                {1007, new Npc(1007, "王胖子3", AutoEnum.eNpcType.Normal, new Int64[] {11001}, "xx", "Spine/Character/Prefabs/TestCharacter", new float[] {-109.5f,73.2f,0f}, new Int64[] {10001,10002}, -1, -1, "xxx")},
                {1008, new Npc(1008, "王涛4", AutoEnum.eNpcType.Normal, new Int64[] {11001}, "xx", "Spine/Character/Prefabs/TestCharacter", new float[] {-65.01f,79.25f,1f}, new Int64[] {10001,10002}, 31001, -1, "xxx")},
                {1009, new Npc(1009, "孙悟空5", AutoEnum.eNpcType.Normal, new Int64[] {11001}, "xx", "Spine/Character/Prefabs/TestCharacter", new float[] {-58.2f,109f,1f}, new Int64[] {10001,10002}, 31002, 10001, "xxx")},
                {1010, new Npc(1010, "孙悟空6", AutoEnum.eNpcType.Normal, new Int64[] {11001}, "xx", "Spine/Character/Prefabs/TestCharacter", new float[] {-24.3f,92.91f,1f}, new Int64[] {10001,10002}, 31002, 10001, "xxx")},
            }
        );
        public static Npc Get(Int64 id) {
            Npc config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
