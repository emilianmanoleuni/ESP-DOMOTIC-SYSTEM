#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

#include "uart.h"
#include "../temperature/HAL_TMP006.h"
#include "../light/HAL_OPT3001.h"

void sendData(uint_fast8_t data){
    UART_transmitData(EUSCI_A2_BASE, data);
}

void sendTemperature(){
    sendData(0);                //temperature start sequence

    int itemp = (int) TMP006_getTemp()*10;

    if(itemp==0){
        sendData(itemp);
        return;                 //no need to send also the stop bit, it would create confusion
    }

    sendInBlock(itemp);

    sendData(0);                //temperature stop sequence
}

void sendLight(){
    sendData(0);                // light start sequence

    int ilight = OPT3001_getLux();

    if(ilight==0){
        sendData(ilight);
        return;
    }

    sendInBlock(ilight);

    sendData(0);                //temperature stop sequence
}

void sendInBlock(int value){

    if(value<255){
        sendData(value);        //if it's minor to 255, send it with 8 bit
    } else {
        while(value>=255){
            sendData(255);      //keep sending 255 until the number can be sent with only 8 bit (<255)
            value-=255;
        }
    }
    if(value>0){
        sendData(value);        //send the remaining only if it's > 0
    }
}


