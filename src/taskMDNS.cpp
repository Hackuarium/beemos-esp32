#include <ESPmDNS.h>
#include <WiFi.h>
#include "./params.h"

char mdns[20];

void TaskMDNS(void* pvParameters) {
  vTaskDelay(5000);
  esp_task_wdt_add(NULL); //add current thread to WDT watch
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(1000);
  }
 // Critical section (to avoid PARAMETERS being read/written at the same time by different tasks): use mutex
  if(xSemaphoreTake(mutex,0) == pdTRUE){
  getParameter("wifi.mdns", mdns);
  xSemaphoreGive(mutex);
  }
  
  Serial.print("Starting MDNS responder for: ");
  Serial.print(mdns);
  Serial.println(".local");
  if (MDNS.begin(mdns)) {
    Serial.println("MDNS responder started");
  } else {
    Serial.println("MDNS responder FAILED");
  }

  while (true) {
    vTaskDelay(10000);
  }
}

void taskMDNS() {
    vTaskDelay(2000);
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskMDNS, "TaskMDNS",
                          8000,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          2,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
