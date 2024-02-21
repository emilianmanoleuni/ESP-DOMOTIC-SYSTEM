#ifndef __ESP_UART_H__
#define __ESP_UART_H__

#include <HardwareSerial.h>
//#include "fan.h"

void UARTinit();
int  getData();
void handleData();
void sendInterrupt(int data);
float readTemperature();
int readLight();

#endif
