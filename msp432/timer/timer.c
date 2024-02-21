#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

#include "timer.h"

const Timer_A_ContinuousModeConfig contModeConfig = {
    TIMER_A_CLOCKSOURCE_ACLK,           //initialized at 32kHz
    TIMER_A_CLOCKSOURCE_DIVIDER_14,     //32kHz/14=2.2kHz (every 30s ca.)
    TIMER_A_TAIE_INTERRUPT_ENABLE,      //overflows when hits 0xFFFF and send an interrupt
    TIMER_A_DO_CLEAR                    //clear counter
};

void timer_init(){
    Timer_A_configureContinuousMode(TIMER_A0_BASE, &contModeConfig);
}
