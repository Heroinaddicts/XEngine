using System.Buffers;
using Google.Protobuf;
public static class ProtobufBytePool
{
    /// <summary>
    /// 序列化并返回一个带长度信息的 PooledBuffer，使用完后必须调用 Dispose() 归还底层数组
    /// </summary>
    public struct PooledBuffer : System.IDisposable
    {
        public byte[] Buffer { get; }
        public int Length { get; }

        public PooledBuffer(byte[] buffer, int length)
        {
            Buffer = buffer;
            Length = length;
        }

        public void Dispose()
        {
            ArrayPool<byte>.Shared.Return(Buffer);
        }
    }

    public static PooledBuffer Serialize<T>(T message) where T : IMessage<T>
    {
        int size = message.CalculateSize();
        byte[] buf = ArrayPool<byte>.Shared.Rent(size);
        var cos = new CodedOutputStream(buf);
        message.WriteTo(cos);
        cos.Flush();
        // 不再调用 CheckNoSpaceLeft()
        return new PooledBuffer(buf, size);
    }
}