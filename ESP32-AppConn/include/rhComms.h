#include <WiFi.h>

#include "ArduinoJson-v7.0.3.h"

class CarController {
 private:
  WiFiServer* server;
  const char* ssid;
  const char* password;
  JsonDocument rcv_doc;
  WiFiClient client;

 public:
  int run();
  int hasClient();
  int connectClient();
  float getValue(char* category, char* parameter);
  void setServer(WiFiServer* server);
};

inline int CarController::run() {
  //
  int read = 0;
  while (client.available()) {
    read += 1;
    DeserializationError error = deserializeJson(rcv_doc, client);
    if (error) {
      // put error handling
      client.stop();
      return -1;
    }
  }
  return read;
}

inline int CarController::connectClient() {
  client = server->available();
  if (client) return 0;
  return 1;  // no clients connected
}

inline int CarController::hasClient() { return client.connected(); }

inline float CarController::getValue(char* category, char* parameter) {
  return rcv_doc[category][parameter];
}

inline void CarController::setServer(WiFiServer* server) {
  CarController::server = server;
}