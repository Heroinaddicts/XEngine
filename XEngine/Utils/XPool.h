#ifndef __XPool_h__
#define __XPool_h__

#include "SafeMemory.h"

namespace XEngine {
    template <typename T, int ChunkCount = 1, int ChunkSize = 64>
    class XPool {
        enum {
            InUse,
            IsFree,
        };

        struct ChunkList;
        struct Chunk {
            char _Buffer[sizeof(T)];
            ChunkList* _Parent;
            Chunk* _Prev;
            Chunk* _Next;
            int _Len;
            int8 _State;
#ifdef _DEBUG
            char _File[128];
            int _Line;
#endif
        };

        struct ChunkList {
            int _Count;
            ChunkList* _Prev;
            ChunkList* _Next;
            Chunk _Chunks[ChunkSize];
        };

    public:
        XPool() : _Head(nullptr), _ListHead(nullptr), _ChunkCount(0) {
            AllocChunk(ChunkCount);
        }

        ~XPool() {
            while (_ListHead != nullptr) {
                auto tmp = _ListHead;
                _ListHead = _ListHead->_Next;

                xfree(tmp);
            }
        }

#ifdef _DEBUG
        T* CreateObject(const char* file, const int line) {
            Chunk* chunk = Create(file, line);
#else
        T* CreateObject() {
            Chunk* chunk = Create();
#endif
            T* t = xnew(chunk->_Buffer) T();
            return t;
        }

        template<typename... Args>
#ifdef _DEBUG
        T* CreateObject(const char* file, const int line, Args... args) {
            Chunk* chunk = Create(file, line);
#else
        T* CreateObject(Args... args) {
            Chunk* chunk = Create();
#endif
            T* t = xnew(chunk->_Buffer) T(args...);
            return t;
        }

#ifdef _DEBUG
        void RecoverObject(T * t, const char* file, const int line) {
#else
        void RecoverObject(T * t) {
#endif //_DEBUG
            t->~T();
            Recover((Chunk*)t);
        }

        int Count() { return _ChunkCount * ChunkSize; }

    private:
#ifdef _DEBUG
        Chunk* Create(const char* file, const int line) {
#else
        Chunk* Create() {
#endif
            Chunk* ret = nullptr;
            if (_Head == nullptr)
                AllocChunk(1);

            ret = _Head;
            Remove(_Head);
            XASSERT(ret->_State == IsFree && ret->_Len == sizeof(Chunk), "chunk invalid");
            ++ret->_Parent->_Count;
            ret->_State = InUse;
#ifdef _DEBUG
            sprintf_s(ret->_File, sizeof(ret->_File), "%s", file);
            ret->_Line = line;
#endif
            return ret;
        }

        void Recover(Chunk * chunk) {
            XASSERT(chunk->_State == InUse && chunk->_Len == sizeof(Chunk), "Recover invalid chunk");
            XASSERT(chunk->_Parent->_Count > 0, "chunk list error");
            --chunk->_Parent->_Count;
            chunk->_State = IsFree;
            if (chunk->_Parent->_Count == 0 && _ChunkCount > ChunkCount)
                FreeChunkList(chunk->_Parent);
            else
                Add(chunk);
        }

        void AllocChunk(int count) {
            for (int i = 0; i < count; ++i) {
                ChunkList* new_list = (ChunkList*)xmalloc(sizeof(ChunkList));
                XASSERT(new_list, "xnew chunk faild");
                ArrangeChunkList(new_list);

                new_list->_Next = _ListHead;
                if (_ListHead != nullptr)
                    _ListHead->_Prev = new_list;
                _ListHead = new_list;
            }

            _ChunkCount += count;
        }

        inline void FreeChunkList(ChunkList * chunk_list) {
            for (int i = 0; i < ChunkSize; ++i) {
                XASSERT(chunk_list->_Chunks[i]._State == IsFree, "free chunk list but child chunk is not free");
                Remove(&(chunk_list->_Chunks[i]));
            }

            if (chunk_list->_Next != nullptr)
                chunk_list->_Next->_Prev = chunk_list->_Prev;

            if (chunk_list->_Prev != nullptr)
                chunk_list->_Prev->_Next = chunk_list->_Next;

            if (_ListHead == chunk_list)
                _ListHead = chunk_list->_Next;

            xfree(chunk_list);
            --_ChunkCount;
        }

        inline void ArrangeChunkList(ChunkList * chunk_list) {
            chunk_list->_Prev = nullptr;
            chunk_list->_Next = nullptr;
            chunk_list->_Count = 0;
            for (int i = 0; i < ChunkSize; ++i) {
                chunk_list->_Chunks[i]._Parent = chunk_list;
                chunk_list->_Chunks[i]._State = IsFree;
                chunk_list->_Chunks[i]._Len = sizeof(Chunk);
                chunk_list->_Chunks[i]._Prev = nullptr;
                chunk_list->_Chunks[i]._Next = nullptr;

                Add(&(chunk_list->_Chunks[i]));
            }
        }

        inline void Add(Chunk * chunk) {
            chunk->_Next = _Head;
            if (_Head != nullptr)
                _Head->_Prev = chunk;
            _Head = chunk;
        }

        inline void Remove(Chunk * chunk) {
            if (chunk->_Next != nullptr)
                chunk->_Next->_Prev = chunk->_Prev;

            if (chunk->_Prev != nullptr)
                chunk->_Prev->_Next = chunk->_Next;

            if (_Head == chunk)
                _Head = chunk->_Next;

            chunk->_Next = nullptr;
            chunk->_Prev = nullptr;
        }

    private:
        Chunk* _Head;
        ChunkList* _ListHead;
        int _ChunkCount;
    };
}

#ifdef _DEBUG
#define XPOOL_CREATE(pool, ...) pool.CreateObject(__FILE__, __LINE__, ##__VA_ARGS__)
#define XPOOL_RELEASE(pool, p) pool.RecoverObject((p), __FILE__, __LINE__)
#else
#define XPOOL_CREATE(pool, ...) pool.CreateObject( __VA_ARGS__ )
#define XPOOL_RELEASE(pool, p) pool.RecoverObject(p)
#endif
#endif //__XPool_h__
