#include <Arduino.h>
#include <HardwareSerial.h>
#include "ClientSpeechText.h"
#include "INMP441.h"
#include "wav.h"


#define BUTTON_PIN 18       // button pin, pull-up
#define DEBOUNCE_DELAY 50   // delay for anti debouncing precedure
#define BUFFER_LEN  1024*2  // lenght of buffers, dma ecc.
#define RX_PIN 16           // serial pins
#define TX_PIN 17

// microphone variables
byte samplesBuff[BUFFER_LEN];
int32_t flash_wr_size;
size_t bytes_read;  

// button variables
int lastButtonState = HIGH;  
unsigned long lastDebounceTime = 0;  

// declaration microphone object
INMP441 mic;




void setup() {
  Serial.begin(115200);
  // input button internal pull-up
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // microphone init
  mic.init();
  
}

void loop() {
  
  while(bool buttonState = debounceButton());
  Serial.println("Button pressed");

  // start recording and save file
  Serial.println("\n new record");
  delay(500);
  Wav* wav = new Wav();
  flash_wr_size = 0;
  Serial.print("arrivo");
  
  mic.reset();

  Serial.println(" *** Recording Start *** ");
  wav->bodyOpen("w");
  while (flash_wr_size < FLASH_RECORD_SIZE) {
    i2s_read(I2S_PORT, &samplesBuff, BUFFER_LEN, &bytes_read, portMAX_DELAY);
    wav->writeBody(samplesBuff, bytes_read);
    flash_wr_size += bytes_read;  
    ets_printf("Sound recording %u%%\n", flash_wr_size * 100 / FLASH_RECORD_SIZE);
  }
  wav->bodyClose();
  wav->writeFile();
  delay(1000);
  wav->listLittleFS();

  //wav.encodeFile();
  //wav.listLittleFS();

  // send file to server
  CloudSpeechClient* cloudspeechclient = new CloudSpeechClient();
  int message = cloudspeechclient->sendFile(FILENAME);

  Serial2.println(message);
  
  delete cloudspeechclient;
  delete wav;

  
  delay(5000);
}



bool debounceButton() {
 
  int buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState != lastButtonState) lastDebounceTime = millis();
  

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) buttonState = lastButtonState;

  lastButtonState = buttonState;
  return buttonState;
}
