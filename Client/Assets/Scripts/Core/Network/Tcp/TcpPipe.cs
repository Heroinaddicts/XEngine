using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Threading;
using System.Timers;
using TCore.Api;
using TLib;
using UnityEngine;

namespace TNet
{
    class TcpPipe : TCore.Api.iTcpPipe
    {
        public static TcpPipe Create(in TCore.Api.iTcpSession session, in string host, in int port)
        {
            return new TcpPipe(session, host, port);
        }

        public TCore.Api.iTcpSession _session;

        readonly Socket _socket;
        
        private byte[] _recv_buff;
        private bool _isSending;

        private AnnulusBuff _recv_stream;
        private AnnulusBuff _send_stream;

        TcpPipe(in TCore.Api.iTcpSession session, in string host, in int port)
        {
            _session = session;

            _recv_buff = new byte[Define.KB];

            _recv_stream = new AnnulusBuff(20 * Define.MB);
            _send_stream = new AnnulusBuff(20 * Define.MB);
            _isSending = false;

            try
            {
                _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                _socket.SetSocketOption(SocketOptionLevel.Tcp, SocketOptionName.NoDelay, true);
                IPEndPoint address = new IPEndPoint(IPAddress.Parse(host), port);
                _socket.BeginConnect(address, OnConnect, this);
            }
            catch (Exception e)
            {
                Debug.LogError("pipe connect " + host + ":" + port + " error : " + e.Message);
                _session.OnConnect(false);
                _socket.Close();
            }
        }

        public bool HasMessageToDeal()
        {
            return _recv_stream.GetLength() > 0;
        }

        public void Send(in byte[] data, in int offset, in int length, in bool immediately)
        {
            //Debug.Log("Send thread id " + Thread.CurrentThread.ManagedThreadId);

            _send_stream.In(data, offset, length);
            if (!_isSending && immediately)
            {
                int offset2, length2;

                byte[] temp = _send_stream.GetData(out offset2, out length2);
                _isSending = true;
                _socket.BeginSend(temp, offset2, length2, 0, OnSend, this);
            }
        }

        public void Close()
        {
            _socket.Close();
            if (null != _session)
            {
                _session._pipe = null;
                _session = null;
            }
        }

        void OnConnect(IAsyncResult ar)
        {
            //Debug.Log("OnConnect thread id " + Thread.CurrentThread.ManagedThreadId);
            try
            {
                _socket.EndConnect(ar);
                NetWorker.PushEvent(eTNetEventType.TcpConnect, _socket.Connected == true ? eTNetErrorCode.Success : eTNetErrorCode.ConnectFailed, this);
            }
            catch (Exception e)
            {
                NetWorker.PushEvent(eTNetEventType.TcpConnect, eTNetErrorCode.ConnectFailed, this);
            }
        }

        public int DoRecv()
        {
            int offset, length;
            byte[] temp = _recv_stream.GetData(out offset, out length);
            int ret = _session.OnRecv(temp, offset, length);
            _recv_stream.Out(ret);
            return ret;
        }

        public void AsyncReceive()
        {
            _socket.BeginReceive(_recv_buff, 0, _recv_buff.Length, 0, OnReceive, this);
        }

        void OnReceive(IAsyncResult ar)
        {
            //Debug.Log("OnReceive thread id " + Thread.CurrentThread.ManagedThreadId);
            try
            {
                int size = _socket.EndReceive(ar);
                if (size > 0)
                {
                    _recv_stream.In(_recv_buff, 0, size);
                    //_socket.IOControl(SIO_TCP_SET_ACK_FREQUENCY, BitConverter.GetBytes(1), Dummy);
                    _socket.BeginReceive(_recv_buff, 0, _recv_buff.Length, SocketFlags.None, OnReceive, this);
                    NetWorker.PushEvent(eTNetEventType.TcpRecv, eTNetErrorCode.Success, this);
                    return;
                }
            }
            catch (Exception ex)
            {
                Debug.LogError("pipe receive error : " + ex.Message);
            }

            NetWorker.PushEvent(eTNetEventType.TcpRecv, eTNetErrorCode.RecvError, this);
            _socket.Close();
        }
        public void DoSend()
        {
            if (_send_stream.GetLength() > 0)
            {
                int offset, length;
                byte[] temp = _send_stream.GetData(out offset, out length);
                _socket.BeginSend(temp, offset, length, 0, OnSend, this);
            }
            else
            {
                _isSending = false;
            }
        }
        void OnSend(IAsyncResult ar)
        {
            //Debug.Log("OnSend thread id " + Thread.CurrentThread.ManagedThreadId);
            try
            {
                int size = _socket.EndSend(ar);
                if (size > 0)
                {
                    _send_stream.Out(size);

                    if (_send_stream.GetLength() > 0)
                    {
                        int offset, length;
                        byte[] temp = _send_stream.GetData(out offset, out length);
                        _socket.BeginSend(temp, offset, length, 0, OnSend, this);
                    } else
                    {
                        NetWorker.PushEvent(eTNetEventType.TcpSend, eTNetErrorCode.SendOver, this);
                    }
                    return;
                }
            }
            catch ( Exception ex)
            {

            }

            NetWorker.PushEvent(eTNetEventType.TcpSend, eTNetErrorCode.SendError, this);
        }
    }
}

