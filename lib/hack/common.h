#include <Arduino.h>
// SD card
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <esp_task_wdt.h>            // Watchdog timer 
// DS3231 RTC clock
#include <Wire.h>
#include <DS3231.h>


// #define THR_WIRE_MASTER 1

#define LED_BUILTIN 5

#define MAX_PARAM 52
extern int16_t parameters[MAX_PARAM];


#define PARAM_ERROR                51

#define PARAM_TEMPERATURE           0 // A
#define PARAM_HUMIDITY              1
#define PARAM_TEMPERATURE_EXT       2
#define PARAM_TEMPERATURE_EXT2      3
#define PARAM_TEMPERATURE_AHT21     4
#define PARAM_HUMIDITY_AHT21        5
         
#define PARAM_BATTERY_LEVEL        48 // Raw NCR18650b Li-ion battery level, larger is better (non linear)
#define PARAM_WIFI_RSSI            49 // AX  Wifi RSSI, larger is better

extern byte MENU_NUMBER; 
extern byte WIFI_CONNECT_TIMEOUT; 
extern int32_t LOG_INTERVAL_DURATION;  //in seconds 
extern SemaphoreHandle_t mutex;



void goToDeepSleep();
void logToSDcard();
String getLogNow();

// SD card primary functions
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
void appendFile(fs::FS &fs, const char * path, const char * message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);

