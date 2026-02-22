#ifndef __CommonQueue_h__
#define __CommonQueue_h__

#include "MultiSys.h"

#include "XPool.h"
#include <queue>

namespace XEngine {
    template<typename T>
    class CommonQueue {
    public:
        ~CommonQueue() {}

        __forceinline bool Push(const T& t) {
            if (_NodeMap.find(t) != _NodeMap.end()) {
                return false;
            }

            QueueNode* node = XPOOL_CREATE(_NodePool, t);
            _NodeMap.insert({ t, node });
            _Queue.push(node);
            return true;
        }

        __forceinline bool Pop(OUT T& t) {
            if (_Queue.empty()) {
                return false;
            }

            QueueNode* node = _Queue.front();
            _Queue.pop();
            while (node && node->_Invalid) {
                XPOOL_RELEASE(_NodePool, node);
                node = nullptr;
                if (!_Queue.empty()) {
                    node = _Queue.front();
                    _Queue.pop();
                }
            }

            if (nullptr == node) {
                return false;
            }

            t = node->_T;
            _NodeMap.erase(t);
            XPOOL_RELEASE(_NodePool, node);
            return true;
        }

        __forceinline bool IsExist(const T& t) const {
            return _NodeMap.find(t) != _NodeMap.cend();
        }

        __forceinline void Delete(const T& t) {
            auto itor = _NodeMap.find(t);
            if (_NodeMap.end() != itor) {
                itor->second->_Invalid = true;
                _NodeMap.erase(itor);
            }
        }

        __forceinline int Count() const {
            return _NodeMap.size();
        }

    protected:
        struct QueueNode {
            const T _T;
            bool _Invalid;

            QueueNode(const T& t) : _T(t), _Invalid(false) {}
        };

        XPool<QueueNode> _NodePool;

        std::map<T, QueueNode*> _NodeMap;
        std::queue<QueueNode*> _Queue;
    };
}

#endif //__CommonQueue_h__
