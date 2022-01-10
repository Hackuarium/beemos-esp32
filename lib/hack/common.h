#include <Arduino.h>

// #define THR_WIRE_MASTER 1

#define LED_BUILTIN 5

#define MAX_PARAM 52
extern int16_t parameters[MAX_PARAM];

#define MAX_LED                 256

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