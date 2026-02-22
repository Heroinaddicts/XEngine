using System;
using System.Net;
using System.Net.Sockets;
// using static PlasticPipe.PlasticProtocol.Client.ConnectionCreator.PlasticProtoSocketConnection;
// using static PlasticPipe.PlasticProtocol.Messages.Serialization.ItemHandlerMessagesSerialization;

namespace XEngine
{
    class Udper : XApi.INetwork.IUdper
    {
        Socket _Socket;
        readonly Network _Network;
        EndPoint _RemoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
        bool _IsClosed = false;
        XApi.INetwork.IUdper.fUdpReceive _UdpReceive;

        public XApi.INetwork.IUdper.fUdpReceive Receive => _UdpReceive;
        public bool IsClosed => _IsClosed;

        public Udper(in Socket sock, in Network network)
        {
            _Socket = sock;
            _Network = network;
        }

        public void SendTo(in string host, in int port, in byte[] data, in int offset, in int length)
        {
            _Socket.BeginSendTo(
                data,
                offset,
                length,
                SocketFlags.None,
                NetDefine.ParseEndPoint(host, port),
                null,
                null
            );
        }

        public void SetUdpReciver(in XApi.INetwork.IUdper.fUdpReceive reciver)
        {
            _UdpReceive = reciver;
        }

        public void AsyncReceive()
        {
            byte[] buffer = new byte[NetDefine.UDP_RECIVE_BUFF_SIZE];
            _Socket.BeginReceiveFrom(
                buffer,
                0,
                buffer.Length,
                SocketFlags.None,
                ref _RemoteEndPoint,
                ReceiveCallback,
                buffer
            );
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            if (_IsClosed)
                return;
            
            try
            {
                byte[] buffer = (byte[])ar.AsyncState;
                int bytesRead = _Socket.EndReceiveFrom(ar, ref _RemoteEndPoint);
                if (bytesRead > 0)
                {
                    _Network.PushEvent(this, buffer, bytesRead, (_RemoteEndPoint as IPEndPoint).Address.ToString(), (_RemoteEndPoint as IPEndPoint).Port);
                }

                AsyncReceive();
            }
            catch (ObjectDisposedException)
            {
                Console.WriteLine("接收已停止");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"接收错误: {ex.Message}");
            }
        }

        public void Close()
        {
            _IsClosed = true;
            _Socket.Close();
        }

        public void Bind(in string ip, in int port)
        {
            if (port < 0 || port > 65535)
                throw new ArgumentOutOfRangeException(nameof(port), "Port number must be in the range 0-65535");

            if (_Socket == null)
                throw new InvalidOperationException("Socket is not initialized");

            if (_Socket.IsBound)
                throw new InvalidOperationException("Socket is already bound");

            try
            {
                _Socket.Bind(new IPEndPoint(string.IsNullOrEmpty(ip) ? IPAddress.Any : IPAddress.Parse(ip), port));
                this.AsyncReceive();
            }
            catch (SocketException ex)
            {
                string errorMsg = $"Binding to {ip}:{port} failed: {ex.Message}";
                XApi.Trace($"{errorMsg}\n{ex.StackTrace}");
                throw new InvalidOperationException(errorMsg, ex);
            }
            catch (FormatException ex)
            {
                string errorMsg = $"Invalid IP address format: {ip}";
                XApi.Trace($"{errorMsg}\n{ex.StackTrace}");
                throw new ArgumentException(errorMsg, nameof(ip), ex);
            }
        }

        public void StartRecive()
        {
            this.AsyncReceive();
        }
    }
}
