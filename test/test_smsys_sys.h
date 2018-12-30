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


#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <thism/sm2.h>
#include <thism/sm2_test_helper.h>

#include <base_sys/base_test_sys.h>


using namespace testing;

TEST(thism, thism) {
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

struct StateDeAndActivationTest : public ::testing::Test {
    BAHA_TYPE baha;
    SMSys sys;

    SMSys_Test<SMSys> smsTester;

    StateDeAndActivationTest() : sys(&baha), smsTester(sys) {
        sys.init();
    }

    void clear() {
        smsTester.clearAllStateFlags();
        for(uint16_t i=0; i!=sys.numberOfStates; i++)
            ASSERT_EQ(smsTester.t_isStateActiveBI(i), false) << "All State Flags were cleared, but state " << i << " is active.";
    }
protected:
  void SetUp() override {
      smsys = &sys;
  }
  void TearDown() override {}
};

TEST_F(StateDeAndActivationTest, ActivateState) {
    typedef S_TickInner StateToTest;

    clear();

    smsTester.t_isStateActiveSetBI(SMSys::StateId<StateToTest>::value, true);

    for(uint16_t i=0; i!=sys.numberOfStates; i++)
        if(SMSys::StateId<StateToTest>::value==i)
            EXPECT_EQ(smsTester.t_isStateActiveBI(i), true) << "All State Flags were cleared, but state " << i << " is not active.";
        else
            EXPECT_EQ(smsTester.t_isStateActiveBI(i), false) << "All State Flags were cleared, but state " << i << " is active.";
}

TEST_F(StateDeAndActivationTest, ActivateStateFull) {

    // activate state and parents
    // this means: destState is not curState
    // -> state is active
    // -> msg-queue is empty
    clear(); smsTester.clearMessageBuffer();

    smsTester.t_activateStateFullByIds( SMSys::StateId<S_TickInner>::value, SMSys::StateId<S_TickInnerInner>::value,
                                        SMSys::StateId<S_Tick>::value, false);

    EXPECT_EQ(smsTester.eventBufferReadPosGet(), 0) << "eventBufferReadPos should not change";
    EXPECT_EQ(smsTester.eventBufferWritePosGet(), 0) << "Event was raised but should not.";

    for(uint16_t i=0; i!=sys.numberOfStates; i++)
        if(SMSys::StateId<S_TickInner>::value==i)
            EXPECT_EQ(smsTester.t_isStateActiveBI(i), true) << "All State Flags were cleared, but state " << i << " is not active.";
        else
            EXPECT_EQ(smsTester.t_isStateActiveBI(i), false) << "All State Flags were cleared, but state " << i << " is active.";

    // activate state that does not have active
    // this means: destState is not curState
    // -> state is active
    // -> msg-queue is empty
    clear(); smsTester.clearMessageBuffer();

    smsTester.t_activateStateFullByIds( SMSys::StateId<S_TickInnerInner>::value, SMSys::StateId<S_TickInnerInner>::value,
                                        SMSys::StateId<S_Tick>::value, false);

    EXPECT_EQ(smsTester.eventBufferReadPosGet(), 0) << "eventBufferReadPos should not change";
    EXPECT_EQ(smsTester.eventBufferWritePosGet(), 1) << "Event was raised but should not.";

    for(uint16_t i=0; i!=sys.numberOfStates; i++)
        if(SMSys::StateId<S_TickInnerInner>::value==i)
            EXPECT_EQ(smsTester.t_isStateActiveBI(i), true) << "test activateStateFullByIds(): state " << i << " is not active but should.";
        else
            EXPECT_EQ(smsTester.t_isStateActiveBI(i), false) << "test activateStateFullByIds(): state " << i << " is active but should not.";

}

TEST_F(StateDeAndActivationTest, DISABLED_ActivateStateAndParents) {
    /// @todo check activation order

    // activate state whose child state is also to be activated
    // this means: destState is not curState
    // -> state is active
    // -> msg-queue is empty
    clear(); smsTester.clearMessageBuffer();

    smsTester.t_activateStateAndParentsByIds( SMSys::StateId<S_TickInner>::value, SMSys::StateId<S_Tick>::value);

    EXPECT_EQ(smsTester.eventBufferReadPosGet(), 0) << "eventBufferReadPos should not change";
    EXPECT_EQ(smsTester.eventBufferWritePosGet(), 0) << "Event was raised but should not.";

    for(uint16_t i=0; i!=sys.numberOfStates; i++)
        if( (SMSys::StateId<S_TickInner>::value==i) || (SMSys::StateId<S_Tick>::value==i))
            EXPECT_EQ(smsTester.t_isStateActiveBI(i), true) << "All State Flags were cleared, but state " << i << " is active.";
        else
            EXPECT_EQ(smsTester.t_isStateActiveBI(i), false) << "All State Flags were cleared, but state " << i << " is active.";
}

// ***
TEST_F(StateDeAndActivationTest, DISABLED_DeactivateState) {

}

TEST_F(StateDeAndActivationTest, DISABLED_DeactivateStateFull) {

}

TEST_F(StateDeAndActivationTest, DISABLED_DeactivateStateAndParents) {

}

// ***
TEST_F(StateDeAndActivationTest, DISABLED_Transition) {

}


// **************************************************************
struct EventBufferTest : public ::testing::Test {

};

TEST_F(EventBufferTest, DISABLED_RaiseEvent) {

}


// **************************************************************
struct TimerTest : public StateDeAndActivationTest {
};

TEST_F(TimerTest, CheckSingleShot) {
    ASSERT_THAT(smsTester.timerNumGet(), Eq(1));

    smsys->template startTimer<SMT_Std, S_Tick>(true);

}

TEST_F(TimerTest, DisableTimerOnExit) {
}

