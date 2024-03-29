#include <stdio.h>
#include <HardwareSerial.h>
#include "esp_uart.h"

extern float temperature;
extern int light;

HardwareSerial SerialPort(2); 

void UARTinit(){
	SerialPort.begin(115200, SERIAL_8N1, 16, 17); //baudrate=115200, 8bit message length with 1 stop bit, 16 receiver pin, 17 transmitter pin
}

int  getData(){
	while(!SerialPort.available()); //trap the MCU until a data is available in the UART channel
	return SerialPort.read();
}

void sendWithInterrupt(int data){
	SerialPort.write(data);
}

float readTemperature(){
    temperature = 0;
    getData();                 //waiting for start bit
    while(1){
        int data = getData();
        if(data==0) break;     //two cases: or the temperature is 0, or the sending is over
        temperature+=data;
    }
    temperature=(float) (temperature)/10.0;
    return temperature;
}

int readLight(){
    light = 0;
    getData();                 //waiting for start bit
    while(1){
    	int data = getData();
    	if(data==0) break;
    	light+=data;
    }
    return light;
}
