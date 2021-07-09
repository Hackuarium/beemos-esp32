#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "./common.h"
#include "./taskNTPD.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String dayStamp;
String timeStamp;

struct tm timeInfo;

void TaskNTPD(void* pvParameters) {
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(5000);
  }

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(3600);

  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = 3600;
  const int   daylightOffset_sec = 3600;

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  while (true) {
    timeClient.update();
    vTaskDelay(15 * 60 * 1000);
  }
}

NTPClient getTimeClient() {
  return timeClient;
}

void getHourMinute(char* hourMinute) {
  // todo we should use local time
  getLocalTime(&timeInfo);
  strftime(hourMinute, 6, "%H:%M", &timeInfo);
}

int16_t getHourMinute() {
  // todo we should use local time
  getLocalTime(&timeInfo);
  return timeInfo.tm_hour*60+timeInfo.tm_min;
}

int getSeconds() {
  return timeClient.getSeconds();
}

void taskNTPD() {
  // Now set up two tasks to rntpdun independently.
  xTaskCreatePinnedToCore(TaskNTPD, "TaskNTPD",
                          20000,  // This stack size can be checked & adjusted
                                  // by reading the Stack Highwater
                          NULL,
                          3,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
