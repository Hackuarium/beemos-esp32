
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <common.h>

void TaskOTA(void* pvParameters) {
  esp_task_wdt_add(NULL); //add current thread to WDT watch
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(5000);
  }

  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else  // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS
        // using SPIFFS.end()
        Serial.println("Start updating " + type);
      })
      .onEnd([]() {
        Serial.println("\nCompleted");
        MDNS.end();
        vTaskDelay(200);
        WiFi.disconnect();  // disconnect to wifi to have a clean restart
        vTaskDelay(2000);
        Serial.println("Disconnected from WIFI");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");
      });

  ArduinoOTA.begin();
  while (true) {
    ArduinoOTA.handle();
    vTaskDelay(1);
  }
}

void taskOTA() {
  vTaskDelay(2000);
  // Now set up two tasks to rntpdun independently.
  xTaskCreatePinnedToCore(TaskOTA, "TaskOTA",
                          20000,  // This stack size can be checked & adjusted
                                  // by reading the Stack Highwater
                          NULL,
                          3,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 0);

}
