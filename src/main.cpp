

#include "./common.h"
#include "./params.h"
#include "taskNTPD.h"
#include "driver/adc.h"              // For deep sleep
#include "SSD1306.h" 
#include <esp_wifi.h>
#include <esp_bt.h>
#include <WiFi.h>
#include <esp_task_wdt.h>            // Watchdog timer 


#define WDT_TIMEOUT 15               // in seconds (if reached before deep sleep starts this will restart the ESP32)
byte WIFI_CONNECT_TIMEOUT = 6;
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


void goToDeepSleep(){         
 Serial.println("Going to sleep now.");
 vTaskDelay(50);
 // Prepare before sleep
 Serial.println(millis());
 Serial.flush();
 // Create OLED instance "display" just to be able to switch it off from this task
 SSD1306 display2(0x3c, 21, 22);
 display2.displayOff();  // Switch off OLED
 vTaskDelay(50);
 WiFi.disconnect(true);
 WiFi.mode(WIFI_OFF);
 btStop();
 adc_power_off();
 esp_wifi_stop();
 esp_bt_controller_disable();
 esp_sleep_enable_timer_wakeup(LOG_INTERVAL_DURATION*1000000 - 1000*millis()); // set Deep sleep timer between measurements in uS
 esp_deep_sleep_start();  // Deep sleep here
 vTaskDelay(50);
 Serial.println("This is after deep sleep and will never be printed.");
}

void logToSDcard(){
Serial.print("START=");
    Serial.println(millis());

  Serial.println("Parameters will be logged to SD card");
    float paramValue = 0;
    String fullCSVlog = "";

    for (int i = 0; i < MAX_PARAM; i++) {     // loop through parameters
       if(i == 48 || i == 49){                // these do not need to be divided by 100
         paramValue = getParameter(i);
       }
       else {
         paramValue = getParameter(i)/100.0;
       }
       
      fullCSVlog = fullCSVlog + String(paramValue) + ",";
    }

  fullCSVlog.remove(fullCSVlog.length()-1);  // Remove last char (extra comma)
  fullCSVlog = fullCSVlog + "\n";
  Serial.println("ALL PARAMETERS LOG = ");
  Serial.println(fullCSVlog);
  

   SPI.begin(14, 2, 15);
    if(!SD.begin(13)){
        Serial.println("Card Mount Failed");
        goToDeepSleep();
    }
    
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    uint64_t usedSpace = SD.usedBytes() / (1024 * 1024);
    uint64_t freeSpace = cardSize - usedSpace;  
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    Serial.printf("Used space: %lluMB\n", usedSpace);
    Serial.printf("Free space: %lluMB\n", freeSpace);
  // listDir(SD, "/", 0);
    createDir(SD, "/BeeMoS_logs");
    createDir(SD, "/BeeMoS_logs/2022");
    createDir(SD, "/BeeMoS_logs/2022/raw");
    createDir(SD, "/BeeMoS_logs/2022/filtered");
    appendFile(SD, "/BeeMoS_logs/2022/raw/temp.log", (fullCSVlog).c_str());
   // deleteFile(SD, "/hello.txt");
   // readFile(SD, "/hello.txt");
    Serial.print("END=");
    Serial.println(millis());


}





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