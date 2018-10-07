#ifndef SM2_SIL_H
#define SM2_SIL_H

#include <thism/sm2.h>
#include <thism/sm2_test_helper.h>

// ***********************************************************
// ***********************************************************
namespace SMSystem_SIL_helper {
    template<typename EventList, typename Events>
    struct EventList_Add;
    template<typename EventList, typename Events>
    struct EventList_Delete;
}


// ***********************************************************
// ***********************************************************
template <typename BAHA, typename SMSys, typename EventsAdd, typename EventsRemove, typename SMsAdd,
          typename SMsRemove, typename TimerAdd, typename TimerRemove>
struct SMSystem_SIL {
    typedef typename detail::CollectorsRemoveSecond<typename SMSys::AllEvents::EventListT::type, EventsRemove>::type EventListElsRemoved;
    typedef typename detail::JoinCollectors<EventListElsRemoved, EventsAdd>::type EventListElsFinal;


    typedef typename detail::CollectorsRemoveSecond<typename SMSys::SMsT, SMsRemove>::type SMsListRemoved;
    typedef typename detail::JoinCollectors<SMsListRemoved, SMsAdd>::type SMsListFinal;

    typedef typename detail::CollectorsRemoveSecond<typename SMSys::SMTimerListT, TimerRemove>::type TimerListRemoved;
    typedef typename detail::JoinCollectors<TimerListRemoved, TimerAdd>::type TimerListFinal;

    typedef SMSystem< BAHA, EventListFinal, SMsListFinal, TimerListFinal> SMSysCur;

    typedef SMSys_Test<TestSys> SMSysCur_Test;
};



#endif

