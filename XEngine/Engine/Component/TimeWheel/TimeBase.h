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

        inline bool IsValid() const { return _Valid; }
        inline bool IsPolling() const { return _Polling; }

        inline bool IsPaused() const { return _Paused; }

        inline unsigned_int64 GetExpire() const { return _Expire; }
        inline void SetExpire(unsigned_int64 expire) { _Expire = expire; }
        void AdjustExpire(unsigned_int64 now);

        inline void SetNext(TimeBase* next) { _Next = next; }
        inline TimeBase* GetNext() const { return _Next; }

        inline void SetPrev(TimeBase* prev) { _Prev = prev; }
        inline TimeBase* GetPrev() const { return _Prev; }

        inline void SetList(TimeBaseList* list) { _List = list; }
        inline TimeBaseList* GetList() const { return _List; }

        inline Api::iTimer* GetTimer() const { return _Timer; }

    public:
        const int _Id;
        const int _Interval;
        void* const _Context;

#ifdef _DEBUG
        const std::string _File;
        const int _Line;
#endif //_DEBUG

    private:
        friend XPool<TimeBase>;
        TimeBase(Api::iTimer* timer, const int id, void* const context, int count, int interval, const char* file, const int line);

    private:
        TimeBaseList* _List;
        TimeBase* _Next;
        TimeBase* _Prev;

        Api::iTimer* const _Timer;

        bool _Valid;
        bool _Polling;

        unsigned_int64 _Expire;
        int _Count;
        bool _Started;

        unsigned_int64 _PauseTick;
        bool _Paused;
    };
}

#endif //__TimeBase_h__

