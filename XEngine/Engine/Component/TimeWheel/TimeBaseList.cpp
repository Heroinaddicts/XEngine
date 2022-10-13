#include "TimeBaseList.h"
#include "TimeBase.h"

namespace XEngine {
    void TimeBaseList::PushBack(TimeBase* p) {
        p->SetList(this);
        p->SetNext(nullptr);
        p->SetPrev(_tail);
        if (_tail)
            _tail->SetNext(p);
        else
            _head = p;

        _tail = p;
    }

    TimeBase* TimeBaseList::PopFront() {
        TimeBase* p = _head;
        if (_head) {
            _head = _head->GetNext();
            if (!_head)
                _tail = nullptr;
            else
                _head->SetPrev(nullptr);

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

        if (p == _head)
            _head = _head->GetNext();

        if (p == _tail)
            _tail = _tail->GetPrev();

        p->SetNext(nullptr);
        p->SetPrev(nullptr);
        p->SetList(nullptr);
    }
}
