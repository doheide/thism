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

#include <thism/sm2.h>


void LogBase::log(uint8_t n) {
    logNumberImpl(n, 2);
}

void LogBase::log(uint16_t n) {
    logNumberImpl(n, 4);
}

void LogBase::log(uint32_t n) {
    logNumberImpl(n, 8);
}

void LogBase::log(EventIdT id) {
    baha->sysBaseGet()->logEventName(id.id);
}

void LogBase::log(StateIdT id) {
    baha->sysBaseGet()->logStateName(id.id);
}

void LogBase::logTime() {
    log(baha->sysBaseGet()->sysTimeGet());
}


void LogBase::logNumberImpl(uint32_t n, uint8_t digits) {
    char temp[digits+1];
    uint8_t cwp = digits;
    temp[cwp--] = 0;
    while(cwp!=255) {
        uint8_t cv = n & 15;
        if(cv<10)
            temp[cwp--] = '0'+cv;
        else
            temp[cwp--] = 'A'+cv-10;
        n >>= 4;
    }
    log(temp);
}


SystemBase::SystemBase(BAHA_TYPE *_baha) : baha(_baha), logf(baha), eventBuffer{} {
    eventBufferReadPos = 0;
    eventBufferWritePos = 0;
#ifdef DO_SIMULATION
    sicaba = 0;
#endif
}

void SystemBase::processEvents() {
    //BAHABase->logLine("processEvents(): ");

    // @todo add special treatment for E_Initial -> only process for sending state
    //smSystem_helper::TransitionBufferSys tr;

    uint8_t readUntil = eventBufferWritePos;
    for(; eventBufferReadPos != readUntil; eventBufferReadPos=(eventBufferReadPos+1)&((1<<EVENT_BUFFER_SIZE_V)-1)) {
    //for(uint8_t i=eventBufferReadPos; i != readUntil; i=(i+1)&((1<<EVENT_BUFFER_SIZE_V)-1)) {
        sys_detail::EventBuffer &cevent = eventBuffer[eventBufferReadPos];

        logf.logLine("!! EB ", EventIdT{cevent.event}, " | ", StateIdT{cevent.sender});

        for(uint16_t level = maxLevel; level!=0; level--)
            for(uint16_t csi=0; csi!=numberOfStates; csi++)
                if(isStateActiveBI(csi) && (stateLevels[csi]==level)) {
                    //BAHABase->logLine("active + level: ", (uint16_t) level, " ", StateIdT{csi}, " ", (uint8_t)csi);

                    for(uint16_t tii=0; tii!=transitionsNumberPerState[csi]; tii++) {
                        TransitionImpl *tics = &(transitions[csi][tii]);

                        if(cevent.event == tics->eventId) {
                            if(checkEventProtection(cevent, csi)) {
                                StateBase *sb = getStateById(csi);

                                uint16_t &cs = tics->stateId;
                                //bahaBase->logLine("!! T1 ", EventIdT{tics->eventId}, ": ", StateIdT{csi}, " -> ", StateIdT{cs});

                                executeTransition(csi, cs, cevent.sender, cevent.event, true);

                                if(isStateActiveBI(csi))
                                    sb->internalTransition(cevent.event, cevent.sender);
                            }
                        }
                    }
        }

//        for(; eventBufferReadPos != readUntil; eventBufferReadPos=(eventBufferReadPos+1)&((1<<EVENT_BUFFER_SIZE_V)-1)) {
//            sys_detail::EventBuffer &cevent = eventBuffer[i];

//            for(uint16_t csi=0; csi!=numberOfStates; csi++)

//        }
/*        for(int i=0;i!=transitionsImpl_getSize(); i++) {
             transtionGet(i, tr);

            if(cevent.event == tr.event)
                //                    if( getStateActive_ById(transitionsImpl.startStatesGet(i)) ) {
                if( getStateBase(tr.startState)->isActiveGet() ) {
                    //uint16_t startState = transitionsImpl.startStatesGet(i);
                    if(eventCheckSpecial(cevent.event, tr.startState, cevent.sender))
                        executeTransition(tr.startState, tr.destState,
                                          cevent.sender, cevent.event, true);
                }
        } */
    }

/*    for(; eventBufferReadPos != readUntil; eventBufferReadPos=(eventBufferReadPos+1)&((1<<EVENT_BUFFER_SIZE_V)-1)) {
        EventBuffer &cevent = eventBuffer[eventBufferReadPos];

        for(uint16_t i=1; i!=numStatesAll+1; i++)
            //                if( getStateActive_ById(i) ) {
            if( getStateBase(i)->isActiveGet() ) {
                //smSystem_helper::TransitionBuffer tr = transtionGet(i);
                transtionGet(i, tr);
                //uint16_t startState = transitionsImpl.startStatesGet(i);

                if(eventCheckSpecial(cevent.event, tr.startState, cevent.sender)) {
                    getStateBase(i-1)->onEvent(cevent.event, cevent.sender, this);
                    //states.statesB[i-1]->onEvent(cevent.event, cevent.sender, this);
                }
            }

        //            typedef iterateOverStatesWithCB<This> tempT;
        //            tempT ioswcb(cevent.event, cevent.sender, this);
        //            ForEachInCollector<tempT, statesWithEventCB> feic;
        //            feic(ioswcb);
    }

    for(uint16_t i=1; i!=numStatesAll+1; i++)
        //            isStateBlocked[i] = 0;
        getStateBase(i)->isBlockedSet(false); */
}

