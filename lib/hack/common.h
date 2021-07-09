#include <Arduino.h>

#define LED_BUILTIN 5

#define MAX_PARAM 52
extern int16_t parameters[MAX_PARAM];

#define MAX_LED                 256

#define PARAM_ERROR                51

#define PARAM_TEMPERATURE           0 // A
#define PARAM_HUMIDITY              1
#define PARAM_TEMPERATURE_EXT       2