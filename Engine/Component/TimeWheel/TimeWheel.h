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
        virtual void FixedUpdate(Api::iEngine* const engine) override;
        virtual void LaterUpdate(Api::iEngine* const engine) override;

        virtual void StartTimer(Api::iTimer* timer, const int id, const int delay, const int count, const int interval, void* const context, const char* const file, const int line) override;
        virtual void KillTimer(Api::iTimer* timer, const int id, void* const context = nullptr) override;
        virtual void PauseTimer(Api::iTimer* timer, const int id, void* const context = nullptr) override;
        virtual void ResumeTimer(Api::iTimer* timer, const int id, void* const context = nullptr) override;

        TimeBase* CreateTimerBase(Api::iTimer* timer, const int id, void* const context, int count, int interval, const char* file, const int line);
        void RemoveTimerBase(TimeBase* base, Api::iTimer* timer, const int id, void* const context);
        TimeBase* FindTimerBase(Api::iTimer* timer, const int id, void* const context);

        void Schedule(TimeBase* base);
        void MoveToRunning(TimeBase* base);
        inline unsigned_int64 Jiff() const { return _jiff; }
    private:
        TimeWheel() : _jiff(0) {
            SafeMemory::Memset(_time_gears, sizeof(_time_gears), 0, sizeof(_time_gears));
        }
        ~TimeWheel() {}

        TimeBaseList* FindTimerList(unsigned_int64 expire);
        void _Update_();
        void Remove(TimeBase* base);

    private:
        unsigned_int64 _jiff;
        TimeGear* _time_gears[5];
        TimeBaseList* _running;
        TimeBaseList* _suspended;
    };
}

#endif //__TimeWheel_h__
