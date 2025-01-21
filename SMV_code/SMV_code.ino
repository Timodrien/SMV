// Libaries and define
#include "Movements.h"
#include "Sensors.h"
#include "CameraSetup.h"
#include <Wire.h>
#include <esp_camera.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <WebSocketsServer.h>
#include <ESP32Servo.h>
int ValHead = 180;
Servo ServoHead;
String lastCommand = "";

// Communication define
const char* ssid = "ProjectSMV";
const char* password = "12345678";
WebSocketsServer webSocket = WebSocketsServer(81);

// Communication receive
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_TEXT) {
        String command = String((char*)payload);
        Serial.printf("%s\n", command.c_str());

      if (command != lastCommand) {
          lastCommand = command;
          
        if (command == "Stop") {
          Stop();
        } else if (command == "Vor") {
            Vor();
        } else if (command == "Rueck") {
            Rueck();
        } else if (command == "Links") {
            Links();
        } else if (command == "Rechts") {
            Rechts();
        } 
        else {
        int commandValue = command.toInt();
        if (commandValue >= 1 && commandValue <= 180) {
            ServoHead.write(commandValue);
        }
      }
      }
        if (command == "Stop") {
          Stop();
        } 
  }
}

void setup() {
// Main setup
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  ServoHead.attach(12);
  ServoHead.write(180);

// Communication setup
  WiFi.mode(WIFI_AP);
  WiFi.softAPdisconnect(true);
  delay(100);
  WiFi.softAP(ssid, password);
  delay(1000);
  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

// Camera setup int
  initCamera();

// Motoren init
  initMotor();

void loop() {
// Send Data
  readSensorData();

// Send videostream
  webSocket.loop();
    camera_fb_t* fb = capturePicture();
    if (fb) {
      webSocket.broadcastBIN(fb->buf, fb->len);
      esp_camera_fb_return(fb);
    }
    delay(100);
}
