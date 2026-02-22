using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace AutoConfig
{
    public class TalkContent
    {
        public const Int64 INVALID_CONFIG_ID = 0;
        public readonly Int64 _ID;
        public readonly string _Content;
        public readonly string _Title;
        public readonly string _Picture;
        public TalkContent(Int64 ID, string Content, string Title, string Picture)
        {
            _ID = ID;
            _Content = Content;
            _Title = Title;
            _Picture = Picture;
        }
        public readonly static ReadOnlyDictionary<Int64, TalkContent> Datas = new ReadOnlyDictionary<Int64, TalkContent>(
            new Dictionary<Int64, TalkContent>() {
                {41001, new TalkContent(41001, "王涛，你站在这等我给你买橘子", "张茂青", "xxx")},
                {41002, new TalkContent(41002, "好的", "王涛", "xxx")},
            }
        );
        public static TalkContent Get(Int64 id) {
            TalkContent config = null; if (Datas.TryGetValue(id, out config)) return config; return null;
        }
    }
}
