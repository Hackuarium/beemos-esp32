#include "./common.h"
#include <NTPClient.h>

void TaskNTPD(void* pvParameters);

void getHourMinute(char* hourMinute);
int16_t getHourMinute();
int getSeconds();
NTPClient getTimeClient();

void taskNTPD();


