#include "../include/rhComms.h"

int CarController::run() {
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

int CarController::initialize(const char* ssid, const char* password,
                              WiFiServer* server) {
  // CarController::ssid = ssid;
  // CarController::password = password;
  // CarController::server = server;
  // // bool result = WiFi.softAP(ssid, password);
  // if (result) {
  //   ip_address = WiFi.softAPIP();
  //   server->begin();
  //   return 0;
  // } else {
  //   return 1;
  // }
  return 0;
}

int CarController::connectClient() {
  client = server->available();
  if (client) return 0;
  return 1;  // no clients connected
}

int CarController::hasClient() { return client.connected(); }

void CarController::setAutoSend(bool autoSend) {
  CarController::autoSend = autoSend;
}

int CarController::sendUpdates() {
  if (client.availableForWrite()) {
    serializeJson(send_doc, client);
    return 0;
  }
  return 1;
}

void CarController::updateValue(char* category, char* parameter, float value) {
  send_doc[category][parameter] = value;
}

float CarController::getValue(char* category, char* parameter) {
  return rcv_doc[category][parameter];
}

const char* CarController::getIPAddress() {
  return ip_address.toString().c_str();
}

void CarController::setIPAddress(IPAddress ip_address) {
  CarController::ip_address = ip_address;
}

void CarController::setServer(WiFiServer* server) {
  CarController::server = server;
}