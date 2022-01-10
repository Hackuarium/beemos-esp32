

#include "./common.h"
#include "./params.h"
#include "taskNTPD.h"
#include "driver/adc.h"              // For deep sleep
#include <esp_task_wdt.h>            // Watchdog timer 
#define WDT_TIMEOUT 15               // in seconds (if reached before deep sleep starts this will restart the ESP32)
byte WIFI_CONNECT_TIMEOUT = 10;
int32_t LOG_INTERVAL_DURATION = 60;    // The deep sleep period in seconds will be LOG_INTERVAL_DURATION - millis()/1000 

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
void taskRotaryButton();
void taskAHT21();

void setup() {
  
  setCpuFrequencyMhz(80);                  // to reduce power consumption
  Serial.begin(115200);                    // only for debug purpose
  adc_power_on();                          // After wakeup from deepsleep
  esp_task_wdt_init(WDT_TIMEOUT, true);    // Set watchdog timer
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
  
  taskRotaryButton();
  // taskAHT21();
  
}

void loop() {
  esp_task_wdt_add(NULL); //add current thread to WDT watch
  vTaskDelay(10000);
}