bool SystemBase::checkEventProtection(sys_detail::EventBuffer &cevent, uint16_t cStateId) {
//    if((cevent.event==ID_E_Initial) && (cevent.sender!=cStateId))
//        return false;
//    else if((cevent.event==ID_E_Timer) && (cevent.sender!=cStateId))
//        return false;
//    return true;

    if((eventOpts[cevent.event] & EOPT_ONLY_FROM_SELF) && (cevent.sender==cStateId))
        return true;
    if(eventOpts[cevent.event] & EOPT_ONLY_FROM_SELF_OR_PARENT) {
        uint16_t cn = cStateId;
        while((cn=getParentIdBI(cn)) != ID_S_Undefined)
            if(cevent.sender==cn)
                return true;
    }
    if(eventOpts[cevent.event] & EOPT_ONLY_FROM_SELF_OR_PARENT_OR_CHILD) {
        uint16_t cn = cevent.sender;
        while((cn=getParentIdBI(cn)) != ID_S_Undefined)
            if(cevent.sender==cStateId)
                return true;
    }
    return false;
}

bool SystemBase::isStateActiveBI(uint16_t cstate) {
    if(cstate>=SystemBase::ID_S_Undefined) return 0;
    return (stateFlags[cstate]&1)==1;
}
void SystemBase::isStateActiveSetBI(uint16_t cstate, bool v) {
    if(cstate>=SystemBase::ID_S_Undefined) return; // @todo raise fatal error
    if(v) stateFlags[cstate] |= 1;
    else  stateFlags[cstate] &= ~1;
}
bool SystemBase::isStateBlockedBI(uint16_t cstate) {
    if(cstate>=SystemBase::ID_S_Undefined) return 0;
    return (stateFlags[cstate]&1)==2;
}
void SystemBase::isStateBlockedSetBI(uint16_t cstate, bool v) {
    if(cstate>=SystemBase::ID_S_Undefined) return; // @todo raise fatal error
    if(v) stateFlags[cstate] |= 2;
    else  stateFlags[cstate] &= ~2;
}

uint16_t SystemBase::getParentIdBI(uint16_t cstate) {
    if(cstate >= numberOfStates) return ID_S_Undefined;
    return stateParents[cstate];
}

void SystemBase::sysTickCallback() {
    if(baha->pauseSysTickGet())
        return;

    sysTime++;

    for(uint16_t i=0; i!=timerNum; i++) {
        if(timerCounter[i]==1) {
            raiseEventIdByIds(timerEvents[i], timerInitiator[i]);
            timerCounter = timerCounterRepeat;
        }
        else if(timerCounter[i]>0)
           timerCounter[i]--;
    }
}

