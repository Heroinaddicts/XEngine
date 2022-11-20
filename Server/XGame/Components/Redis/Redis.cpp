#include "Redis.h"

#ifdef WIN32
#pragma  comment(lib, "ws2_32.lib")
#endif //WIN32

#include "RedisPipe.h"

iEngine* g_Engine = nullptr;
Redis* g_Redis = nullptr;
std::set<RedisPipe*> g_PipeSet;

bool Redis::Initialize(iEngine* const engine) {
    g_Redis = this;
    g_Engine = engine;

    Start(1);
    return true;
}

bool Redis::Launch(iEngine* const engine) {
    return true;
}

bool Redis::Destroy(iEngine* const engine) {
    for (auto itor = g_PipeSet.begin(); itor != g_PipeSet.end(); itor++) {
        (*itor)->Close();
        xdel(*itor);
    }

    g_PipeSet.clear();
    return true;
}

void Redis::Update(iEngine* const engine) {
    RedisPipe* pipe = nullptr;
    while (_PipeConnectResponseQueue.Pull(pipe)) {
        if (pipe->IsConnected()) {
            iRedisPipe* temp = pipe;
            SafeMemory::Memcpy((void*)&(pipe->_Session->_Pipe), sizeof(pipe->_Session->_Pipe), &temp, sizeof(temp));
            pipe->_Session->OnConnect(true);
            pipe->Start();
            g_PipeSet.insert(pipe);
        }
        else {
            pipe->_Session->OnConnect(false);
        }
    }

    for (auto itor = g_PipeSet.begin(); itor != g_PipeSet.end();) {
        if ((*itor)->GetCode() == RedisPipe::eErrorCode::SUCCESS) {
            (*itor)->Update();
            itor++;
        }
        else {
            g_PipeSet.erase(itor++);
        }
    }
}

void Redis::ConnectRedis(iRedisSession* session, const std::string& host, const int port, const std::string& password) {
    RedisPipe* pipe = xnew RedisPipe(session, host, port, password);
    if (false == _PipeConnectRequestQueue.Push(pipe)) {
        session->OnConnect(false);
        xdel pipe;
    }
}

void Redis::Run(void* constext) {
    while (!IsClose()) {
        RedisPipe* pipe = nullptr;
        if (_PipeConnectRequestQueue.Pull(pipe)) {
            pipe->Connect();
            _PipeConnectResponseQueue.Push(pipe);
        }
        else {
            SafeSystem::Time::MillisecondSleep(2);
        }
    }
}

