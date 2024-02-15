#include <WiFi.h>
#include "include/rhComms.h"

const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

CarController cont;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);  // set the LED pin mode
  digitalWrite(2, LOW);
  Serial.println("Serial Started");

//  delay(1000);
  WiFi.softAP(ssid, password);
  Serial.println("SoftAP started");
  cont.setIPAddress(WiFi.softAPIP());
  Serial.println("ip address set");
  cont.setServer(&server);
  Serial.println("server set");
  Serial.println(WiFi.softAPIP());
  Serial.println("ip address got");
  server.begin();
  Serial.println("Server started");
}

void loop() {
  if (cont.hasClient()) {
    int read = cont.run();
    if (read > 0) {
      Serial.print("read ");
      Serial.println(read);
      digitalWrite(2, cont.getValue("controlType", "Keyboard"));
    } else if (read == -1) {
      Serial.println("Error reading");
    }
  } else {
    Serial.println("Waiting for client...");
    while (cont.connectClient()) {
    }
    Serial.println("Connected to client!");
  }
}
