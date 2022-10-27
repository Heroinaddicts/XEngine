#ifndef __TimeBaseList_h__
#define __TimeBaseList_h__

#include "Header.h"

namespace XEngine {
    class TimeBase;
    class TimeBaseList {
    public:
        TimeBaseList() : _Head(nullptr), _Tail(nullptr) {}
        virtual ~TimeBaseList() {}

        void PushBack(TimeBase* p);
        TimeBase* PopFront();
        void Remove(TimeBase* p);

        inline bool Empty() { return _Head == nullptr; }

    private:
        TimeBaseList(TimeBaseList&);
        TimeBaseList& operator=(TimeBaseList&);

        TimeBase* _Head;
        TimeBase* _Tail;
    };
}

#endif //__TimeBaseList_h__
