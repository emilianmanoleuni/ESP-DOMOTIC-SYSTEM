#ifndef __CLIENTSPEECHTEXT__
#define __CLIENTSPEECHTEXT__

#include <WiFi.h>
#include <HTTPClient.h>


class CloudSpeechClient {
private:
  HTTPClient client;
public:
  CloudSpeechClient();
  ~CloudSpeechClient();

  int sendFile(const char* filename); 

};

#endif 
