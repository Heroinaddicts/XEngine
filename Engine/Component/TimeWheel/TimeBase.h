#ifndef __TimeBase_h__
#define __TimeBase_h__

#include "Header.h"

namespace XEngine {
    class TimeBaseList;

    class TimeBase {
    public:
        static TimeBase* Create(Api::iTimer* timer, const int id, void* const context, const int count, const int interval, const char* file, const int line);

        void OnTimer();
        void ForceEnd();
        void Pause(unsigned_int32 jiff);
        void Resume(unsigned_int32 jiff);

        void Release();

        inline bool IsValid() const { return _valid; }
        inline bool IsPolling() const { return _polling; }

        inline bool IsPaused() const { return _paused; }

        inline unsigned_int64 GetExpire() const { return _expire; }
        inline void SetExpire(unsigned_int64 expire) { _expire = expire; }
        void AdjustExpire(unsigned_int64 now);

        inline void SetNext(TimeBase* next) { _next = next; }
        inline TimeBase* GetNext() const { return _next; }

        inline void SetPrev(TimeBase* prev) { _prev = prev; }
        inline TimeBase* GetPrev() const { return _prev; }

        inline void SetList(TimeBaseList* list) { _list = list; }
        inline TimeBaseList* GetList() const { return _list; }

        inline Api::iTimer* GetTimer() const { return _timer; }

    public:
        const int _id;
        const int _interval;
        void* const _context;

#ifdef _DEBUG
        const std::string _file;
        const int _line;
#endif //_DEBUG

    private:
        friend XPool<TimeBase>;
        TimeBase(Api::iTimer* timer, const int id, void* const context, int count, int interval, const char* file, const int line);

    private:
        TimeBaseList* _list;
        TimeBase* _next;
        TimeBase* _prev;

        Api::iTimer* const _timer;

        bool _valid;
        bool _polling;

        unsigned_int64 _expire;
        int _count;
        bool _started;

        unsigned_int64 _pause_tick;
        bool _paused;
    };
}

#endif //__TimeBase_h__

