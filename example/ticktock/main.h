//
// MIT License
//
// Copyright (c) 2018 Dominik Heide
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "sm2.h"
#include "baha_qt.h"


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

typedef SMSystem<BAHA_Base, EventList, Collector<SM>, SMTimerList> SMSys;

// *****
extern SMSys *smsys;

