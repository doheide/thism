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

#ifndef BASE_TEST_SYS
#define BASE_TEST_SYS

#include "thism/sm2.h"
#include "thism/baha_qt.h"


// **********************************************************
// **********************************************************
// Define Events
struct E_Lala; MAKE_EVENT(E_Lala, 0);
struct E_Lili; MAKE_EVENT(E_Lili, 0);

struct EventList; MAKE_EVENT_LIST(EventList, E_Lala, E_Lili);

// **********************************************************
#include "base_test_sys_sm.h"

// **********************************************************
// Define state machine system and Timer
typedef SMTimer<Collector<S_Tick, S_Tack>, E_Timer, 111> SMT_Std;
typedef SMTimerListTmpl<EventList, SMT_Std> SMTimerList;

typedef SMSystem<EventList, Collector<SM>, SMTimerList> SMSys;

// *****
extern SMSys *smsys;



#endif
