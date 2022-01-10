#include "./params.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_task_wdt.h>                  // Watchdog timer 
#include "SSD1306.h" 
#include "driver/adc.h"                    // For deep sleep
#include <esp_wifi.h>
#include <esp_bt.h>

char ssid[20];
char password[20];


void goToDeepSleepNoWifi(){          // This function is a mirror of the one in taskMQTT
  
 Serial.println("WiFi timeout reached, Going to sleep now.");
 vTaskDelay(50);
 // Prepare before sleep
 Serial.println(millis());
 Serial.flush();
 // Create OLED instance "display" just to be able to switch it off from this task
 SSD1306 display2(0x3c, 21, 22);
 display2.displayOff();  // Switch off OLED
 vTaskDelay(50);
 WiFi.disconnect(true);
 WiFi.mode(WIFI_OFF);
 btStop();
 adc_power_off();
 esp_wifi_stop();
 esp_bt_controller_disable();
  esp_sleep_enable_timer_wakeup(LOG_INTERVAL_DURATION*1000000 - 1000*millis()); // Deep sleep timer between measurements in uS
 esp_deep_sleep_start();  // Deep sleep here
 vTaskDelay(50);
 Serial.println("This is after deep sleep and will never be printed.");
}

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
    Serial.println("Entered function");
    // TO DO: log values on memory resilient to sleep (RTC internal/external TBD)

    goToDeepSleepNoWifi();    
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
                          3,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
