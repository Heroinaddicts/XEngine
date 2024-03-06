#ifndef __TimeWheel_h__
#define __TimeWheel_h__

#include "Header.h"

namespace XEngine {

    class TimeBase;
    class TimeBaseList;
    class TimeGear;


    class TimeWheel : public iTimeWheel {
    public:
        static iTimeWheel* GetInstance();

        // 通过 iTimeWheel 继承
        virtual bool Initialize(Api::iEngine* const engine) override;
        virtual bool Launch(Api::iEngine* const engine) override;
        virtual void Release(Api::iEngine* const engine) override;

        virtual void EarlyUpdate(Api::iEngine* const engine) override;
        virtual void Update(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

        virtual void StartTimer(Api::iTimer* timer, const int id, const int delay, const int count, const int interval, const UInt64 context, const char* const file, const int line) override;
        virtual void KillTimer(Api::iTimer* timer, const int id, const UInt64 context = 0) override;
        virtual void PauseTimer(Api::iTimer* timer, const int id, const UInt64 context = 0) override;
        virtual void ResumeTimer(Api::iTimer* timer, const int id, const UInt64 context = 0) override;

        TimeBase* CreateTimerBase(Api::iTimer* timer, const int id, const UInt64 context, int count, int interval, const char* file, const int line);
        void RemoveTimerBase(TimeBase* base, Api::iTimer* timer, const int id, const UInt64 context);
        TimeBase* FindTimerBase(Api::iTimer* timer, const int id, const UInt64 context);

        void Schedule(TimeBase* base);
        void MoveToRunning(TimeBase* base);
        inline UInt64 Jiff() const { return _Jiff; }
    private:
        TimeWheel() : _Jiff(0) {
            SafeMemory::Memset(_TimeGears, sizeof(_TimeGears), 0, sizeof(_TimeGears));
        }
        ~TimeWheel() {}

        TimeBaseList* FindTimerList(UInt64 expire);
        void _Update_();
        void Remove(TimeBase* base);

    private:
        UInt64 _Jiff;
        TimeGear* _TimeGears[5];
        TimeBaseList* _Running;
        TimeBaseList* _Suspended;
    };
}

#endif //__TimeWheel_h__
