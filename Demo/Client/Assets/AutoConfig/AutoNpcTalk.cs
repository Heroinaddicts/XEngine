using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class NpcTalk
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly Int64[] _TalkContent;
        public readonly Int64 _TaskEvent;
        public NpcTalk(Int64 ID, Int64[] TalkContent, Int64 TaskEvent)
        {
            _ID = ID;
            _TalkContent = TalkContent;
            _TaskEvent = TaskEvent;
        }
        public readonly static ReadOnlyDictionary<Int64, NpcTalk> Datas = new ReadOnlyDictionary<Int64, NpcTalk>(
            new Dictionary<Int64, NpcTalk>() {
                {11001, new NpcTalk(11001, new Int64[] {41002}, 4001)},
                {11002, new NpcTalk(11002, new Int64[] {41001}, 4002)},
            }
        );
        public static NpcTalk Get(Int64 id) {
            NpcTalk config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
