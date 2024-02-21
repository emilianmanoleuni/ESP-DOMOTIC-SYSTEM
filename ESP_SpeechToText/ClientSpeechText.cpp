#include "ClientSpeechText.h"
#include "NetworkParams.h"
#include <ArduinoJson.h>
#include <LittleFS.h> 



CloudSpeechClient::CloudSpeechClient(){
  WiFi.begin(ssid, password);
  Serial.println("Connetting to wifi");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to wifi");

  
  if(!LittleFS.begin(true)){ // if init fail enter in infinite loop
      Serial.println("LittleFS initialisation failed!");
      while(1) yield();
  }

  client.setTimeout(60000); // Tempo in millisecondi (60 secondi)
 

};



CloudSpeechClient::~CloudSpeechClient(){
  client.end();
  WiFi.disconnect();
};




int CloudSpeechClient::sendFile(const char* filename){
  // open file
  File file = LittleFS.open(filename, "r");
  if (!file) {
      Serial.println("Errore nell'apertura del file");
      return 0;
  }
  
  client.begin(server);
  client.addHeader("Content-Type", "audio/wav");
  int httpResponseCode = client.sendRequest("POST", &file, file.size());
  
  int message;
  if(httpResponseCode == 200){
    String response = client.getString(); // Read the response as a string
    Serial.println("Trascrizione audio: " + response);
  
    // Parse the JSON response
    StaticJsonDocument<200> doc; // Adjust the size based on your JSON response
    DeserializationError error = deserializeJson(doc, response);
  
 
    // Extract the integer value from the parsed JSON object
    message = doc["message"];
    Serial.print("Message: ");
    Serial.println(message);
  }




  //close file
  file.flush();
  file.close();

  return message;
}

  
  
  

  



    


  
