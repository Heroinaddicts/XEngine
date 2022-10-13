#ifndef __TimeBaseList_h__
#define __TimeBaseList_h__

#include "Header.h"

namespace XEngine {
    class TimeBase;
    class TimeBaseList {
    public:
        TimeBaseList() : _head(nullptr), _tail(nullptr) {}
        virtual ~TimeBaseList() {}

        void PushBack(TimeBase* p);
        TimeBase* PopFront();
        void Remove(TimeBase* p);

        inline bool Empty() { return _head == nullptr; }

    private:
        TimeBaseList(TimeBaseList&);
        TimeBaseList& operator=(TimeBaseList&);

        TimeBase* _head;
        TimeBase* _tail;
    };
}

#endif //__TimeBaseList_h__
