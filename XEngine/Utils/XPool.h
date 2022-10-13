#ifndef __XPool_h__
#define __XPool_h__

#include "SafeMemory.h"

namespace XEngine {
    template <typename T, int chunk_count = 1, int chunk_size = 64>
    class XPool {
        enum {
            in_use,
            is_free,
        };

        struct ChunkList;
        struct Chunk {
            char buffer[sizeof(T)];
            ChunkList* parent;
            Chunk* prev;
            Chunk* next;
            int len;
            int8 state;
#ifdef _DEBUG
            char file[128];
            int line;
#endif
        };

        struct ChunkList {
            int count;
            ChunkList* prev;
            ChunkList* next;
            Chunk chunks[chunk_size];
        };

    public:
        XPool() : _head(nullptr), _list_head(nullptr), _chunk_count(0) {
            AllocChunk(chunk_count);
        }

        ~XPool() {
            while (_list_head != nullptr) {
                auto tmp = _list_head;
                _list_head = _list_head->next;

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
            T* t = xnew(chunk->buffer) T();
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
            T* t = xnew(chunk->buffer) T(args...);
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

        int Count() { return _chunk_count * chunk_size; }

    private:
#ifdef _DEBUG
        Chunk* Create(const char* file, const int line) {
#else
        Chunk* Create() {
#endif
            Chunk* ret = nullptr;
            if (_head == nullptr)
                AllocChunk(1);

            ret = _head;
            Remove(_head);
            XASSERT(ret->state == is_free && ret->len == sizeof(Chunk), "chunk invalid");
            ++ret->parent->count;
            ret->state = in_use;
#ifdef _DEBUG
            sprintf_s(ret->file, sizeof(ret->file), "%s", file);
            ret->line = line;
#endif
            return ret;
        }

        void Recover(Chunk * _chunk) {
            XASSERT(_chunk->state == in_use && _chunk->len == sizeof(Chunk), "Recover invalid chunk");
            XASSERT(_chunk->parent->count > 0, "chunk list error");
            --_chunk->parent->count;
            _chunk->state = is_free;
            if (_chunk->parent->count == 0 && _chunk_count > chunk_count)
                FreeChunkList(_chunk->parent);
            else
                Add(_chunk);
        }

        void AllocChunk(int count) {
            for (int i = 0; i < count; ++i) {
                ChunkList* new_list = (ChunkList*)xmalloc(sizeof(ChunkList));
                XASSERT(new_list, "xnew chunk faild");
                ArrangeChunkList(new_list);

                new_list->next = _list_head;
                if (_list_head != nullptr)
                    _list_head->prev = new_list;
                _list_head = new_list;
            }

            _chunk_count += count;
        }

        inline void FreeChunkList(ChunkList * chunk_list) {
            for (int i = 0; i < chunk_size; ++i) {
                XASSERT(chunk_list->chunks[i].state == is_free, "free chunk list but child chunk is not free");
                Remove(&(chunk_list->chunks[i]));
            }

            if (chunk_list->next != nullptr)
                chunk_list->next->prev = chunk_list->prev;

            if (chunk_list->prev != nullptr)
                chunk_list->prev->next = chunk_list->next;

            if (_list_head == chunk_list)
                _list_head = chunk_list->next;

            xfree(chunk_list);
            --_chunk_count;
        }

        inline void ArrangeChunkList(ChunkList * chunk_list) {
            chunk_list->prev = nullptr;
            chunk_list->next = nullptr;
            chunk_list->count = 0;
            for (int i = 0; i < chunk_size; ++i) {
                chunk_list->chunks[i].parent = chunk_list;
                chunk_list->chunks[i].state = is_free;
                chunk_list->chunks[i].len = sizeof(Chunk);
                chunk_list->chunks[i].prev = nullptr;
                chunk_list->chunks[i].next = nullptr;

                Add(&(chunk_list->chunks[i]));
            }
        }

        inline void Add(Chunk * chunk) {
            chunk->next = _head;
            if (_head != nullptr)
                _head->prev = chunk;
            _head = chunk;
        }

        inline void Remove(Chunk * chunk) {
            if (chunk->next != nullptr)
                chunk->next->prev = chunk->prev;

            if (chunk->prev != nullptr)
                chunk->prev->next = chunk->next;

            if (_head == chunk)
                _head = chunk->next;

            chunk->next = nullptr;
            chunk->prev = nullptr;
        }

    private:
        Chunk* _head;
        ChunkList* _list_head;
        int _chunk_count;
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
