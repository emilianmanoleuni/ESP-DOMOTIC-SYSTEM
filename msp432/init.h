#ifndef INIT_H_
#define INIT_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "uart/uart.h"
#include "init.h"
#include "temperature/HAL_TMP006.h"
#include "i2c/HAL_I2C.h"
#include "timer/timer.h"

void UART_init();
void hw_init();
void temp_init();
void light_init();

void init();

#endif
