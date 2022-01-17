#include "./common.h"
#include "./params.h"


void TaskBatteryLevel(void* pvParameters) {
  (void)pvParameters;
  esp_task_wdt_add(NULL); //add current thread to WDT watch

  pinMode(36, INPUT);

  while (true) {
    int raw_level = analogRead(36);
    vTaskDelay(100);
    // Emergency sleep to allow the battery to charge enough to connect to WiFi (below this value WiFi will reset the board without allowing deepsleep)
    if (raw_level < 1600 && raw_level > 1300){
      Serial.println("EMERGENCY deep sleep"); 
       goToDeepSleep();
    }
    // Critical section (to avoid PARAMETERS being read/written at the same time by different tasks): use mutex
    if(xSemaphoreTake(mutex,0) == pdTRUE){
    setParameter(PARAM_BATTERY_LEVEL, raw_level);
    xSemaphoreGive(mutex);
    }

    vTaskDelay(900);
  }
}

void taskBatteryLevel() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskBatteryLevel, "TaskBatteryLevel",
                          1048,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          3,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
