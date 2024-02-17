#include "WiFi.h"

#include "include/rhComms.h"

#define LED_PIN 2
#define MOT1_DIR1_PIN 14
#define MOT1_DIR2_PIN 27
#define MOT2_DIR1_PIN 26
#define MOT2_DIR2_PIN 25
#define MOT1_PWM_PIN 32
#define MOT2_PWM_PIN 33

const char* ssid = "robohacks test";
const char* password = "1234";

CarController controller;

WiFiServer server(80);

void resetAll() {
  digitalWrite(LED_PIN, LOW);
  // reset other peripherals
}

void cMot1(){
  digitalWrite(MOT1_DIR1_PIN, LOW);
  digitalWrite(MOT1_DIR2_PIN, HIGH);
}

void ccMot1(){
  digitalWrite(MOT1_DIR1_PIN, HIGH);
  digitalWrite(MOT1_DIR2_PIN, LOW);
}

void offMot1(){
  digitalWrite(MOT1_DIR1_PIN, HIGH);
  digitalWrite(MOT1_DIR2_PIN, HIGH);
}

void cMot2(){
  digitalWrite(MOT2_DIR1_PIN, LOW);
  digitalWrite(MOT2_DIR2_PIN, HIGH);
}

void ccMot2(){
  digitalWrite(MOT2_DIR1_PIN, HIGH);
  digitalWrite(MOT2_DIR2_PIN, LOW);
}

void offMot2(){
  digitalWrite(MOT2_DIR1_PIN, HIGH);
  digitalWrite(MOT2_DIR2_PIN, HIGH);
}

void speedMot1(float speed){
  int analogVal = abs(speed) * 255;
  analogWrite(MOT1_PWM_PIN, analogVal);
  if(speed < 0){
    ccMot1();
  }else{
    cMot1();
  }
}

void speedMot2(float speed){
  int analogVal = abs(speed) * 255;
  analogWrite(MOT2_PWM_PIN, analogVal);
  if(speed < 0){
    cMot2();
  }else{
    ccMot2();
  }
}

// setup function sets up the ESP32 for communication with the server
void setup() {
  // initialize serial communication
  Serial.begin(115200);

  // set ESP32 LED pin to LOW
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(MOT1_DIR1_PIN, OUTPUT);
  pinMode(MOT1_DIR2_PIN, OUTPUT);
  pinMode(MOT2_DIR1_PIN, OUTPUT);
  pinMode(MOT2_DIR2_PIN, OUTPUT);
  pinMode(MOT1_PWM_PIN, OUTPUT);
  pinMode(MOT2_PWM_PIN, OUTPUT);

  offMot1();
  offMot2();

  delay(1000);

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

int up_key = 0;
int down_key = 0;
int left_key = 0;
int right_key = 0;

float speed1 = 0.0;
float speed2 = 0.0;

void loop() {
  if (controller.hasClient()) {
    int read = controller.run();
    if (read == -1) {
      Serial.println("Error reading");
    } else {
      int controlType = controller.getValue("controlType", "Keyboard");
      digitalWrite(LED_PIN, controlType);
      if(controlType == 1){
        up_key = controller.getValue("keys", "UP");
        down_key = controller.getValue("keys", "DOWN");
        left_key = controller.getValue("keys", "LEFT");
        right_key = controller.getValue("keys", "RIGHT");

        if(up_key == 1 && down_key == 0){
          speed1 = 1.0;
          speed2 = 1.0;
        }else if(up_key == 0 && down_key == 1){
          speed1 = -1.0;
          speed2 = -1.0;
        }else{
          speed1 = 0.0;
          speed2 = 0.0;
        }

        if(left_key == 1 && right_key == 0){
          speed2 = speed2 * 0.5;
        }else if(left_key == 0 && right_key == 1){
          speed1 = speed1 * 0.5;
        }

        speedMot1(speed1);
        speedMot2(speed2);
      }
      // Add more functionalities to other values
    }
  } else {
    Serial.println("Waiting for client...");
    while (controller.connectClient()) {
    }
    Serial.println("Connected to client!");
  }
  delay(50);
}
