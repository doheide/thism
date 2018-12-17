#include "thism/sm2.h"


// **********************************************************
// **********************************************************
// Define Events
struct E_Lala; MAKE_EVENT(E_Lala, 0);

struct EventList; MAKE_EVENT_LIST(EventList, E_Lala);


// **********************************************************
// **********************************************************
#include "base_test_sys_sm.h"

// **********************************************************
// **********************************************************
struct SM_Test;
Make_StateMachine( SM_Test, S_Main, S_Tick, S_TickInner, S_TickInnerInner, S_Tack );


typedef SMTimer<Collector<S_Tick, S_Tack>> SMT_Std;
typedef SMTimerListTmpl<EventList, SMT_Std> SMTimerList;

typedef SMSystem<EventList, Collector<SM_Test>, SMTimerList> SMSys_Test;

BAHA_TYPE bb;
SMSys_Test sys(&bb);


