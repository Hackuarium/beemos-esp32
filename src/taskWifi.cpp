#include "./params.h"
#include <WiFi.h>
#include <esp_wifi.h>

char ssid[20];
char password[20];

void TaskWifi(void* pvParameters) {
  getParameter("wifi.ssid", ssid);
  getParameter("wifi.password",password);
  Serial.print("Trying to connect to ");
  Serial.println(ssid);
  Serial.print("Using password ");
  Serial.println(password);

if (false) { //  access point
Serial.println("Starting wifi server");
 WiFi.mode(WIFI_AP);
 esp_wifi_set_protocol( WIFI_IF_AP, WIFI_PROTOCOL_LR );
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);


  while (true) {
    vTaskDelay(1000);
     //  setParameter(PARAM_WIFI_RSSI, WiFi.RSSI());
  }
} else {
	 WiFi.mode(WIFI_STA);
	  esp_wifi_set_protocol( WIFI_IF_STA, WIFI_PROTOCOL_LR );
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
    vTaskDelay(1000);
      if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        vTaskDelay(5000);
        WiFi.reconnect();
      }
       setParameter(PARAM_WIFI_RSSI, WiFi.RSSI());
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