void SystemBase::raiseEventIdByIds(uint16_t eventId, uint16_t senderStateId) {
    logf.logLine("!! R ", EventIdT{eventId}, " | ", StateIdT{senderStateId});

#ifdef DO_SIMULATION
    if(sicaba)
        sicaba->onRaiseEvent(eventId);
#endif

    eventBuffer[eventBufferWritePos++] = { eventId, senderStateId };
    eventBufferWritePos &= (1<<EVENT_BUFFER_SIZE_V) - 1;
}

void SystemBase::executeTransition(uint16_t startState, uint16_t destState, uint16_t senderState, uint16_t event, bool blockActivatedStates) {
    if(!isStateActiveBI(startState) || isStateActiveBI(destState) ) {
        // @todo Add error message

        raiseEventIdByIds(ID_E_FatalError, senderState);
    }

    logf.logLine("!! T ", EventIdT{event}, ": ", StateIdT{startState}, " -> ", StateIdT{destState}, " | ", StateIdT{senderState});

    uint16_t temp, i;
    uint16_t commonState;
    uint16_t lastActiveChild;

    // find common state
    commonState = destState;
    while( ((temp=getParentIdBI(commonState))!=SystemBase::ID_S_Undefined) && (!isStateActiveBI(commonState)) )
        commonState = temp;

    // find last active child state
    lastActiveChild = startState;
    do {
        for(i=0; i!=numberOfStates; i++) {
            if( (getParentIdBI(i) == lastActiveChild) && isStateActiveBI(i) ) {
                lastActiveChild = i;
                break;
            }
        }
    } while(i!=numberOfStates);

    // make list of states to disable
    while( (lastActiveChild!=0) && (lastActiveChild!=commonState) ) {
        deactivateStateFullById(lastActiveChild);
        lastActiveChild = getParentIdBI(lastActiveChild);
    }

    // activate from common state to destState
    activateStateAndParentsByIds(destState, senderState, blockActivatedStates);
}

void SystemBase::activateStateFullByIds(uint16_t curStateId, uint16_t destStateId, uint16_t senderStateId, bool blockActivatedStates) {

    logf.logLine("!! EN ", StateIdT{curStateId});

#ifdef DO_SIMULATION
    if(sicaba)
        sicaba->onActivateState(curStateId);
#endif

    isStateActiveSetBI(curStateId, true);
    isStateBlockedSetBI(curStateId, blockActivatedStates);

    bool isDestState = curStateId == destStateId;

    getStateById(curStateId)->onEnter(senderStateId, isDestState);

    if(isDestState)
        raiseEventIdByIds(ID_E_Initial, curStateId);
}

void SystemBase::deactivateStateFullById(uint16_t curStateId) {

    logf.logLine("!! EX ", StateIdT{curStateId});

#ifdef DO_SIMULATION
    if(sicaba)
        sicaba->onDeactivateState(curStateId);
#endif

    getStateById(curStateId)->onExit();
    isStateActiveSetBI(curStateId, false);

    for(uint16_t i=0; i!=timerNum; i++) {
        if(timerOwner[i] == curStateId)
            timerCounter[i] = 0;
    }
}

void SystemBase::activateStateAndParentsByIds(uint16_t destState, uint16_t senderState, bool blockActivatedStates) {
    int counter = 0;
    uint16_t cstate = destState;

    // make list of all parent state
    do {
        stateListTemp[counter++] = cstate;
        cstate = getParentIdBI(cstate);
    } while((cstate!=SystemBase::ID_S_Undefined) && !isStateActiveBI(cstate));

    // activate all states top down
    while(counter>0) {
        counter--;
        activateStateFullByIds(stateListTemp[counter], destState, senderState, blockActivatedStates);
    }
}

void SystemBase::initialSetup() {
    for(uint16_t i=0; i!=numberOfStates; i++)
        if(isStateActiveBI(i))
            activateStateAndParentsByIds(i, SystemBase::ID_S_Undefined, false);
}

bool SystemBase::checkIfStateIsChildOfOrSame(uint16_t parentState, uint16_t childState)
{
    uint16_t cState = childState;
    while (cState != SystemBase::ID_S_Undefined) {
        if(cState == parentState)
            return true;
        cState = stateParents[cState];
    }
    return false;
}


