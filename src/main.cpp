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

void setup() {
  Serial.begin(115200);  // only for debug purpose
  setupParameters();
  if (getParameter(PARAM_CURRENT_PROGRAM)==2) {
    setParameter(PARAM_CURRENT_PROGRAM, 5);
  }
  taskSerial();
  taskWebserver();
  taskNTPD();
  taskOTA();
  taskMDNS();
  taskWifi();
//  taskMQTT();
  taskBlink();
}

void loop() {
  vTaskDelay(10000);
}