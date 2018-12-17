#ifndef __SMSYS_H
#define __SMSYS_H

#include "thism/sm2.h"


// **********************************************************
// **********************************************************
// Define Events
struct E_Lala; MAKE_EVENT(E_Lala, 0);

struct EventList; MAKE_EVENT_LIST(EventList, E_Lala);

// **********************************************************
// Define states
struct S_Main;
struct S_Tick; struct S_TickInner; struct S_Tack;

struct S_Main : public StateBase {
    StateSetup(S_Main, "") { }

    typedef StateDetails< void, TransitionListT<
        TransitionT<E_Initial, S_Tick>
    > > details;
};

struct S_Tick : public StateBase {
    StateSetup(S_Tick, "") { }

    typedef StateDetails< S_Main, TransitionListT<
        TransitionT<E_Initial, S_TickInner>,
        TransitionT<E_Timer, S_Tack>
    > > details;

    void onEnter(uint16_t, bool);
};
struct S_TickInner : public StateBase {
    StateSetup(S_TickInner, "") {
    }

    typedef StateDetails< S_Tick, TransitionListT<> > details;
};

struct S_Tack : public StateBase {
    StateSetup(S_Tack, "") { }

    typedef StateDetails< S_Main, TransitionListT<TransitionT<E_Timer, S_Tick>> > details;

    void onEnter(uint16_t, bool);
};

// Define state machine
struct SM;
Make_StateMachine( SM, MarkInitialState<S_Main>, S_Tick, S_TickInner, S_Tack );

// **********************************************************
// Define state machine system and Timer
typedef SMTimer<Collector<S_Tick, S_Tack>> SMT_Std;
typedef SMTimerListTmpl<EventList, SMT_Std> SMTimerList;

typedef SMSystem<EventList, Collector<SM>, SMTimerList> SMSys;

// *****
extern SMSys *smsys;



#endif
