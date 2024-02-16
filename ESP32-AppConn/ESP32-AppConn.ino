#include <WiFi.h>

#include "include/rhComms.h"

#define LED_PIN 2

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

CarController controller;

WiFiServer server(80);

void resetAll() {
  digitalWrite(LED_PIN, LOW);
  // reset other peripherals
}

// setup function sets up the ESP32 for communication with the server
void setup() {
  // initialize serial communication
  Serial.begin(115200);

  // set ESP32 LED pin to LOW
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  delay(1000);

  // start the wifi access point with the specified name and password
  WiFi.softAP(ssid, password);

  // set the CarController object's server reference to the server object
  cont.setServer(&server);

  // print the IP address of the ESP32 to the serial monitor
  Serial.print("IP Address of ESP32: ");
  Serial.println(WiFi.softAPIP());

  // start the server on port 80
  server.begin();
}

void loop() {
  if (cont.hasClient()) {
    int read = controller.run();
    if (read == -1) {
      Serial.println("Error reading");
    } else {
      digitalWrite(LED_PIN, controller.getValue("controlType", "Keyboard"));
      // Add more functionalities to other values
    }
  } else {
    Serial.println("Waiting for client...");
    while (cont.connectClient()) {
    }
    Serial.println("Connected to client!");
  }
}
