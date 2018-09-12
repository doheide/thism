#include <QCoreApplication>

#include "sm2.h"
#include "hawal_qt.h"

// **********************************************************
// **********************************************************
// Define Events
struct E_Lala; MAKE_EVENT(E_Lala, 0);

struct EventList; MAKE_EVENT_LIST(EventList, E_Lala);

// **********************************************************
// Define states
struct S_Main;
struct S_Tick; struct S_TickInner;

struct S_Main : public StateBase {
    StateSetup(S_Main, "") { }

    typedef StateDetails< void, TransitionListT<
        TransitionT<E_Initial, S_Tick>
    > > details;
};

struct S_Tick : public StateBase {
    StateSetup(S_Tick, "") { }

    typedef StateDetails< S_Main, TransitionListT<TransitionT<E_Initial, S_TickInner>> > details;
};
struct S_TickInner : public StateBase {
    StateSetup(S_TickInner, "") { }

    typedef StateDetails< S_Tick, TransitionListT<> > details;
};

// Define state machine
struct SM;
Make_StateMachine( SM, MarkInitialState<S_Main>, S_Tick, S_TickInner );

// **********************************************************
// Define state machine system and Timer
typedef SMTimer<Collector<S_Main>> SMT_AT_Idle;
typedef SMTimerListTmpl<EventList, SMT_AT_Idle> SMTimerList;

typedef SMSystem<HAWAL_Base, EventList, Collector<SM>, SMTimerList> SMSys;


// **********************************************************
// **********************************************************
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HAWAL_qt hawal;
    SMSys sys(&hawal);

    return a.exec();
}
