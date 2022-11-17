using Google.Protobuf;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ProtoAnalysis
{
    public interface iProto<C>
    {
        void Deserialization(in C c, in byte[] data, in int offset, in int len);
    }
    public class tProto<T, C> : iProto<C> where T : IMessage, new()
    {
        public delegate void call(in C c, in T body);

        readonly call _caller;

        public tProto(in call caller)
        {
            _caller = caller;
        }
        public void Deserialization(in C c, in byte[] data, in int offset, in int len)
        {
            //             try
            //             {

            T t = new T();
            CodedInputStream stream = new CodedInputStream(data, offset, len);
            t.MergeFrom(stream);
            _caller(c, t);
            //             }
            //             catch (Exception ex)
            //             {
            //                 Debug.LogError(ex.ToString());
            //             }
        }
    }
}
