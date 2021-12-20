

#include "./common.h"
#include "./params.h"
#include "taskNTPD.h"

void taskBlink();
void taskSerial();
void taskWebserver();
void taskNTPD();
void taskOTA();
void taskMDNS();
void taskWifi();
void taskMQTT();
void taskSI7021();
void taskOneWire();
void taskOLEDscreen();
void taskBatteryLevel();

void setup() {
  Serial.begin(115200);  // only for debug purpose
  setupParameters();
  taskSerial();
  taskWebserver();
  taskNTPD();
  taskSI7021();
  taskOneWire();
  taskOLEDscreen();
  taskOTA();
  taskMDNS();
  taskWifi();
  taskMQTT();
  taskBatteryLevel();
  taskBlink();
}

void loop() {
  vTaskDelay(10000);
}