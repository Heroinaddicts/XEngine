#include "TimeBaseList.h"
#include "TimeBase.h"

namespace XEngine {
    void TimeBaseList::PushBack(TimeBase* p) {
        p->SetList(this);
        p->SetNext(nullptr);
        p->SetPrev(_Tail);
        if (_Tail)
            _Tail->SetNext(p);
        else
            _Head = p;

        _Tail = p;
    }

    TimeBase* TimeBaseList::PopFront() {
        TimeBase* p = _Head;
        if (_Head) {
            _Head = _Head->GetNext();
            if (!_Head)
                _Tail = nullptr;
            else
                _Head->SetPrev(nullptr);

            p->SetNext(nullptr);
            p->SetPrev(nullptr);
            p->SetList(nullptr);
        }
        return p;
    }

    void TimeBaseList::Remove(TimeBase* p) {
        if (p->GetPrev())
            p->GetPrev()->SetNext(p->GetNext());

        if (p->GetNext())
            p->GetNext()->SetPrev(p->GetPrev());

        if (p == _Head)
            _Head = _Head->GetNext();

        if (p == _Tail)
            _Tail = _Tail->GetPrev();

        p->SetNext(nullptr);
        p->SetPrev(nullptr);
        p->SetList(nullptr);
    }
}
