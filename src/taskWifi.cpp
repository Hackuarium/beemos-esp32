#include "./common.h"
#include "./params.h"
#include <WiFi.h>
#include <esp_wifi.h>


char ssid[20];
char password[20];


void TaskWifi(void* pvParameters) {

  esp_task_wdt_add(NULL); //add current thread to WDT watch
  // Critical section (to avoid PARAMETERS being read/written at the same time by different tasks): use mutex
  if(xSemaphoreTake(mutex,0) == pdTRUE){
  getParameter("wifi.ssid", ssid);
  getParameter("wifi.password",password);
  xSemaphoreGive(mutex);
  }

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
    // setParameter(PARAM_WIFI_RSSI, WiFi.RSSI());
  }
} else {

	WiFi.mode(WIFI_STA);
	esp_wifi_set_protocol( WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N );
  WiFi.begin(ssid, password);
 // vTaskDelay(500);
  // Wait for connection
  
  byte counter = 0;
  // While not connected for WIFI_CONNECT_TIMEOUT seconds
  while (WiFi.status() != WL_CONNECTED && counter/2 <= WIFI_CONNECT_TIMEOUT) {
    vTaskDelay(500);
    Serial.println("."); 
    counter++;
    Serial.println(counter/2);
  }

  // In case WiFi not connected after trying for more than WIFI_CONNECT_TIMEOUT seconds
  if(counter/2 >= WIFI_CONNECT_TIMEOUT){
    logToSDcard();     // log values on memory resilient to sleep 
    goToDeepSleep();    
  } else {

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  }

  while (true) {
    vTaskDelay(1000);
      if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        vTaskDelay(3000);
        WiFi.reconnect();
      }
       // Protect simulatneous read/Write of different tasks to common resource (parameters) using mutex
  if(xSemaphoreTake(mutex,0) == pdTRUE){ 
       setParameter(PARAM_WIFI_RSSI, WiFi.RSSI());
       xSemaphoreGive(mutex);
    }
      // Serial.println("GOT RSSI");
  }
}
 
}

void taskWifi() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskWifi, "TaskWifi",
                          8000,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          3,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
