#ifndef __timermgr_h__
#define __timermgr_h__

#include "multisys.h"
#include "api.h"
#include <string.h>

namespace tcore {
    using namespace api;

#define min_interval 4

#define jiffies_interval 4
    class tlist;
    class tbase;
    class tgear;
    class timermgr {
        enum
        {
            tq_tvn_bits = 6,
            tq_tvr_bits = 8,
            tq_tvn_size = 1 << tq_tvn_bits,//64
            tq_tvr_size = 1 << tq_tvr_bits,//256
            tq_tvn_mask = tq_tvn_size - 1,//63
            tq_tvr_mask = tq_tvr_size - 1,//255
        };

    public:
        static timermgr* getInstance() {
            static timermgr* instance = nullptr;

            if (instance == nullptr) {
                instance = tnew timermgr;
                if (!instance->launch()) {
                    tdel instance;
                    instance = nullptr;
                }
            }

            return instance;
        }


        bool launch();
        void update();
        void shutdown();

        tbase* createTimerBase(iTimer* timer, const int id, const iContext& context, int count, int64 interval, const char* file, const int line);
        void removeTimerBase(tbase* base, iTimer* timer, const int id, const iContext& context);
        tbase* findTimerBase(iTimer* timer, const int id, const iContext& context);

        void startTimer(iTimer* timer, const int id, const iContext& context, const int64 delay, const int count, const int64 interval, const char* file, const int line);
        void killTimer(iTimer* timer, const int id, const iContext& context);
        void pauseTimer(iTimer* timer, const int id, const iContext& context);
        void resumeTimer(iTimer* timer, const int id, const iContext& context);
        virtual void traceTimer();

        void schedule(tbase* base);
        void moveToRunning(tbase* base);

        uint64 jiffies() const { return _jiffies; }

    private:
        timermgr() : _jiffies(0) {
            memset(_timerGear, 0, sizeof(_timerGear));
        }
        ~timermgr() {}

        tlist* findTimerList(uint64 expire);
        void _update();
        void del(tbase* base);

    private:
        uint64 _jiffies;
        tgear* _timerGear[5];
        tlist* _running;
        tlist* _suspended;
    };
}

#endif //__timermgr_h__
