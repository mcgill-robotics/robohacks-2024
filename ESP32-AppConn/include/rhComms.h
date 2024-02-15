#include <WiFi.h>

#include "ArduinoJson-v7.0.3.h"

class CarController {
 private:
  WiFiServer* server;
  const char* ssid;
  const char* password;
  JsonDocument rcv_doc;
  JsonDocument send_doc;
  WiFiClient client;
  bool autoSend = false;
  IPAddress ip_address;

 public:
  int run();
  int initialize(const char* ssid, const char* password, WiFiServer* server);
  int sendUpdates();
  void setAutoSend(bool autoSend);
  void updateValue(char* category, char* parameter, float value);
  int hasClient();
  int connectClient();
  float getValue(char* category, char* parameter);
  const char* getIPAddress();
  void setIPAddress(IPAddress ip_address);
  void setServer(WiFiServer* server);
};