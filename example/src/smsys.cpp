
#include <thism/sm2.h>
#include <smsys.h>


void S_Tack::onEnter(uint16_t, bool) {
    smsys->template startTimer<SMT_Std, ThisState>(10);
}
void S_Tick::onEnter(uint16_t, bool) {
    smsys->template startTimer<SMT_Std, ThisState>(10);
}
