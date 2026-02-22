using System;
using System.Security.Principal;
using System.Text;

public static class XConvert
{
    public static float AngleToRad(in float angle)
    {
        return angle * (float)Math.PI / 180f;
    }
    public static float RadToAngle(float rad)
    {
        return rad * 180f / (float)Math.PI;
    }

    public static string EncodeToBase64(string input)
    {
        if (string.IsNullOrEmpty(input))
            return string.Empty;
        byte[] bytes = Encoding.UTF8.GetBytes(input);

        return Convert.ToBase64String(bytes);
    }
    public static byte[] UInt16ToBytes(UInt16 value)
    {
        return new byte[]
        {
        (byte)(value & 0xFF),               // 低 8 位（小端第一个字节）
        (byte)((value >> 8) & 0xFF)         // 高 8 位（小端第二个字节）
        };
    }

    public static byte[] UInt32ToBytes(uint value)
    {
        return new byte[]
        {
            (byte)( value        & 0xFF),
            (byte)((value >>  8) & 0xFF),
            (byte)((value >> 16) & 0xFF),
            (byte)((value >> 24) & 0xFF),
        };
    }

    public static void UInt32ToBytes(in UInt32 value, byte[] buff, in int offset)
    {
        try
        {
            buff[offset] = (byte)value;           // 低8位
            buff[offset + 1] = (byte)(value >> 8);    // 次低8位
            buff[offset + 2] = (byte)(value >> 16);   // 次高8位
            buff[offset + 3] = (byte)(value >> 24);       // 高8位
        }
        catch (Exception ex)
        {
            throw new ArgumentOutOfRangeException($"offset={offset}, buff.Length={buff.Length}", ex);
        }
    }
    public static byte[] Int32ToBytes(int value)
    {
        return new byte[]
        {
            (byte)( value        & 0xFF),
            (byte)((value >>  8) & 0xFF),
            (byte)((value >> 16) & 0xFF),
            (byte)((value >> 24) & 0xFF),
        };
    }

    /// <summary>
    /// 从 data[offset] 开始，读取 size（1～4）字节，组合成 UInt32（小端）。
    /// </summary>
    public static UInt32 BytesToUInt32(byte[] data, int offset, int size)
    {
        if (data is null)
            throw new ArgumentNullException(nameof(data));
        if (offset < 0 || size < 1 || size > 4 || offset + size > data.Length)
            throw new ArgumentOutOfRangeException($"offset={offset}, size={size}, data.Length={data.Length}");

        UInt32 value = 0;
        for (int i = 0; i < size; i++)
        {
            value |= (UInt32)data[offset + i] << (8 * i);
        }
        return value;
    }

    public static Int32 BytesToInt32(byte[] data, int offset, int size)
    {
        if (data is null)
            throw new ArgumentNullException(nameof(data));
        if (offset < 0 || size < 1 || size > 4 || offset + size > data.Length)
            throw new ArgumentOutOfRangeException($"offset={offset}, size={size}, data.Length={data.Length}");

        Int32 value = 0;
        for (int i = 0; i < size; i++)
        {
            value |= (Int32)data[offset + i] << (8 * i);
        }
        return value;
    }
    public static UInt16 BytesToUInt16(byte[] data, int offset, int size)
    {
        if (data is null)
            throw new ArgumentNullException(nameof(data));
        // 校验 size 范围（1～2）和偏移量合法性
        if (offset < 0 || size < 1 || size > 2 || offset + size > data.Length)
            throw new ArgumentOutOfRangeException($"offset={offset}, size={size}, data.Length={data.Length}");

        UInt16 value = 0;
        for (int i = 0; i < size; i++)
        {
            // 小端字节序：第 i 字节左移 8*i 位（低位字节存低位数据）
            value |= (UInt16)(data[offset + i] << (8 * i));
        }
        return value;
    }

    public static byte[] StringToBytes(string str)
    {
        // 处理null情况（返回空数组或根据需求抛异常）
        if (str == null)
            return Array.Empty<byte>();

        // 使用UTF-8编码（最常用，支持多语言）
        return Encoding.UTF8.GetBytes(str);
    }
}
