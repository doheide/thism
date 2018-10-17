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

#ifndef SM2_H
#define SM2_H

#include <type_traits>
#include <stdint.h>


// *****************************************************************
//#define __make_treeuml

// *****************************************************************
// *****************************************************************
#define EVENT_BUFFER_SIZE_V 4
#define STATE_NESTING_LEVEL_MAX 30

#define __useNames
//#define __noNames

#define __useDescription
//#define __noDescription

// *****************************************************************
// *****************************************************************
#if !defined(__useNames) && !defined(__noNames)
static_assert(false, "SM: Either '__useNames' or '__noNames' have to be defined.");
#endif

#if defined(__useNames) && defined(__noNames)
static_assert(false, "SM: Either '__useNames' or '__noNames' have to be defined, not both!");
#endif

#if !defined(__useDescription) && !defined(__noDescription)
static_assert(false, "SM: Either '__noDescription' or '__noDescription' have to be defined.");
#endif

#if defined(__useDescription) && defined(__noDescription)
static_assert(false, "SM: Either '__noDescription' or '__noDescription' have to be defined, not both!");
#endif

#if !defined(__useNames) && defined(__useDescription)
static_assert(false, "SM: If '__useDescription' is active '__useNames' have also to be defined activated.");
#endif

// *****************************************************************
// *****************************************************************
#define EVENT_BUFFER_SIZE_V 4



// ******************************************************************
// ******************************************************************
#include <thism/sm2_base.h>

// ******************************************************************
// ******************************************************************
class SystemBase;

struct EventIdT { uint16_t id; };
struct StateIdT { uint16_t id; };

class BAHA_Base {
protected:
    uint32_t sysTime;
    SystemBase *sys;

public:
    BAHA_Base() : sysTime(0) { }

    void sysSet(SystemBase *_sys) {
        sys = _sys;
    }

    virtual void log(const char *) {
    }
    void log(uint8_t n);
    void log(uint16_t n);
    void log(uint32_t n);
    void log(EventIdT id);
    void log(StateIdT id);

    template<typename ...A>
    void logLine(A... a) {
        log(sysTime);
        log(" | ");
        logLineImpl(a...);
        logLineEnd();
    }
    virtual void logLineEnd() { }

    virtual void sysTickCallback();

    void processEvents();

protected:
    template<typename A, typename ...As>
    void logLineImpl(A ca, As... c) {
        log(ca);
        logLineImpl(c ...);
    }
    void logLineImpl() { }

    void logNumberImpl(uint32_t n, uint8_t digits);

};


// ******************************************************************
// ******************************************************************
namespace sys_detail {
    namespace helper {
        template <typename ...>
        struct Id_Impl;
        template <typename CSTATE, uint16_t IDX, typename STATE, typename ... STATEs>
        struct Id_Impl<CSTATE, std::integral_constant<uint16_t, IDX>, Collector<STATE, STATEs...>> {
            typedef typename Id_Impl<CSTATE, std::integral_constant<uint16_t, IDX+1>, Collector<STATEs...>>::type type;
        };
        template <typename CSTATE, uint16_t IDX, typename ... STATEs>
        struct Id_Impl<CSTATE, std::integral_constant<uint16_t, IDX>, Collector<CSTATE, STATEs...>> {
            typedef typename std::integral_constant<uint16_t, IDX> type;
        };
        template <typename CSTATE, uint16_t IDX>
        struct Id_Impl<CSTATE, std::integral_constant<uint16_t, IDX>, Collector<>> {
            typedef typename std::integral_constant<uint16_t, 0xFFFF> type;
        };
    }
}

// ******************************************************************
#define EOPT_ONLY_FROM_SELF 1
#define EOPT_ONLY_FROM_SELF_OR_PARENT 3
#define EOPT_ONLY_FROM_SELF_OR_PARENT_OR_CHILD 7

#ifdef __useNames
#define MAKE_EVENT(EVENTNAME, OPTS) \
struct EVENTNAME { \
    typedef event_details::EventBase details; \
    static const char * name() { return #EVENTNAME; }\
    typedef std::integral_constant<uint8_t, OPTS> val_t; \
}
#else
#define MAKE_EVENT(EVENTNAME, OPTS) \
struct EVENTNAME { \
    typedef event_details::EventBase details; \
    std::integral_constant<uint8_t, OPTS> val_t; \
}
#endif

namespace event_details {
    struct EventBase {};

    // *******************************************************
    template<typename X>
    struct EventConceptCheck {
        static_assert(detail::has_type_details<X>::value,
                      "CTC: Event struct has to be have a member ::details of the type EventBase.");
        static_assert(std::is_same<EventBase, typename X::details>::value,
                      "CTC: Event struct has to be have a member ::details of the type EventBase.");
#ifdef __useNames
//        static_assert(detail::has_static_name<X>::value,
//                      "CTC Event struct has to be have a member static method named ::name returning const char*.");
#endif
        static constexpr auto value = std::true_type();
    };
    template<typename X, typename ...A>
    struct EventEnsureEveryConcept {
        static_assert(EventConceptCheck<X>::value, "");
        static constexpr auto value = EventEnsureEveryConcept<A...>::value;
    };
    template<typename X>
    struct EventEnsureEveryConcept<X> {
        static constexpr auto  value = EventConceptCheck<X>::value;
    };

