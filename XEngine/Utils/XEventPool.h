#ifndef __XEventPool_h__
#define __XEventPool_h__

#include <unordered_map>
#include <set>

namespace XEngine {
    template<typename E, typename... Args>
    class XEventPool {
    public:
        virtual ~XEventPool() {}

        struct __DebugInfo__ {
            const char* _File;
            const int _Line;
            __DebugInfo__(const char* file, const int line) : _File(file), _Line(line) {}
            __DebugInfo__(const __DebugInfo__& info) : _File(info._File), _Line(info._Line) {}
        };

#define XEventPoolRegist(pool, id, fun) pool.Register(id, fun, __FILE__, __LINE__)

        typedef void(*fEvent)(Args...);
        void Register(const E id, const fEvent fun, const char* file, const int line) {
            auto itor = _Pool.find(id);
            if (_Pool.end() == itor) {
                itor = _Pool.insert({ id, std::unordered_map<fEvent, __DebugInfo__>() }).first;
            }

            itor->second.insert({ fun, __DebugInfo__(file, line) });
        }

        void Trigger(const E id, Args... args) {
            auto itor = _Pool.find(id);
            if (_Pool.end() != itor) {
                for (auto i = itor->second.begin(); i != itor->second.end(); i++) {
                    i->first(args...);
                }
            }

        }

    private:
        std::unordered_map<E, std::unordered_map<fEvent, __DebugInfo__>> _Pool;
    };
}

#endif //__XEventPool_h__
