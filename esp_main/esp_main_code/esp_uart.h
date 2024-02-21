#ifndef __ESP_UART_H__
#define __ESP_UART_H__

#include <HardwareSerial.h>

void UARTinit();
int  getData();
void sendInterrupt(int data);
float readTemperature();
int readLight();

#endif
