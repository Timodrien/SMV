#include "Sensors.h"
#include <Arduino.h>

unsigned long previousMillis = 0;
const long interval = 10000; //10 seconds

float measuredVoltage, batteryVoltage, batteryPercentage;
int analogValue;

void initSensors() {

}

void readSensorData() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    analogValue = analogRead(32);
    measuredVoltage = (analogValue / 4095.0) * 3.3;
    batteryVoltage = measuredVoltage * 2.0;
    batteryPercentage = ((batteryVoltage - 6.0) / (6.6 - 6.0)) * 100;
    if (batteryPercentage < 0) {
        } else if (batteryPercentage > 100) {
      batteryPercentage = 100;
    }
    String message = String(batteryPercentage); 
    webSocket.broadcastTXT(message);
  }
}
