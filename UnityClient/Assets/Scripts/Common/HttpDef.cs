using System;

namespace HttpDef
{

    public enum eRequestID
    {
        VersionXML = 0,
        AccountLogin = 1
    }

    public enum eRequestType
    {
        Get,
        Post
    }

    public class eError
    {
        public const int unknown = 0;
        public const int success = 1;
    }

    public static class Url
    {
        public static class Account
        {
            public static class Login
            {
                public const eRequestID id = eRequestID.AccountLogin;
                public const string url = "account/login";
                public struct Parameters
                {
                    readonly public string id;
                    readonly public Define.ePlatform platform;
                    public Parameters(in string iid, in Define.ePlatform iplatform)
                    {
                        id = iid;
                        platform = iplatform;
                    }
                }
            }
        }
    }
}