    // ***************************************************************
    template<typename ...>
    struct GetEventNameImpl;
    template<uint16_t CID, typename EVENT, typename ... EVENTS>
    struct GetEventNameImpl<std::integral_constant<uint16_t, CID>, Collector<EVENT, EVENTS...>> {
        const char* impl(uint16_t eid) {
            if(eid == CID)
                return EVENT::name();

            GetEventNameImpl<std::integral_constant<uint16_t, CID+1>, Collector<EVENTS...>> geni;
            return geni.impl(eid);
        }
    };
    template<uint16_t CID>
    struct GetEventNameImpl<std::integral_constant<uint16_t, CID>, Collector<>> {
        const char* impl(uint16_t ) {
            return "E_UNKNOWN";
        }
    };
    template<typename EVENTLIST>
    const char* getEventName(uint16_t eid) {
        GetEventNameImpl<std::integral_constant<uint16_t, 0>, typename EVENTLIST::AllEvents::type> geni;
        return geni.impl(eid);
    }
}

// ******************************************************************
// *****************************************************************
namespace event_details {
    struct EventListBase { };

    template<typename ...A>
    struct EventListAll {
        static_assert(event_details::EventEnsureEveryConcept<A...>::value, "lala");
        typedef Collector<A...> type;
        static const uint16_t size = sizeof...(A);

        template<typename CEV>
        using EventId = typename sys_detail::helper::Id_Impl<CEV, std::integral_constant<uint16_t, 0>, Collector<A...>>::type;

        uint8_t eventOpts[sizeof...(A)];

        EventListAll() : eventOpts{ A::val_t::value ... }
        { }
    };
}

struct E_FatalError; MAKE_EVENT(E_FatalError, 0);
struct E_Initial; MAKE_EVENT(E_Initial, EOPT_ONLY_FROM_SELF);
struct E_Timer; MAKE_EVENT(E_Timer, EOPT_ONLY_FROM_SELF);

#define ID_E_FatalError 0
#define ID_E_Initial 1
#define ID_E_Timer 2
template<typename ...A>
struct EventListAll : event_details::EventBase {
    typedef typename event_details::EventListAll<E_FatalError, E_Initial, E_Timer, A...> AllEvents;
    typedef typename AllEvents::type type;

    template<typename CEV>
    using EventId = typename AllEvents::template EventId<CEV>;

    AllEvents events;
};
#define MAKE_EVENT_LIST(XXX, ...) struct XXX : EventListAll<__VA_ARGS__> {}

// *****************************************************************
// *****************************************************************
class StateBase;

namespace state_details {
    template<typename X>
    struct StateConceptCheck {
        static_assert(detail::has_type_details<X>::value,
                      "CTC: State struct has to have a member ::details specialized from StateDetails.");
        static_assert(detail::has_type_details_TransitionsT<X>::value,
                      "CTC: State struct has to have a member ::transitions of the type StateBase.");
        static_assert(std::is_base_of<StateBase, X>::value,
                      "CTC: State struct has to have a member ::details of the type StateBase.");

        static constexpr auto value = std::true_type();
    };
    template<>
    struct StateConceptCheck<void> {
        static constexpr auto value = std::true_type();
    };

    template<typename X, typename ...A>
    struct StateEnsureAllConcept {
        static_assert(StateConceptCheck<X>::value, "");
        static constexpr auto value = StateEnsureAllConcept<A...>::value;
    };
    template<typename X, typename ...A>
    struct StateEnsureAllConcept<Collector<X, A...>> {
        static constexpr auto value = StateEnsureAllConcept<X, A...>::value;
    };
    template<typename X>
    struct StateEnsureAllConcept<X> {
        static constexpr auto  value = StateConceptCheck<X>::value;
    };
}

struct TransitionBase { };

template<typename Event, typename State>
struct TransitionT : TransitionBase {
    typedef Event event;
    typedef State destState;
};
template<typename ...TransitionsT>
struct TransitionListT {
    typedef Collector<TransitionsT...> type;
};

struct TransitionImpl {
    uint16_t eventId;
    uint16_t stateId;
};

template<typename _ParentT, typename _TransitionsT>
struct StateDetails  {
    typedef _ParentT ParentT;
    typedef _TransitionsT TransitionsT;
};

// *****
// TO BE REMOVED
//#include <iostream>

class StateBase {
protected:
    // Disable copy and moveconstructors, states are not to be copied or moved
    StateBase(const char */*name*/, const char */*description*/) { }
    StateBase(const StateBase &) { }
    StateBase(StateBase &&) { }
    StateBase(const StateBase &&) { }
    StateBase& operator=(const StateBase&);      // Prevent assignment

public:
    virtual void internalTransition(uint16_t /*event*/, uint16_t /*sender*/) { }

