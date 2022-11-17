using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TLib
{
    public static class SafeMemory
    {
        public static bool Copy(byte[] src, in int src_offset, in int src_size, in byte[] target, in int target_offset, in int length)
        {
            if (src_size < length)
            {
                return false;
            }

            Buffer.BlockCopy(target, target_offset, src, src_offset, length);
            return true;
        }

        public static byte[] Int32ToBytes(in int value)
        {
            byte[] src = new byte[4];
            src[3] = (byte)((value >> 24) & 0xFF);
            src[2] = (byte)((value >> 16) & 0xFF);
            src[1] = (byte)((value >> 8) & 0xFF);//高8位
            src[0] = (byte)(value & 0xFF);//低位
            return src;
        }

        public static int BytesToInt32(in byte[] src, in int offset)
        {
            int value;
            value = (int)((src[offset] & 0xFF)
                    | ((src[offset + 1] & 0xFF) << 8)
                    | ((src[offset + 2] & 0xFF) << 16)
                    | ((src[offset + 3] & 0xFF) << 24));

            return value;
        }

        public static byte[] Int16ToBytes(in Int16 value)
        {
            byte[] src = new byte[2];
            src[1] = (byte)((value >> 8) & 0xFF);//高8位
            src[0] = (byte)(value & 0xFF);//低位
            return src;
        }

        public static Int16 BytesToInt16(in byte[] src, in int offset)
        {
            Int16 value;
            value = (Int16)((src[offset] & 0xFF)
                    | ((src[offset + 1] & 0xFF) << 8));

            return value;
        }

        public static byte[] StringToBytes(in string str)
        {
            return Encoding.Default.GetBytes(str);
        }

        public static string BytesToString(in byte[] bts, in int offset, in int length)
        {
            return Encoding.Default.GetString(bts, offset, length);
        }
    }
}
