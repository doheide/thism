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
    typedef EventListXXX EventListAdd;
    typedef EventListXXX EventListFinal;

    typedef SMsListXXX SMsListAdd;
    typedef SMsListXXX SMsListFinal;

    typedef TimerListXXX TimerListAdd;
    typedef TimerListXXX TimerListFinal;

    typedef SMSystem< BAHA, EventListFinal, SMsListFinal, TimerListFinal> SMSysCur;

    typedef SMSys_Test<TestSys> SMSysCur_Test;
};



#endif

