#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

#include "uart/uart.h"
#include "init.h"
#include "temperature/HAL_TMP006.h"

uint8_t RXData = 0;

void main(void){
    init();

    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);

    while(1){
        PCM_gotoLPM0();
    }
}

void EUSCIA2_IRQHandler(void){ //interrupting when the receiver gets something
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG){
        RXData = UART_receiveData(EUSCI_A2_BASE);
    }
}

void TA0_N_IRQHandler(void){ //timer interrupt handler
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    sendTemperature();
    sendLight();
}


