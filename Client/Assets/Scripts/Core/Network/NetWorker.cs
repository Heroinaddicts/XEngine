using System.Collections.Generic;
using TCore.Api;

namespace TNet
{
    public static class NetWorker
    {
        public static bool _pause = false;

        static private Queue<TNetEvent> _queue = new Queue<TNetEvent>();

        public static void PushEvent(in eTNetEventType type, in eTNetErrorCode code, iPipe pipe)
        {
            TNetEvent ev = new TNetEvent(type, code, pipe);
            lock (_queue)
            {
                _queue.Enqueue(ev);
            }
        }

        public static TNetEvent PullEvent()
        {
            TNetEvent ev = null;
            lock (_queue)
            {
                if (_queue.Count > 0)
                {
                    ev = _queue.Dequeue();
                }
            }

            return ev;
        }

        public static void LaunchTcpClient(iTcpSession session, in string host, in int port)
        {
            TcpPipe pipe = TcpPipe.Create(session, host, port);
        }

        static List<TcpPipe> _pipes_need_to_deal = new List<TcpPipe>();

        public static void Loop()
        {
            while (!_pause && _pipes_need_to_deal.Count >0)
            {
                TcpPipe pipe = _pipes_need_to_deal[0];
                while (pipe.DoRecv() > 0 && !_pause)
                {

                }

                if (!_pause)
                {
                    _pipes_need_to_deal.RemoveAt(0);
                }
            }

            TNetEvent ev = null;
            while (!_pause && (ev = PullEvent()) != null)
            {
                switch (ev._type)
                {
                    case eTNetEventType.TcpConnect:
                        {
                            TcpPipe pipe = (TcpPipe)ev._pipe;

                            if (ev._code != eTNetErrorCode.Success)
                            {
                                //TO DO : 处理连接失败
                                pipe._session.OnConnect(false);
                                pipe.Close();
                                pipe._session = null;
                            }
                            else
                            {
                                pipe._session._pipe = pipe;
                                pipe._session.OnConnect(true);
                                //TO DO : 处理连接成功
                                pipe.AsyncReceive();
                            }
                            break;
                        }
                    case eTNetEventType.TcpRecv:
                        {
                            TcpPipe pipe = (TcpPipe)ev._pipe;
                            if (ev._code != eTNetErrorCode.Success)
                            {
                                //TO DO : 处理连接失败
                                pipe._session.OnDisconnected();
                                pipe.Close();
                            }
                            else
                            {
                                while(pipe.DoRecv() > 0 && !_pause)
                                {

                                }

                                if (_pause && pipe.HasMessageToDeal())
                                {
                                    _pipes_need_to_deal.Add(pipe);
                                }
                            }
                            break;
                        }
                    case eTNetEventType.TcpSend:
                        {
                            TcpPipe pipe = (TcpPipe)ev._pipe;
                            if (ev._code != eTNetErrorCode.Success && ev._code != eTNetErrorCode.SendOver)
                            {
                                //TO DO : 处理连接失败
                                pipe._session.OnDisconnected();
                                pipe.Close();
                            }

                            if (ev._code == eTNetErrorCode.SendOver)
                            {
                                pipe.DoSend();
                            }

                            break;
                        }
                    default:
                        break;
                }

                if (_pause)
                {
                    return;
                }
            }
        }
    }
}
