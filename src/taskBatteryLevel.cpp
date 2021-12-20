#include "./common.h"
#include "./params.h"


void TaskBatteryLevel(void* pvParameters) {
  (void)pvParameters;

  pinMode(36, INPUT);

  while (true) {
    int raw_level = analogRead(36);
    vTaskDelay(100);
    setParameter(PARAM_BATTERY_LEVEL, raw_level);
    // Serial.println("raw_level = "+String(raw_level));
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
