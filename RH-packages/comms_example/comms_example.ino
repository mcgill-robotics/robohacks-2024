#include <ArduinoJson-v7.0.3.h>
#include <rhComms.h>
#include "WiFi.h"

const char* ssid = "robohacks test";
const char* password = "1234";

CarController controller;

WiFiServer server(80);

void setup(){
  // initialize serial communication
  Serial.begin(115200);

  // set ESP32 LED pin to LOW
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  // start the wifi access point with the specified name and password
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  // set the CarController object's server reference to the server object
  controller.setServer(&server);

  // print the IP address of the ESP32 to the serial monitor
  Serial.print("IP Address of ESP32: ");
  Serial.println(WiFi.softAPIP());

  // start the server on port 80
  server.begin();
}

void loop(){

  if (controller.hasClient()) { //
    int read = controller.run();
    if (read == -1) {
      Serial.println("Error reading");
    } else {
      digitalWrite(2, controller.getValue("controlType", "Keyboard"));
    }
  } else {
  Serial.println("Waiting for client...");
  while (controller.connectClient()) {}
  Serial.println("Connected to client!");
  }
  delay(50);
}
