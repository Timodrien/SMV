#ifndef Sensors_H
#define Sensors_H
#include <WebSocketsServer.h>

extern WebSocketsServer webSocket;

void initSensors();
void readSensorData();

#endif // Sensors_H