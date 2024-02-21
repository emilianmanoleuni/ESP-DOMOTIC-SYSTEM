#include <Wire.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "file.h"
#include "webserver.h"
#include "esp_uart.h"

using namespace std;

unsigned long previousMillis = 0;
const long interval = 5000; 

// Pin Rele
int Relay[] = { 33, 26, 27, 14, 12, 13};

//Pin Voltages DON'T use ADC2 with wifi
int PIN_12V = 34;
int PIN_5V = 32;
int PIN_PHOTOVOLTAIC = 33;

// Replace with your network credentials
const char* ssid = "iliadbox-10271E";
const char* password = "!Nicoletta22012002";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;

// Variables to save readings
float readed12V;
float readed5V;
float readedPhotovoltaic;
float temperature  = 0.0;
int light  = 0;

float read12V(){
  return analogRead(PIN_12V) * (12.47/4095.0);
}

float read5V(){
  return analogRead(PIN_5V) * (6.6/4095.0);
}

float readPhotovoltaic(){
  return analogRead(PIN_PHOTOVOLTAIC) * (3.6/4095.0);
}

void setup(){
  //Start log.txt file
  writeFile(LittleFS, "/log.txt", "LOG.TXT FILE ESP DOMOTIC SYSTEM\n--------------------------\n\n");

  //START UART con. with MSP
  UARTinit();                                                                   
  
  Serial.begin(115200);

  // Pin Rele
  for(int i=0; i<sizeof(Relay)/sizeof(int); i++){
    pinMode(Relay[i], OUTPUT);  
  }

  //Pin Led Server On
  pinMode(15, OUTPUT);
  
  //Pin Voltages
  pinMode(PIN_12V, INPUT);
  pinMode(PIN_5V, INPUT);
  pinMode(PIN_PHOTOVOLTAIC, INPUT);
  
  bool status; 
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LITTLEFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  digitalWrite(15, HIGH);
  Serial.println(WiFi.localIP());

  delay(1000);
  // Reset Rele
  for(int i = 0; i<sizeof(Relay)/sizeof(int); i++){
    Serial.print("Reset relay ");
    appendFile(LittleFS, "/log.txt", "Reset relay ");
    Serial.print(i);
    appendFile(LittleFS, "/log.txt", String((i)).c_str());
    Serial.print("\n");
    appendFile(LittleFS, "/log.txt", "\n");
    digitalWrite(Relay[i], LOW);  
    delay(1000);
  }

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  //timeClient.setTimeOffset(3600);

  appendFile(LittleFS, "/log.txt", "\n\n---------------\nNTPClient starter\n---------------\n\n");

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html");
    appendFile(LittleFS, "/log.txt", "Request index.html\n");
  });
  //Components
  server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/settings.html");
    appendFile(LittleFS, "/log.txt", "Request settings.html\n");
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/style.css", "text/css");
    appendFile(LittleFS, "/log.txt", "Request style.cssl\n");
  });
  server.on("/casa_grafica.png", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/casa_grafica.png", "image/png");
    appendFile(LittleFS, "/log.txt", "Request casa_grafica.png\n");
  });
  server.on("/background.jpg", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/background.jpg", "image/jpg");
    appendFile(LittleFS, "/log.txt", "Request background.jpg\n");
  });
  
  //Readings and Databases
  //Database
  server.on("/log.txt", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/log.txt", "text/txt");
    appendFile(LittleFS, "/log.txt", "Request log.txt\n");
  });
  server.on("/data12v.csv", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/data12v.csv", "text/csv");
    appendFile(LittleFS, "/log.txt", "Request data12v.csv\n");
  });
  server.on("/data5v.csv", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/data5v.csv", "text/csv");
    appendFile(LittleFS, "/log.txt", "Request data5v.csv\n");
  });
  server.on("/dataPhotovoltaic.csv", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/dataPhotovoltaic.csv", "text/csv");
    appendFile(LittleFS, "/log.txt", "Request dataPhotovoltaic.csv\n");
  });
  server.on("/dataTemperature.csv", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/dataTemperature.csv", "text/csv");
    appendFile(LittleFS, "/log.txt", "Request dataTemperature\n");
  });
  server.on("/dataLight.csv", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/dataLight.csv", "text/csv");
    appendFile(LittleFS, "/log.txt", "Request dataLight\n");
  });
  
  //Last single reading
  server.on("/lastData12v.txt", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/lastData12v.txt", "text/txt");
    appendFile(LittleFS, "/log.txt", "Request lastData12v.txt\n");
  });
  server.on("/lastData5v.txt", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/lastData5v.txt", "text/txt");
    appendFile(LittleFS, "/log.txt", "Request lastData5v.txt\n");
  });
  server.on("/lastPhotovoltaic.txt", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/lastPhotovoltaic.txt", "text/txt");
    appendFile(LittleFS, "/log.txt", "Request lastPhotovoltaic.txt\n");
  });
  server.on("/lastTemperature.txt", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/lastTemperature.txt", "text/txt");
    appendFile(LittleFS, "/log.txt", "Request lastTemperature.txt\n");
  });
  server.on("/lastLight.txt", HTTP_GET, [](AsyncWebServerRequest * request){
    request->send(LittleFS, "/lastLight.txt", "text/txt");
    appendFile(LittleFS, "/log.txt", "Request lastLight.txt\n");
  });

  

  //First write on LOG
  timeClient.forceUpdate();
  formattedDate = timeClient.getFormattedDate();
  appendFile(LittleFS, "/log.txt", "NTPC client force update\n");


  //Reset and write on Database
  readed12V = read12V();
  writeFile(LittleFS, "/data12v.csv", ("Time, Value\n"+formattedDate+",").c_str());
  appendFile(LittleFS, "/data12v.csv", (String(readed12V)+"\r").c_str());
  appendFile(LittleFS, "/log.txt", "Write data12v.csv first time.\n");

  readed5V = read5V();
  writeFile(LittleFS, "/data5v.csv", ("Time, Value\n"+formattedDate+",").c_str());
  appendFile(LittleFS, "/data5v.csv", (String(readed5V)+"\r").c_str());
  appendFile(LittleFS, "/log.txt", "Write data5v.csv first time.\n");

  readedPhotovoltaic = readPhotovoltaic();
  writeFile(LittleFS, "/dataPhotovoltaic.csv", ("Time, Value\n"+formattedDate+",").c_str());
  appendFile(LittleFS, "/dataPhotovoltaic.csv", (String(readedPhotovoltaic)+"\r").c_str());
  appendFile(LittleFS, "/log.txt", "Write dataPhotovoltaic.csv first time.\n");

  temperature = readTemperature();
  writeFile(LittleFS, "/dataTemperature.csv", ("Time, Value\n"+formattedDate+",").c_str());
  appendFile(LittleFS, "/dataTemperature.csv", (String(temperature)+"\r").c_str());
  appendFile(LittleFS, "/log.txt", "Write dataTemperature.csv first time.\n");
  
  light = readLight();
  writeFile(LittleFS, "/dataLight.csv", ("Time, Value\n"+formattedDate+",").c_str());
  appendFile(LittleFS, "/dataLight.csv", (String(light)+"\r").c_str());
  appendFile(LittleFS, "/log.txt", "Write dataLight.csv first time.\n");



  //Voltages //DEPRECATED no more client request
  server.on("/read12V", HTTP_GET, [](AsyncWebServerRequest *request){
    float readed12V = read12V();
    request->send(200, "text/plain", String(readed12V));
    Serial.print("Read 12v:");
    Serial.print(readed12V);
    Serial.print("\n");

    timeClient.forceUpdate();
    formattedDate = timeClient.getFormattedDate();

    //Database
    appendFile(LittleFS, "/data12v.csv", (formattedDate+",").c_str());
    appendFile(LittleFS, "/data12v.csv", (String(readed12V)+"\r").c_str());
    //LastRead
    writeFile(LittleFS, "/lastData12v.txt", (String(readed12V)).c_str());
  });
  
  //DEPRECATED no more client request
  server.on("/read5V", HTTP_GET, [](AsyncWebServerRequest *request){
    float readed5V = read5V();
    request->send(200, "text/plain", String(readed5V));
    Serial.print("Read 5v:");
    Serial.print(readed5V);
    Serial.print("\n");

    timeClient.forceUpdate();
    formattedDate = timeClient.getFormattedDate();

    //Database
    appendFile(LittleFS, "/data5v.csv", (formattedDate+",").c_str());
    appendFile(LittleFS, "/data5v.csv", (String(readed5V)+"\r").c_str());
    //LastRead
    writeFile(LittleFS, "/lastData5v.txt", (String(readed5V)).c_str());
  });




  //Rele 
  server.on("/relay1/on", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[0], HIGH);
    Serial.println("Relay 1 ON");
  });
  server.on("/relay1/off", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[0], LOW);
    Serial.println("Relay 1 OFF");
  }); 

  server.on("/relay2/on", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[1], HIGH);
    Serial.println("Relay 2 ON");
  });
  server.on("/relay2/off", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[1], LOW);
    Serial.println("Relay 2 OFF");
  }); 

  server.on("/relay3/on", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[2], HIGH);
    Serial.println("Relay 3 ON");
  });
  server.on("/relay3/off", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[2], LOW);
    Serial.println("Relay 3 OFF");
  }); 

  server.on("/relay4/on", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[3], HIGH);
    Serial.println("Relay 4 ON");
  });
  server.on("/relay4/off", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[3], LOW);
    Serial.println("Relay 4 OFF");
  }); 

  server.on("/relay5/on", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[4], HIGH);
    Serial.println("Relay 5 ON");
  });
  server.on("/relay5/off", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[4], LOW);
    Serial.println("Relay 5 OFF");
  }); 
  
  server.on("/relay6/on", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[5], HIGH);
    Serial.println("Relay 6 ON");
  });
  server.on("/relay6/off", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS,"/index.html");
    digitalWrite(Relay[5], LOW);
    Serial.println("Relay 6 OFF");
  });



    
  // Start server
  server.begin();
  
}
 
