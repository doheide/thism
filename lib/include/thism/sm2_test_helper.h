#ifndef SM2_TEST_HELPER_H
#define SM2_TEST_HELPER_H

#include <thism/sm2.h>

template<typename TestSys>
struct SMSys_Test : public TestSys {
    SMSys_Test (const TestSys &osys) : TestSys(osys) {
    }

    void clearAllStateFlags() {
        for(uint16_t i=0; i != this->numberOfStates; (this->stateFlags[i++] = 0));
    }

    void t_isStateActiveSetBI(uint16_t cstate, bool v) {
        this->isStateActiveSetBI(cstate, v);
    }
    void t_isStateBlockedSetBI(uint16_t cstate, bool v) {
        this->isStateBlockedSetBI(cstate, v);
    }
    bool t_isStateActiveBI(uint16_t cstate) {
        return this->isStateActiveBI(cstate);
    }

    void t_activateStateFullByIds(uint16_t curStateId, uint16_t destStateId, uint16_t senderStateId, bool blockActivatedStates) {
        this->activateStateFullByIds(curStateId, destStateId, senderStateId, blockActivatedStates);
    }

    void t_activateStateAndParentsByIds(uint16_t destState, uint16_t senderState, bool blockActivatedStates=true) {
        this->activateStateAndParentsByIds(destState, senderState, blockActivatedStates);
    }

    void clearMessageBuffer() {
        this->eventBufferReadPos = 0;
        this->eventBufferWritePos = 0;
    }
    uint8_t eventBufferReadPosGet() {
        return this->eventBufferReadPos;
    }
    uint8_t eventBufferWritePosGet() {
        return this->eventBufferWritePos;
    }

    uint16_t timerNumGet() { return timerNum; }
    uint32_t *timerCounterGet() { return timerCounter; }
    uint32_t *timerCounterRepeatGet() { return timerCounterRepeat; }
    uint16_t *timerOwnerGet() { return timerOwner; }
    uint16_t *timerInitiatorGet() { return timerInitiator; }
    uint16_t *timerEventsGet() { return timerEvents; }
};


#endif // SM2_TEST_HELPER_H
