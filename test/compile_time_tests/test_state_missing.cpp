#include "thism/sm2.h"


// **********************************************************
// **********************************************************
// Define Events
struct E_Lala; MAKE_EVENT(E_Lala, 0);
struct E_Lili; MAKE_EVENT(E_Lili, 0);

struct EventList; MAKE_EVENT_LIST(EventList, E_Lala);


// **********************************************************
// **********************************************************
#include "base_test_sys_sm.h"

// **********************************************************
// **********************************************************
struct SM_Test;
Make_StateMachine( SM_Test, MarkInitialState<S_Main>, S_Tick, S_TickInner, S_Tack ); // S_TickInnerInner is missing so that the transition should fail


typedef SMTimer<Collector<S_Tick, S_Tack>> SMT_Std;
typedef SMTimerListTmpl<EventList, SMT_Std> SMTimerList;

typedef SMSystem<BAHA_Base, EventList, Collector<SM_Test>, SMTimerList> SMSys_Test;

BAHA_Base bb;
SMSys_Test sys(&bb);