void loop(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    readed12V = read12V();
    readed5V = read5V();
    readedPhotovoltaic = readPhotovoltaic();
    temperature = readTemperature();
    light = readLight();
    
    timeClient.forceUpdate();
    formattedDate = timeClient.getFormattedDate();

    //Database 12V
    appendFile(LittleFS, "/data12v.csv", (formattedDate+",").c_str());
    appendFile(LittleFS, "/data12v.csv", (String(readed12V)+"\r").c_str());
    //LastRead
    writeFile(LittleFS, "/lastData12v.txt", (String(readed12V)).c_str());

    //Database 5V
    appendFile(LittleFS, "/data5v.csv", (formattedDate+",").c_str());
    appendFile(LittleFS, "/data5v.csv", (String(readed5V)+"\r").c_str());
    //LastRead
    writeFile(LittleFS, "/lastData5v.txt", (String(readed5V)).c_str());

    //Database Photovoltaic
    
    appendFile(LittleFS, "/dataPhotovoltaic.csv", (formattedDate+",").c_str());
    appendFile(LittleFS, "/dataPhotovoltaic.csv", (String(readedPhotovoltaic)+"\r").c_str());
    appendFile(LittleFS, "/log.txt", "Write dataPhotovoltaic.csv first time.\n");
    //LastRead
    writeFile(LittleFS, "/lastPhotovoltaic.txt", (String(readedPhotovoltaic)).c_str());

    //Database Temperature
    appendFile(LittleFS, "/dataTemperature.csv", (formattedDate+",").c_str());
    appendFile(LittleFS, "/dataTemperature.csv", (String(temperature)+"\r").c_str());
    //LastRead
    writeFile(LittleFS, "/lastTemperature.txt", (String(temperature)).c_str());

    //Database Light
    appendFile(LittleFS, "/dataLight.csv", (formattedDate+",").c_str());
    appendFile(LittleFS, "/dataLight.csv", (String(light)+"\r").c_str());
    //LastRead
    writeFile(LittleFS, "/lastLight.txt", (String(light)).c_str());
    
  }
}
