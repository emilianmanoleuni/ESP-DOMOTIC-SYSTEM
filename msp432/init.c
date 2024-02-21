#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "uart/uart.h"
#include "init.h"
#include "temperature/HAL_TMP006.h"
#include "light/HAL_OPT3001.h"
#include "i2c/HAL_I2C.h"
#include "timer/timer.h"

//clock = 24MHz, baudrate = 115200 (http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html)
const eUSCI_UART_ConfigV1 UARTConfig = {
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,                     //clock source
    13,
    0,
    37,
    EUSCI_A_UART_NO_PARITY,                             //don't use the parity bit
    EUSCI_A_UART_LSB_FIRST,                             //less significant bit first (normal way to communicate)
    EUSCI_A_UART_ONE_STOP_BIT,                          //use one stop bit
    EUSCI_A_UART_MODE,                                  //default value for UART mode
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,      //oversampling or low frequency baudrate generation
    EUSCI_A_UART_8_BIT_LEN                              //indicates character length
};

void UART_init(){
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    UART_initModule(EUSCI_A2_BASE, &UARTConfig);            //pins 3.2 (receiver) and 3.3 (transmitter)
    UART_enableModule(EUSCI_A2_BASE);

}

void temp_init(){
    Init_I2C_GPIO();
    I2C_init();
    TMP006_init();
}

void light_init(){
    OPT3001_init();
}

void hw_init(){
    WDT_A_holdTimer();
    Interrupt_enableMaster();                               //if disabled, only a NMI can interrupt the processor

    PCM_setCoreVoltageLevel(PCM_VCORE1);

    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);

    Interrupt_enableInterrupt(INT_EUSCIA2);
    Interrupt_enableInterrupt(INT_TA0_N);
    Interrupt_enableSleepOnIsrExit();

    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);         //24MHz
    CS_setReferenceOscillatorFrequency(CS_REFO_32KHZ);       //32kHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); //24Mhz used for I2C and UART
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1); //32kHz used for the timer
}

void init(){
    hw_init();
    UART_init();
    temp_init();
    light_init();
    timer_init();
}


