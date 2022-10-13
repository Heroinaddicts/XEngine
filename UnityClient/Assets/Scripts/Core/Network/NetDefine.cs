
namespace TNet
{
    public enum eTNetErrorCode
    {
        Unknown = -1,
        Success = 0,
        ConnectFailed = 1,
        RecvError = 2,
        SendError = 3,
        SendOver = 4
    }

    public enum eTNetEventType
    {
        TcpConnect,
        TcpRecv,
        TcpSend
    }

    public class TNetEvent
    {
        public readonly eTNetEventType _type;
        public readonly eTNetErrorCode _code;
        public readonly TCore.Api.iPipe _pipe;

        public TNetEvent(in eTNetEventType type, in eTNetErrorCode code, in TCore.Api.iPipe pipe)
        {
            _type = type;
            _code = code;
            _pipe = pipe;
        }
    }
    public class Define
    {
        public const int KB = 1024;
        public const int MB = 1024 * KB;
        public const int GB = 1024 * MB;
    }
}