    virtual void onEnter(uint16_t /*senderStateId*/, bool /*isDestState*/) { }
    virtual void onExit() { }
#ifdef __useNames
    virtual const char * name() { return ""; }
#endif
#ifdef __useDescription
    virtual const char * description() { return ""; }
#endif
};

//namespace sys_details {
//    namespace helper {
//        template <typename ...>
//        struct StatePtrList_impl;
//        template <typename ...STATE, typename ...STATEs, typename ...PLEs>
//        struct StatePtrList_impl<Collector<STATE, STATEs...>, Collector<PLEs...>> {
//        };
//    }
//
//}

#define StateSetup_wwww(STATECLASSNAME, DESCRIPTION) \
public: \
typedef STATECLASSNAME ThisState; \
void dummy() { static_assert(std::is_same<std::remove_reference<decltype(this)>::type, STATECLASSNAME*>::value, \
    "CTC: state_setup(): First argument has to be the type of the parent class."); } \
static ThisState *getInstance() { \
    static ThisState inst; \
    return &inst; } \
static StateBase *getInstanceBase() { \
    return getInstance(); } \
template<typename T> void internalTransition(T) { \
    std::cout << "No internalTransition" << std::endl; \
} \
private: \
STATECLASSNAME() : StateBase(#STATECLASSNAME, DESCRIPTION)

#ifdef __noNames
#define StateSetup(STATECLASSNAME, DESCRIPTION) \
public: \
typedef STATECLASSNAME ThisState; \
void dummy() { static_assert(std::is_same<std::remove_reference<decltype(this)>::type, STATECLASSNAME*>::value, \
    "CTC: state_setup(): First argument has to be the type of the parent class."); } \
using StateBase::internalTransition; \
STATECLASSNAME() : StateBase(#STATECLASSNAME, DESCRIPTION)
#endif

#if defined(__useNames) && !defined(__useDescription)
#define StateSetup(STATECLASSNAME, DESCRIPTION) \
public: \
typedef STATECLASSNAME ThisState; \
void dummy() { static_assert(std::is_same<std::remove_reference<decltype(this)>::type, STATECLASSNAME*>::value, \
    "CTC: state_setup(): First argument has to be the type of the parent class."); } \
const char *name() { return #STATECLASSNAME; } \
using StateBase::internalTransition; \
STATECLASSNAME() : StateBase(#STATECLASSNAME, DESCRIPTION)
#endif

#if defined(__useNames) && defined(__useDescription)
#define StateSetup(STATECLASSNAME, DESCRIPTION) \
public: \
typedef STATECLASSNAME ThisState; \
void dummy() { static_assert(std::is_same<std::remove_reference<decltype(this)>::type, STATECLASSNAME*>::value, \
    "CTC: state_setup(): First argument has to be the type of the parent class."); } \
const char *name() { return #STATECLASSNAME; } \
const char *description() { return DESCRIPTION; } \
using StateBase::internalTransition; \
STATECLASSNAME() : StateBase(#STATECLASSNAME, DESCRIPTION)
#endif

template<typename STATE>
struct MarkInitialState {
    typedef STATE type;
};

// *****************************************************************
// *****************************************************************


// **********************************************************************************
// **********************************************************************************
namespace sys_detail {
	struct EventBuffer {
		uint16_t event;
		uint16_t sender;
	};
}

#ifdef __make_treeuml
#include <QString>
#endif

class SystemBase {
#ifdef __make_treeuml
    template <typename SM, typename SYS> friend QString make_treeuml(SYS *sys);
#endif

public:
    static constexpr uint16_t ID_S_Undefined = 0xFFFF;

    uint16_t numberOfStates;
    uint16_t maxLevel;

protected:
    BAHA_Base *bahaBase;

    sys_detail::EventBuffer eventBuffer[1<<EVENT_BUFFER_SIZE_V];
    uint8_t eventBufferWritePos, eventBufferReadPos;

    StateBase **statesBP;
    uint8_t *stateFlags;
    uint16_t *stateListTemp;
    uint16_t *stateParents;
    uint16_t *stateLevels;
    uint8_t *eventOpts;

    uint16_t *transitionsNumberPerState;
    TransitionImpl **transitions;

    uint16_t timerNum;
    uint32_t *timerCounter;
    uint32_t *timerCounterRepeat;
    uint16_t *timerOwner;
    uint16_t *timerInitiator;
    uint16_t *timerEvents;

public:
    SystemBase(BAHA_Base *_baha);

    void processEvents();

    virtual void logEventName(uint16_t) { }
//    virtual void logStateName(uint16_t) { }

    BAHA_Base *bahaBaseGet() {
        return bahaBase;
    }

    uint16_t getParentIdBI(uint16_t cstate);

    virtual void logStateName(uint16_t id) {
#ifdef __useNames
        if(id==SystemBase::ID_S_Undefined)
            bahaBase->log("S_Undefined");
        else
            bahaBase->log(statesBP[id]->name());
#else
        bahaBase->log("S_");
        bahaBase->log(id);
#endif
    }

    void decreaseCounter();

protected:
    virtual bool checkEventProtection(sys_detail::EventBuffer &cevent, uint16_t cStateId);

    bool isStateActiveBI(uint16_t cstate);
    void isStateActiveSetBI(uint16_t cstate, bool v);
    bool isStateBlockedBI(uint16_t cstate);
    void isStateBlockedSetBI(uint16_t cstate, bool v);

//    uint16_t getParentIdBI(uint16_t cstate);

    uint16_t getStateLevelBI(uint16_t cstate);

    virtual StateBase *getStateById(uint16_t /*id*/) { return 0; }

    void raiseEventIdByIds(uint16_t eventId, uint16_t senderStateId);

    void executeTransition(uint16_t startState, uint16_t destState, uint16_t senderState, uint16_t event, bool blockActivatedStates);

    void activateStateFullByIds(uint16_t curStateId, uint16_t destStateId, uint16_t senderStateId, bool blockActivatedStates);
    void deactivateStateFull(uint16_t curStateId);

    void activateStateAndParentsByIds(uint16_t destState, uint16_t senderState, bool blockActivatedStates=true);

    void initialSetup();

    bool checkIfStateIsChildOfOrSame(uint16_t parentState, uint16_t childState);
};

// **********************************************************************************
namespace sys_detail {
    // *****
    template<typename STATE>
    struct StateRemoveInitialFlag {
        static_assert(state_details::StateConceptCheck<STATE>::value, "");
        typedef STATE type;
    };
    template<typename STATE>
    struct StateRemoveInitialFlag<MarkInitialState<STATE>> {
        typedef STATE type;
    };

    // *****
    template<typename ...>
    struct StatesGetInitial;
    template<typename STATE, typename ... STATEs>
    struct StatesGetInitial<Collector<MarkInitialState<STATE>, STATEs...>> {
        typedef STATE type;
    };
    template<typename STATE, typename ... STATEs>
    struct StatesGetInitial<Collector<STATE, STATEs...>> {
        typedef typename StatesGetInitial<Collector<STATEs...>>::type type;
    };
    template<>
    struct StatesGetInitial<Collector<>> {
        typedef void type;
    };

    // ***
    namespace helper {
        template<typename...>
        struct MaxT;
        template<uint16_t u, uint16_t v>
        struct MaxT<std::integral_constant<uint16_t, u>, std::integral_constant<uint16_t, v>> {
            typedef std::integral_constant<uint16_t, (u > v) ? u : v> type;
        };
    }
    template<typename ...>
    struct StateLevelImpl;
    template<typename STATE, uint16_t u>
    struct StateLevelImpl<STATE, std::integral_constant<uint16_t, u>> {
        typedef typename StateLevelImpl<typename STATE::details::ParentT, std::integral_constant<uint16_t, u+1>>::type type;
    };
    template<uint16_t u>
    struct StateLevelImpl<void, std::integral_constant<uint16_t, u>> {
        typedef std::integral_constant<uint16_t, u> type;
    };
    template<typename STATE>
    using StateLevel = StateLevelImpl<STATE, std::integral_constant<uint16_t, 0>>;

    template<typename ...> struct MaxStateLevel;
    template<typename STATE, typename ... STATEs>
    struct MaxStateLevel<STATE, STATEs...> {
        typedef typename helper::MaxT<typename StateLevel<STATE>::type, typename MaxStateLevel<STATEs...>::type>::type type;
    };
    template<> struct MaxStateLevel<> {
        typedef std::integral_constant<uint16_t, 0> type;
    };
    template<typename ...> struct MaxStateLevelC;
    template<typename ... STATEs>
    struct MaxStateLevelC<Collector<STATEs...>> {
        typedef typename MaxStateLevel<STATEs...>::type type;
    };

    // *****
    namespace helper {
        template<typename STATELIST, typename Transition> TransitionImpl makeTransition() {
            return TransitionImpl{ STATELIST::EventListT::template EventId<typename Transition::event>::value,
                        STATELIST::template StateId<typename Transition::destState>::value };
        }

        template<typename ...>
        struct TransitionList;
        template<typename STATELIST, typename ... TransitionsT>
        struct TransitionList<STATELIST, TransitionListT<TransitionsT...>> {
            typedef std::integral_constant<uint16_t, sizeof...(TransitionsT)> sizeT;
            TransitionImpl impl[sizeT::value];

            TransitionList() : impl { makeTransition<STATELIST, TransitionsT>()... } {
            }
        };

        template<typename STATELIST, typename STATE>
        struct StateListElement {
            STATE state;

            typedef TransitionList<STATELIST, typename STATE::details::TransitionsT> TransitionsT;
            TransitionsT transitions;
        };

        template<typename ...>
        struct StateListImpl;
        template<typename STATELIST, typename ... STATEs>
        struct StateListImpl<STATELIST, Collector<STATEs ...>> : StateListElement<STATELIST, STATEs> ... {
        };

//        template <typename ...>
//        struct StateId_Impl;
//        template <typename CSTATE, uint16_t IDX, typename STATE, typename ... STATEs>
//        struct StateId_Impl<CSTATE, std::integral_constant<uint16_t, IDX>, Collector<STATE, STATEs...>> {
//            typedef typename StateId_Impl<CSTATE, std::integral_constant<uint16_t, IDX+1>, Collector<STATEs...>>::type type;
//        };
//        template <typename CSTATE, uint16_t IDX, typename ... STATEs>
//        struct StateId_Impl<CSTATE, std::integral_constant<uint16_t, IDX>, Collector<CSTATE, STATEs...>> {
//            typedef typename std::integral_constant<uint16_t, IDX> type;
//        };
//        template <typename CSTATE, uint16_t IDX>
//        struct StateId_Impl<CSTATE, std::integral_constant<uint16_t, IDX>, Collector<>> {
//            typedef typename std::integral_constant<uint16_t, 0xFFFF> type;
//        };

        // *****
        struct StateList_Base { };
    }

    namespace helper {
        template<typename STATE> struct IsInitialState
        { typedef std::integral_constant<uint8_t, 0> type; };
        template<typename STATE> struct IsInitialState<MarkInitialState<STATE>>
        { typedef std::integral_constant<uint8_t, 1> type; };
    }

    template<typename ... >
    struct StateList;
    template<typename _EventListT, typename ... STATEs, typename ... STATESWINITIALFLAG>
    struct StateList<_EventListT, Collector<STATEs...>, Collector<STATESWINITIALFLAG...>> : helper::StateList_Base {
        typedef StateList<_EventListT, Collector<STATEs...>, Collector<STATESWINITIALFLAG...>> ThisT;

        typedef _EventListT EventListT;

        template<typename CSTATE>
        using StateId = typename helper::Id_Impl<CSTATE, std::integral_constant<uint16_t, 0>, Collector<STATEs...>>::type;

        typedef std::integral_constant<uint16_t, sizeof...(STATEs)> numberOfStatesT;

        helper::StateListImpl<ThisT, Collector<STATEs...>> states;

        StateBase *statesBP[numberOfStatesT::value];
        uint8_t stateFlags[numberOfStatesT::value];
        uint16_t stateParents[numberOfStatesT::value];

        uint16_t transitionsNumber[numberOfStatesT::value];
        TransitionImpl *transitions[numberOfStatesT::value];

        uint16_t stateLevels[numberOfStatesT::value];

        StateList() : states(), statesBP{ getBase<STATEs>() ... },
            stateFlags{ helper::IsInitialState<STATESWINITIALFLAG>::type::value ... },
            stateParents { getParent<STATEs>() ... },
            transitionsNumber { getTransitionsNumber<STATEs>() ... },
            transitions { getTransitions<STATEs>() ... },
            stateLevels { sys_detail::StateLevel<STATEs>::type::value ... } {
		}

        StateBase *getById(uint16_t id) {
            return statesBP[id];
        }
        template<typename STATE>
        STATE *get() {
            STATE &s = static_cast<helper::StateListElement<ThisT, STATE>*>(&states)->state;
            return &s;
        }
        template<typename STATE>
        StateBase *getBase() {
            return get<STATE>();
        }
        template<typename STATE>
        uint16_t getParent() {
            return StateId<typename STATE::details::ParentT>::type::value;
        }

        template<typename STATE>
        TransitionImpl *getTransitions() {
            TransitionImpl *t = static_cast<helper::StateListElement<ThisT, STATE>*>(&states)->transitions.impl;
            return t;
        }
        template<typename STATE>
        uint16_t getTransitionsNumber() {
            return helper::StateListElement<ThisT, STATE>::TransitionsT::sizeT::value;
        }

    };

}

// **********************************************************************************************************************
// **********************************************************************************************************************

namespace statemachine_detail {
    template<typename ... > struct CheckTransitionsEvent;
    template<typename TRANSITION>
    struct CheckTransitionsEvent<TRANSITION, void> {
        static constexpr bool value = true;
    };
    template<typename TRANSITION, typename EVENTLIST>
    struct CheckTransitionsEvent<TRANSITION, EVENTLIST> {
//        static_assert(std::is_base_of<event_details::EventListBase, EVENTLIST>::value,
//                      "EventList is wrong type.");

        static constexpr bool value = detail::is_one_of_collection<
                typename TRANSITION::event, typename EVENTLIST::type>::value;
        static_assert(value, "CTC: Transition event is not an event of the parent-system.");
    };

    template<typename ... > struct CheckTransitionsDest;
    template<typename TRANSITION>
    struct CheckTransitionsDest<TRANSITION, void> {
        static constexpr bool value = true;
    };
    template<typename TRANSITION, typename ... STATEs>
    struct CheckTransitionsDest<TRANSITION, Collector<STATEs ...>> {
        static constexpr bool value = detail::is_one_of_collection<
                typename TRANSITION::destState, Collector<STATEs ...>>::value;
        static_assert(value, "CTC: Transition destination is not a state of the parent-statemachine");
    };

    template<typename ... > struct CheckTransitions;
    template<typename TRANSITION, typename ... TRANSITIONs, typename STATELIST, typename EVENTLIST>
    struct CheckTransitions<Collector<TRANSITION, TRANSITIONs ...>, STATELIST, EVENTLIST> {
        static_assert(CheckTransitionsDest<TRANSITION, STATELIST>::value, "" );
        static_assert(CheckTransitionsEvent<TRANSITION, EVENTLIST>::value, "" );

        static constexpr bool value = CheckTransitions<Collector<TRANSITIONs ...>,
            STATELIST, EVENTLIST>::value;
    };
    template<typename STATELIST, typename EVENTLIST>
    struct CheckTransitions<Collector<>, STATELIST, EVENTLIST> {
        static constexpr bool value = true;
    };

    template<typename ... > struct CheckStateTransitions_Impl;
    template<typename STATE, typename ... STATEs, typename STATESLIST, typename EVENTLIST>
    struct CheckStateTransitions_Impl<Collector<STATE, STATEs...>, STATESLIST, EVENTLIST> {
        static_assert(CheckTransitions<typename STATE::details::TransitionsT::type,
                      STATESLIST, EVENTLIST>::value, "");
        static constexpr bool value = CheckStateTransitions_Impl<Collector<STATEs...>,
            STATESLIST, EVENTLIST>::value;
    };
    template<typename STATELIST, typename EVENTLIST>
    struct CheckStateTransitions_Impl<Collector<>, STATELIST, EVENTLIST> {
        static constexpr bool value = true;
    };

    template<typename ... > struct CheckStateTransitions_State;
    template<typename ... STATES_ALL, typename EVENTLIST>
    struct CheckStateTransitions_State<Collector<STATES_ALL...>, EVENTLIST> {
        static_assert(state_details::StateEnsureAllConcept<Collector<STATES_ALL...>>::value, "Class has to be used with states.");
        static constexpr bool value = CheckStateTransitions_Impl<Collector<STATES_ALL...>,
            Collector<STATES_ALL...>, EVENTLIST>::value;
    };
    template<typename ... > struct CheckStateTransitions_Events;
    template<typename ... STATES_ALL, typename EVENTLIST>
    struct CheckStateTransitions_Events<Collector<STATES_ALL...>, EVENTLIST> {
        static_assert(state_details::StateEnsureAllConcept<Collector<STATES_ALL...>>::value, "Class has to be used with states.");
        static constexpr bool value = CheckStateTransitions_Impl<Collector<STATES_ALL...>,
            void, EVENTLIST>::value;
    };
}

#ifdef __noNames
#define Make_StateMachine(SMName, ...) \
struct SMName : StateMachine<__VA_ARGS__> { }
#endif
#ifdef __useNames
#define Make_StateMachine(SMName, ...) \
    struct SMName : StateMachine<__VA_ARGS__> { static const char *name() { return #SMName; }}
#endif

template< typename ... STATEs>
struct StateMachine {
    typedef Collector<STATEs...> StatesWithInitialFlagT;
    typedef Collector<typename sys_detail::StateRemoveInitialFlag<STATEs>::type...> StatesT;
    typedef typename sys_detail::StatesGetInitial<Collector<STATEs...>>::type InitialStateT;

    static_assert(state_details::StateEnsureAllConcept<StatesT>::value, "CTC: Not all given states have a correct type");
    static_assert(!std::is_void<InitialStateT>::value, "CTC: No initial state defined.");

    static_assert(detail::no_duplicates_in_collection<StatesT>::value, "CTC: No duplicates are allowed in state list of state machine.");
    static_assert(statemachine_detail::CheckStateTransitions_State<StatesT, void>::value, "");
};


// *****
namespace sys_detail {
    struct SMTimer_Base { };
}

template<typename STATEList, typename EVENT=E_Timer, uint32_t DefaultTime=0>
struct SMTimer : sys_detail::SMTimer_Base {
    typedef STATEList TimerStates;
    typedef EVENT TimerEvent;
    static constexpr uint32_t value = DefaultTime;
};
template<typename EventListT, typename ... SMTIMERs>
struct SMTimerListTmpl {
    typedef Collector<SMTIMERs ...> TimersT;

    template<typename TIMER>
    using TimerId = typename sys_detail::helper::Id_Impl<TIMER, std::integral_constant<uint16_t, 0>, Collector<SMTIMERs...>>::type;

    static constexpr uint16_t size = sizeof...(SMTIMERs);
    uint32_t timerCounter[size];
    uint32_t timerCounterRepeat[size];
    uint16_t timerOwner[size];
    uint16_t timerInitiator[size];
    uint16_t timerEvents[size];

    template<typename TIMER, typename STATE>
    static void timerCheckSender() {
        static_assert(detail::is_one_of_collection<TIMER, TimersT>::value, "Timer has to be in TimerList.");
        static_assert(detail::is_one_of_collection<STATE, typename TIMER::TimerStates>::value, "Timer is not allowed for state.");
    }

    SMTimerListTmpl() : timerCounter {}, timerEvents { EventListT::template EventId<typename SMTIMERs::TimerEvent>::value ... }
    { }
};

// *****
//template< typename ... >
//class System;

template<typename ...>
class SMSystem;

//template<typename LALA, typename ...SMs>
template<typename BAHA_TT, typename EVL, typename ...SMs, typename _SMTimerList>
class SMSystem<BAHA_TT, EVL, Collector<SMs...>, _SMTimerList> : public SystemBase {

public:
    typedef BAHA_TT BAHA_T;

    typedef SMSystem<SMs...> This;

    typedef Collector<SMs...> SMsT;
    typedef EVL EventListT;

    typedef _SMTimerList SMTimerListT;

    static constexpr uint16_t ID_S_Undefined = SystemBase::ID_S_Undefined;

    // ***
    //typedef Collector<typename SMs::States ...> StatesT;
    typedef typename detail::JoinCollectors<typename SMs::StatesT ...>::type StatesT;
    typedef typename detail::JoinCollectors<typename SMs::StatesWithInitialFlagT ...>::type StatesWithInitialFlagT;
    typedef sys_detail::StateList<EventListT, StatesT, StatesWithInitialFlagT> StateListT;

    static_assert(detail::no_duplicates_in_collection<StatesT>::value, "CTC: No duplicates are allowed in state list of all state machine in the system.");

    static_assert(statemachine_detail::CheckStateTransitions_Events<StatesT, EventListT>::value, "");

    template<typename CSTATE>
    using StateId = typename StateListT::template StateId<CSTATE>;

    typedef std::integral_constant<uint16_t, StatesT::size> numberOfStatesT;

    typedef typename sys_detail::MaxStateLevelC<StatesT>::type MaxStateLevelC_T;

    virtual void logEventName(uint16_t id) {
#ifdef __useNames
        bahaBase->log(event_details::getEventName<EventListT>(id));
#else
        bahaBase->log("E_");
        bahaBase->log(id);
#endif
    }

protected:
    sys_detail::StateList<EventListT, StatesT, StatesWithInitialFlagT> statesImpl;
    uint16_t _stateListTemp[ MaxStateLevelC_T::value ];

    EventListT eventList;
    SMTimerListT smTimerList;

    BAHA_T *baha;

public:
    SMSystem(BAHA_T *_baha) : SystemBase(_baha), statesImpl() {
        bahaBase->sysSet(this);

        numberOfStates = numberOfStatesT::value;

        stateListTemp = _stateListTemp;

        statesBP = statesImpl.statesBP;
        stateFlags = statesImpl.stateFlags;
        stateParents = statesImpl.stateParents;
        stateLevels = statesImpl.stateLevels;
        eventOpts = eventList.events.eventOpts;

        maxLevel = MaxStateLevelC_T::value;

        transitionsNumberPerState = statesImpl.transitionsNumber;
        transitions = statesImpl.transitions;

        timerNum = SMTimerListT::size;
        timerCounter = smTimerList.timerCounter;
        timerCounterRepeat = smTimerList.timerCounterRepeat;
        timerEvents = smTimerList.timerEvents;
        timerInitiator = smTimerList.timerInitiator;
        timerOwner = smTimerList.timerOwner;

        initialSetup();
    }

    StateBase *getStateById(uint16_t id) { return statesImpl.getById(id); }
    template<typename STATE> STATE *getState() { return statesImpl.template get<STATE>(); }
    template<typename STATE> StateBase *getStateBase() { return statesImpl.template getBase<STATE>(); }

    template<typename STATE> bool isStateActive()
    { return isStateActiveBI(StateId<STATE>::value); }

    template<typename EVENT, typename STATE> void raiseEvent()
    { raiseEventIdByIds(EventListT::template EventId<EVENT>::value, StateId<STATE>::value); }
    template<typename EVENT> void raiseEvent()
    { raiseEventIdByIds(EventListT::template EventId<EVENT>::value, ID_S_Undefined); }

    template<typename TIMER, typename OWNER, typename INITIATOR=OWNER>
    void startTimer(bool repeat = true, uint32_t time=TIMER::value) {
        SMTimerListT::template timerCheckSender<TIMER, INITIATOR>();

        uint16_t id = SMTimerListT::template TimerId<TIMER>::value;
        smTimerList.timerCounter[id] = time+1;
        if(repeat)
            smTimerList.timerCounterRepeat[id] = smTimerList.timerCounter[id];
        else
            smTimerList.timerCounterRepeat[id] = 0;
        smTimerList.timerOwner[id] = StateId<OWNER>::value;
        smTimerList.timerInitiator[id] = StateId<INITIATOR>::value;
    }
    template<typename TIMER>
    void disableTimer() {
        uint16_t id = SMTimerListT::template TimerId<TIMER>::value;
        smTimerList.timerCounter[id] = 0;
        smTimerList.timerCounterRepeat[id] = 0;
    }
};



#ifdef __make_treeuml

#include <QString>
#include <QStringList>
#include <typeinfo>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QDir>
#include <iostream>

namespace helper {
    template <typename ...>
    struct StateLevelList;
    template <typename ... STATES>
    struct StateLevelList<Collector<STATES...>> {
        uint16_t stateLevel[sizeof...(STATES)];

        StateLevelList() : stateLevel { sys_detail::StateLevel<STATES>::type::value ... }
        { }
    };

    template <typename ...>
    struct StateList_States;
    template <typename SYS, typename ... STATEs>
    struct StateList_States<SYS, Collector<STATEs...>> {
        QVector<uint16_t> ids;

        StateList_States() : ids { SYS::StateListT::template StateId<STATEs>::type::value ... }
        { }
    };
}

template <typename SM, typename SYS>
QString make_treeuml(SYS *sys) {
#ifndef __useDescription
    static_assert(false, "define switch __useDescription has to be activated.");
#endif
    BAHA_Base *baha = sys->bahaBaseGet();

    QString stateUML[SYS::numberOfStatesT::value];
    QString out;

    helper::StateLevelList<typename SYS::StatesT> sll;

    helper::StateList_States<SYS, typename SM::StatesT> sls;

    for(uint8_t cl = sys_detail::MaxStateLevelC<typename SYS::StatesT>::type::value; cl > 0; cl--) {
        for(uint16_t cs=0; cs!=SYS::numberOfStatesT::value; cs++) {
            if((sll.stateLevel[cs] == cl) && (sls.ids.contains(cs))) {
                baha->logLine("Level: ", (uint8_t) cl, ": ", StateIdT{cs});

                uint16_t pid = sys->getParentIdBI(cs);

                QString nt = sys->getStateById(cs)->name();
                QString s = QString("state %1 {\n").arg(nt);
                QString cl;
                foreach(cl, stateUML[cs].split("\n", QString::SkipEmptyParts)) {
                    s += QString("  %1\n").arg(cl);
                }

                {
                    TransitionImpl *trs = sys->transitions[cs];
                    for(uint16_t ct=0; ct!=sys->transitionsNumberPerState[cs]; ct++)
                        if(sls.ids.contains(cs)) {
                            if(sys->checkIfStateIsChildOfOrSame(cs, trs[ct].stateId)) {
                                s += QString("[*]");
                            }
                            else {
                                s += QString("%1").arg(sys->getStateById(cs)->name());
                            }
                            s+= QString(" --> %1: %2\n")
                                    .arg(sys->getStateById(trs[ct].stateId)->name())
                                    .arg(event_details::getEventName<typename SYS::EventListT>(trs[ct].eventId));
                        }
                }

                s += "}\n";

                QString ds = sys->getStateById(cs)->description();
                QStringList sl = ds.split("\n", QString::SkipEmptyParts);
                foreach(cl, sl)
                    s += QString("%1 : %2\n").arg(nt).arg(cl);

                if(pid!=SYS::ID_S_Undefined)
                    stateUML[pid] += s;
                else
                    out += s;
            }
        }
    }

    out += QString("\n[*] --> %1\n").arg(sys->template getState<typename SM::InitialStateT>()->name());

//    for(uint16_t cs=0; cs!=SYS::numberOfStatesT::value; cs++) {
//        TransitionImpl *trs = sys->transitions[cs];
//        for(uint16_t ct=0; ct!=sys->transitionsNumberPerState[cs]; ct++)
//            if(sls.ids.contains(cs)) {
//                out += QString("%1 --> %2: %3\n").arg(sys->getStateById(cs)->name())
//                        .arg(sys->getStateById(trs[ct].stateId)->name())
//                        .arg(event_details::getEventName<typename SYS::EventListT>(trs[ct].eventId));
//            }
//    }

    std::cout << out.toStdString();

    return out;
}

namespace helper {

    template<typename ...>
    struct Make_Treeuml_allSMs_impl;
    template<typename SYS, typename SM, typename ... SMs>
    struct Make_Treeuml_allSMs_impl<SYS, Collector<SM, SMs...>> {
        QString operator()(SYS *sys, const char *path) {
            QString s = QString("@startuml\n\ntitle %1\n\n%2\n\n@enduml\n")
                    .arg(SM::name())
                    .arg(make_treeuml<SM>(sys));

            QFile file(QString("%1/%2.uml").arg(path).arg(SM::name()));
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return QString(" - ERROR for %1 - ").arg(SM::name());

            QTextStream out(&file);
            out << s;

            Make_Treeuml_allSMs_impl<SYS, Collector<SMs...>> mtu;
            return QString("%1.uml %2").arg(SM::name()).arg(mtu(sys, path));
        }
    };
    template<typename SYS>
    struct Make_Treeuml_allSMs_impl<SYS, Collector<>> {
        QString operator()(SYS */*sys*/, const char *) { return ""; }
    };
}

#include <iostream>

template <typename SYS>
void make_treeuml_allSMs(SYS *sys, const char *path) {
    helper::Make_Treeuml_allSMs_impl<SYS, typename SYS::SMsT> mtu;

    QDir::current().mkpath(path);

    QString pngs = mtu(sys, path);

    QString makefile = QString("PUML    = plantuml\nUMLS = %1\n\n"
                               "all: hello\n\n"
                               "hello:\n\t@echo \"building\" $(UMLS)\n"
                               "\t$(PUML) -tpng -progress $(UMLS)\n\t@echo\n"
                               "\t@echo \"done.\"\n\n"
                               "clean:\n\t@echo \"cleaning ...\"\n"
                               "\t@rm *.png 2>/dev/null\n\t@echo \"done.\"\n").arg(pngs);

    QFile file(QString("%1/makefile").arg(path));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << makefile;
}

#endif

#endif // SM2_H


