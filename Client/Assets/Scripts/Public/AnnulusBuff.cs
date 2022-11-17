using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;

namespace TLib
{
    class AnnulusBuff
    {
        int _size;
        int _in;
        int _out;

        byte[] _data;

        public AnnulusBuff(in int size)
        {
            _in = 0;
            _out = 0;
            _size = size;
            _data = new byte[_size];
        }

        public int GetLength()
        {
            return _in - _out;
        }

        public bool In(in byte[] data, in int offset, in int len)
        {
            try
            {
                if (len == 0)
                {
                    return true;
                }

                if (_size - GetLength() < len)
                {
                    lock(this)
                    {
                        byte[] oldData = _data;

                        while (_size - GetLength() < len)
                        {
                            _size *= 2;
                        }

                        _data = new byte[_size];
                        if (_in - _out > 0)
                        {
                            SafeMemory.Copy(_data, 0, _size, oldData, _out, _in - _out);
                        }

                        _in -= _out;
                        _out = 0;
                    }
                }

                if (_size - _in < len)
                {
                    lock(this)
                    {
                        if (_in == _out)
                        {
                            _in = 0;
                            _out = 0;
                        }
                        else
                        {
                            SafeMemory.Copy(_data, 0, _size, _data, _out, _in - _out);
                            _in -= _out;
                            _out = 0;
                        }
                    }
                }

                SafeMemory.Copy(_data, _in, _size - GetLength(), data, offset, len);
                _in += len;
            }
            catch (Exception ex)
            {
                
            }
            
            return true;
        }

        public bool Out(in int size) {
            lock(this)
            {
                if (_out + size > _in)
                {
                    return false;
                }

                _out += size;
            }
            return true;
        }

        public byte[] GetData(out int offset, out int length) {
            byte[] temp;
            lock(this)
            {
                offset = _out;
                length = GetLength();
                temp = _data;
            }
            return temp;
        }

    }
}
