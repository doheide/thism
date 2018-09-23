#ifndef BASE_TEST_SYS_SM_H
#define BASE_TEST_SYS_SM_H

// **********************************************************
// Define states
struct S_Main;
struct S_Tick; struct S_TickInner; struct S_TickInnerInner;
struct S_Tack;

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
        TransitionT<E_Timer, S_Tack>,
        TransitionT<E_Lala, S_Tack>
    > > details;

    void onEnter(uint16_t, bool);
};
struct S_TickInner : public StateBase {
    StateSetup(S_TickInner, "") {
    }

    typedef StateDetails< S_Tick, TransitionListT<
        TransitionT<E_Initial, S_TickInnerInner>
    > > details;
};
struct S_TickInnerInner : public StateBase {
    StateSetup(S_TickInnerInner, "") {
    }

    typedef StateDetails< S_TickInner, TransitionListT<> > details;
};

struct S_Tack : public StateBase {
    StateSetup(S_Tack, "") { }

    typedef StateDetails< S_Main, TransitionListT<TransitionT<E_Timer, S_Tick>> > details;

    void onEnter(uint16_t, bool);
};

// Define state machine
struct SM;
Make_StateMachine( SM, MarkInitialState<S_Main>, S_Tick, S_TickInner, S_TickInnerInner, S_Tack );


#endif // BASE_TEST_SYS_SM_H
