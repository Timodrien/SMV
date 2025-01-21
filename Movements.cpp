#include "Movements.h"
#include <Arduino.h>
#include <Adafruit_PCF8574.h>
#include <Wire.h>

Adafruit_PCF8574 pcf8574;

void initMotor() {
  Wire.begin(13, 14);

  pcf8574.begin(0x26);
  
  pcf8574.pinMode(0, OUTPUT);  
	pcf8574.pinMode(1, OUTPUT);
	pcf8574.pinMode(2, OUTPUT);
  pcf8574.pinMode(3, OUTPUT);
	pcf8574.pinMode(4, OUTPUT);
	pcf8574.pinMode(5, OUTPUT);
  pcf8574.pinMode(6, OUTPUT);
	pcf8574.pinMode(7, OUTPUT);
}


void Vor(){
//Rechte Seite
  //Motor VL
  pcf8574.digitalWrite(0, LOW);  // Motor 1 beginnt zu rotieren
  pcf8574.digitalWrite(1, HIGH);
  //Motor HL
  pcf8574.digitalWrite(2, LOW);  // Motor 2 beginnt zu rotieren
  pcf8574.digitalWrite(3, HIGH);
//Linke Seite
  //Motor VR
  pcf8574.digitalWrite(4, HIGH);  // Motor 3 beginnt zu rotieren
  pcf8574.digitalWrite(5, LOW);
  //Motor HR
  pcf8574.digitalWrite(6, HIGH);  // Motor 4 beginnt zu rotieren
  pcf8574.digitalWrite(7, LOW);
}

void Rueck(){
//Rechte Seite
  //Motor VL
  pcf8574.digitalWrite(0, HIGH);  // Motor 1 beginnt zu rotieren
  pcf8574.digitalWrite(1, LOW);
  //Motor HL
  pcf8574.digitalWrite(2, HIGH);  // Motor 2 beginnt zu rotieren
  pcf8574.digitalWrite(3, LOW);
//Linke Seite
  //Motor VR
  pcf8574.digitalWrite(4, LOW);  // Motor 3 beginnt zu rotieren
  pcf8574.digitalWrite(5, HIGH);
  //Motor HR
  pcf8574.digitalWrite(6, LOW);  // Motor 4 beginnt zu rotieren
  pcf8574.digitalWrite(7, HIGH);
}

void Links(){
//Rechte Seite
  //Motor VL
  pcf8574.digitalWrite(0, LOW);  // Motor 1 beginnt zu rotieren
  pcf8574.digitalWrite(1, HIGH);
  //Motor HL
  pcf8574.digitalWrite(2, LOW);  // Motor 2 beginnt zu rotieren
  pcf8574.digitalWrite(3, HIGH);
//Linke Seite
  //Motor VR
  pcf8574.digitalWrite(4, LOW);  // Motor 3 beginnt zu rotieren
  pcf8574.digitalWrite(5, HIGH);
  //Motor HR
  pcf8574.digitalWrite(6, LOW);  // Motor 4 beginnt zu rotieren
  pcf8574.digitalWrite(7, HIGH);
}

void Rechts(){
//Rechte Seite
  //Motor VL
  pcf8574.digitalWrite(0, HIGH);  // Motor 1 beginnt zu rotieren
  pcf8574.digitalWrite(1, LOW);
  //Motor HL
  pcf8574.digitalWrite(2, HIGH);  // Motor 2 beginnt zu rotieren
  pcf8574.digitalWrite(3, LOW);
//Linke Seite
  //Motor VR
  pcf8574.digitalWrite(4, HIGH);  // Motor 3 beginnt zu rotieren
  pcf8574.digitalWrite(5, LOW);
  //Motor HR
  pcf8574.digitalWrite(6, HIGH);  // Motor 4 beginnt zu rotieren
  pcf8574.digitalWrite(7, LOW);
}

void Stop(){
//Rechte Seite
  //Motor VL
  pcf8574.digitalWrite(0, LOW);  // Motor 1 beginnt zu rotieren
  pcf8574.digitalWrite(1, LOW);
  //Motor HL
  pcf8574.digitalWrite(2, LOW);  // Motor 2 beginnt zu rotieren
  pcf8574.digitalWrite(3, LOW);
//Linke Seite
  //Motor VR
  pcf8574.digitalWrite(4, LOW);  // Motor 3 beginnt zu rotieren
  pcf8574.digitalWrite(5, LOW);
  //Motor HR
  pcf8574.digitalWrite(6, LOW);  // Motor 4 beginnt zu rotieren
  pcf8574.digitalWrite(7, LOW);
}