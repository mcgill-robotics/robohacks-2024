#line 1 "C:\\Users\\Vincent\\Documents\\GitHub\\robohacks-2024\\ESP32-AppConn\\include\\rhComms.h"
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

 public:
  int run();
  int sendUpdates();
  void setAutoSend(bool autoSend);
  void updateValue(char* category, char* parameter, float value);
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
  if (autoSend) {
    sendUpdates();
  }
  return read;
}

inline int CarController::connectClient() {
  client = server->available();
  if (client) return 0;
  return 1;  // no clients connected
}

inline int CarController::hasClient() { return client.connected(); }

inline void CarController::setAutoSend(bool autoSend) {
  CarController::autoSend = autoSend;
}

inline int CarController::sendUpdates() {
  if (client.availableForWrite()) {
    serializeJson(send_doc, client);
    return 0;
  }
  return 1;
}

inline void CarController::updateValue(char* category, char* parameter,
                                       float value) {
  send_doc[category][parameter] = value;
}

inline float CarController::getValue(char* category, char* parameter) {
  return rcv_doc[category][parameter];
}

inline void CarController::setServer(WiFiServer* server) {
  CarController::server = server;
}