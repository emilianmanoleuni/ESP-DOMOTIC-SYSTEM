#include "wav.h"
#include <base64.h>

Wav::Wav(void){
    if(!LittleFS.begin(true)){ // if init fail enter in infinite loop
        Serial.println("LittleFS initialisation failed!");
        while(1) yield();
    }
    remove();
};

void Wav::remove(void){
    LittleFS.remove(FILENAME);
    LittleFS.remove(BODYNAME);
};

void Wav::writeBody(uint8_t* samplesBuff,size_t bytes_read){
    if (body) {
        body.write(samplesBuff, bytes_read);
    } else {
        Serial.println("Failed to open body file for writing");
    }
};

void Wav::bodyOpen(char* m){
  body = LittleFS.open(BODYNAME, m);
};

void Wav::bodyClose(void){
  body.flush();
  body.close();
};

void Wav::fileOpen(char* m){
  file = LittleFS.open(FILENAME, m);
};

void Wav::fileClose(void){
  file.flush();
  file.close();
};

void Wav::writeFile(void){

    fileOpen("w");
    bodyOpen("r");
    Serial.println("writing header");
    uint8_t header[HEADER_SIZE];
    wavHeader(header, body.size());
    file.write(header, HEADER_SIZE);

    Serial.println("writing body");
    size_t bufferSize = 1024;
    uint8_t buffer[bufferSize];
    // Read and write chunks of data until the end of the file
    while (size_t bytesRead = body.read(buffer, bufferSize)) {
        file.write(buffer, bytesRead);
    }
    fileClose();
    bodyClose();
};






void Wav::encodeFile(void){
  Serial.println("encode_file .wav");
  fileOpen("r");
  File encoding = LittleFS.open(ENCODED_FILENAME, "w");
  
  
  size_t bufferSize = 512;
  unsigned char buffer[bufferSize];
  size_t bytesRead;
  while(bytesRead = file.read(buffer, bufferSize)){

    String data = "";
    for (size_t i = 0; i < bytesRead; i++) {
      data += (char)buffer[i];
    }
    // Codifica i dati in Base64
    String encodedData = base64::encode(data);
    encodedData.replace("\n", ""); // Rimuovi il carattere di nuova riga
    encoding.print(encodedData);
  }

  encoding.close();
  fileClose();
}






void Wav::listLittleFS(void) {
    Serial.println(F("\r\nListing LittleFS files:"));
    static const char line[] PROGMEM =  "=================================================";

    Serial.println(FPSTR(line));
    Serial.println(F("  File name                              Size"));
    Serial.println(FPSTR(line));

    File root = LittleFS.open("/");
    if (!root) {
        Serial.println(F("Failed to open directory"));
        return;
    }
    if (!root.isDirectory()) {
        Serial.println(F("Not a directory"));
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("DIR : ");
            String fileName = file.name();
            Serial.print(fileName);
        } else {
            String fileName = file.name();
            Serial.print("  " + fileName);
            // File path can be 31 characters maximum in LittleFS
            int spaces = 33 - fileName.length(); // Tabulate nicely
            if (spaces < 1) spaces = 1;
            while (spaces--) Serial.print(" ");
            String fileSize = (String) file.size();
            spaces = 10 - fileSize.length(); // Tabulate nicely
            if (spaces < 1) spaces = 1;
            while (spaces--) Serial.print(" ");
            Serial.println(fileSize + " bytes");
        }
        file = root.openNextFile();
    }

    Serial.println(FPSTR(line));
    Serial.println();
    delay(1000);
};

void Wav::wavHeader(uint8_t* header, int wavSize){
    header[0] = 'R';
    header[1] = 'I';
    header[2] = 'F';
    header[3] = 'F';
    unsigned int fileSize = wavSize + HEADER_SIZE - 8;
    header[4] = (byte)(fileSize & 0xFF);
    header[5] = (byte)((fileSize >> 8) & 0xFF);
    header[6] = (byte)((fileSize >> 16) & 0xFF);
    header[7] = (byte)((fileSize >> 24) & 0xFF);
    header[8] = 'W';
    header[9] = 'A';
    header[10] = 'V';
    header[11] = 'E';
    header[12] = 'f';
    header[13] = 'm';
    header[14] = 't';
    header[15] = ' ';
    header[16] = 0x10;
    header[17] = 0x00;
    header[18] = 0x00;
    header[19] = 0x00;
    header[20] = 0x01;
    header[21] = 0x00;
    header[22] = 0x01;
    header[23] = 0x00;
    header[24] = 0x80;
    header[25] = 0x3E;
    header[26] = 0x00;
    header[27] = 0x00;
    header[28] = 0x00;
    header[29] = 0x7D;
    header[30] = 0x00;
    header[31] = 0x00;
    header[32] = 0x02;
    header[33] = 0x00;
    header[34] = 0x10;
    header[35] = 0x00;
    header[36] = 'd';
    header[37] = 'a';
    header[38] = 't';
    header[39] = 'a';
    header[40] = (byte)(wavSize & 0xFF);
    header[41] = (byte)((wavSize >> 8) & 0xFF);
    header[42] = (byte)((wavSize >> 16) & 0xFF);
    header[43] = (byte)((wavSize >> 24) & 0xFF);
    
}
