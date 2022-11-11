#ifndef __iSession_h__
#define __iSession_h__

#include "api.h"
using namespace tcore;
#include <map>
#include "google/protobuf/message.h"
#include "tparser.h"

#pragma pack(push, 1)
struct oProtocol {
    uint32 size;
    uint16 id;
};
#pragma pack(pop)

template<typename session>
class iSession : public api::iTcpSession {
public:
    virtual ~iSession() {}
    iSession() : _parser(f_parser_protobuf) {}

    virtual int onRecv(api::iCore * core, const char * context, const int size) {
        if (size < sizeof(oProtocol)) {
            return 0;
        }

        const oProtocol * header = (const oProtocol *)context;
        if (header->size > size) {
            return 0;
        }

        const char * data = ((const char *)context) + sizeof(oProtocol);

        onMessage(core, header->id, data, header->size - sizeof(oProtocol));
        return header->size;
    }

    template<typename pb>
    void tregister(const uint16 id, void(*caller)(const pb &, api::iCore *, session *, const int64 context), const char * debug) {
        _parser.treg(id, caller, debug);
    }

    virtual void sendproto(const uint16 id, const ::google::protobuf::Message & body) {
        const uint32 size = body.ByteSize();
        char * temp = (char *)alloca(size);
        if (!body.SerializeToArray(temp, size)) {
            tassert(false, "body format error");
            return;
        }

        oProtocol protocol;
        protocol.size = size + sizeof(protocol);
        protocol.id = id;
        send(&protocol, sizeof(protocol), false);
        send(temp, size);
    }

    virtual void sendmessage(const uint16 id, const void * data, const uint32 len) {
        oProtocol protocol;
        protocol.size = len + sizeof(protocol);
        protocol.id = id;
        send(&protocol, sizeof(protocol), false);
        send(data, len);
    }

protected:
    static bool f_parser_protobuf(const void * data, const int len, ::google::protobuf::Message & package) {
        if (len > 0 && package.ParseFromArray(data, len)) {
            return true;
        }

        tassert(false, "parser protobuf error");
        return false;
    }

    virtual void onMessage(api::iCore * core, const uint16 id, const char * data, const int len, const int64 context = int64(0)) {
        _parser.trigger(id, data, len, core, (session *)this, context);
    }

private:
    tlib::tparser<uint16, ::google::protobuf::Message, api::iCore *, session *, const int64> _parser;
};

#define register_proto(s, id, fun) s->tregister(id, fun, #fun)

#endif //__iSession_h__
