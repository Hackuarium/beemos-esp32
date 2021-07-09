#include "./params.h"
#include <WiFi.h>

char ssid[20];
char password[20];

void TaskWifi(void* pvParameters) {
  getParameter("wifi.ssid", ssid);
  getParameter("wifi.password",password);
  Serial.print("Trying to connect to ");
  Serial.println(ssid);
  Serial.print("Using password ");
  Serial.println(password);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(500);
    Serial.print(".");
  }

    Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  while (true) {
    vTaskDelay(10000);
      if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        vTaskDelay(5000);
        WiFi.reconnect();
      }
  }
}

void taskWifi() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskWifi, "TaskWifi",
                          8000,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          2,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
