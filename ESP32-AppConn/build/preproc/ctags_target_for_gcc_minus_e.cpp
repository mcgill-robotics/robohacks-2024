# 1 "C:\\Users\\Vincent\\Documents\\GitHub\\robohacks-2024\\ESP32-AppConn\\ESP32-AppConn.ino"
# 2 "C:\\Users\\Vincent\\Documents\\GitHub\\robohacks-2024\\ESP32-AppConn\\ESP32-AppConn.ino" 2

# 4 "C:\\Users\\Vincent\\Documents\\GitHub\\robohacks-2024\\ESP32-AppConn\\ESP32-AppConn.ino" 2

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

CarController cont;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(2, 0x03); // set the LED pin mode
  digitalWrite(2, 0x0);

  //  delay(1000);
  WiFi.softAP(ssid, password);
  cont.setServer(&server);
  Serial.print("IP Address of ESP32: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
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
