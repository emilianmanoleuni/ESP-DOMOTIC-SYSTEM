#ifndef UART_UART_H_
#define UART_UART_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

#define UART_MAX_SIZE 255 //2^8

void sendData(uint_fast8_t data);
void sendInBlock(int value);
void sendTemperature();
void sendLight();

#endif
