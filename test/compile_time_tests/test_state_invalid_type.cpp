#include "thism/sm2.h"

struct E_Lala; MAKE_EVENT(E_Lala, 0);
struct EventList; MAKE_EVENT_LIST(EventList, E_Lala);


#include "base_test_sys_sm.h"

struct SM_Test;
Make_StateMachine( SM_Test, MarkInitialState<S_Main>, S_Tick, S_TickInner, S_TickInnerInner, S_Tack, int );




