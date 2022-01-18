#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "./common.h"
#include <AsyncMqttClient.h>
#include "./params.h"
#include "SSD1306.h" 
#include "driver/adc.h"                    // For deep sleep
#include <esp_wifi.h>
#include <esp_bt.h>
#include <esp_task_wdt.h>                  // Watchdog timer 


AsyncMqttClient mqttClient;

char broker[20];
char topic[20];

void onMqttPublish(uint16_t packetId);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

 // Set Variables for different sensors and log
float p_temperature;
float p_humidity;
float p_temperatureAHT21;
float p_humidityAHT21;
float p_temperature1w;
float p_temperature1w2;
int p_batteryLevel;
String currentLog;

// Name the device here (used in part in MQQT topics)
String DEVICE_ID = "esp321";
// Where to store offline logs on SD card
String offlineLogsPath2 = "/BeeMoS_logs/offline_temp.log";




void TaskMQTT(void* pvParameters) {

  esp_task_wdt_add(NULL); //add current thread to WDT watch

  // Protect simulatneous read/Write of different tasks to common resource (parameters) using mutex
  if(xSemaphoreTake(mutex,0) == pdTRUE){
  getParameter("mqtt.broker", broker);
  getParameter("mqtt.topic", topic);
  xSemaphoreGive(mutex);
  }

  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(5000);
  }




  


  mqttClient.setServer(broker, 1883);
  mqttClient.connect();

  Serial.println("Connecting to MQTT...");
  mqttClient.onConnect(onMqttConnect);
  vTaskDelay(500);
// mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
//  mqttClient.onSubscribe(onMqttSubscribe);
//  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  // mqttClient.onMessage(onMqttMessage);
  
  Serial.println("MQTT task: Checking for previous offline logs...");
  // check if there is a temporary file in SD card (saved offline logs) 
  handleOfflineData();

  RTClib myRTC;
  while (true) {
  ///////////////////////////////////////////////////////
  // TO DO: recover and publish offline logs here, add mutex 

  /*
  String offlineLogsPath = "/BeeMoS_logs/offline_temp.log";
   SPI.begin(14, 2, 15);
  if(!SD.begin(13)){
        Serial.println("Card Mount Failed");
        goToDeepSleep();
    }

    File file2 = SD.open(offlineLogsPath);
     String thisLine="";
     while(file2.available()){
       thisLine = file2.read();
       Serial.println("ThisLine = "+String(thisLine));
       //uint16_t packetId8 = mqttClient.publish((DEVICE_ID+"/log").c_str(), 0, true, thisLine.c_str());
    }
    file2.close();
*/
   
   ///////////////////
   
    Serial.println("Publish to MQTT server task");

    DateTime now = myRTC.now();   // Get time stamp for log

    if(xSemaphoreTake(mutex,0) == pdTRUE){

    // Retrieve log values
    p_temperature = getParameter(PARAM_TEMPERATURE)/100.0;
    p_humidity = getParameter(PARAM_HUMIDITY)/100.0;
    p_temperatureAHT21 = getParameter(PARAM_TEMPERATURE_AHT21)/100.0;
    p_humidityAHT21 = getParameter(PARAM_HUMIDITY_AHT21)/100.0;
    p_temperature1w = getParameter(PARAM_TEMPERATURE_EXT)/100.0;
    p_temperature1w2 = getParameter(PARAM_TEMPERATURE_EXT2)/100.0;
    p_batteryLevel = getParameter(PARAM_BATTERY_LEVEL);
    
   // String logTime = String(now.year()) + String(now.month() + String(now.day()) + String(now.hour()) + String(now.second()));
   // Serial.println("LOG TIME = " + logTime);
   // currentLog2 = logTime + ',' + String(p_temperature) + ',' + String(p_humidity) + ',' + String(p_temperatureAHT21) + ',' + String(p_humidityAHT21) + 
   // ',' + String(p_temperature1w) + ',' + String(p_temperature1w2) + ',' + String(p_batteryLevel);
   // Serial.println("CURRENT LOG = " + currentLog2);


     currentLog = getLogNow();

    xSemaphoreGive(mutex);

    }

    // Only transmit values, not error codes
      if (String(p_temperature) != "0.01" && String(p_temperature) != "-0.01" && p_temperature != 0){
      uint16_t packetId1 = mqttClient.publish((DEVICE_ID+"/temperature").c_str(), 0, true, String(p_temperature).c_str());
      Serial.println("p_temperature = "+String(p_temperature));
      }
    // Only transmit values, not error codes
    if (p_humidity != 0 && String(p_humidity) != "-0.01"){
      uint16_t packetId2 = mqttClient.publish((DEVICE_ID+"/humidity").c_str(), 0, true, String(p_humidity).c_str()); 
      Serial.println("p_humidity = "+String(p_humidity));
    }
    // Only transmit values, not error codes
    if (String(p_temperatureAHT21) != "0.01" && String(p_temperatureAHT21) != "-0.01" && p_temperatureAHT21 != 0){
      uint16_t packetId3 = mqttClient.publish((DEVICE_ID+"/temperatureAHT21").c_str(), 0, true, String(p_temperatureAHT21).c_str());
      Serial.println("p_temperatureAHT21 = "+String(p_temperatureAHT21));
    }
    // Only transmit values, not error codes
    if (p_humidityAHT21 != 0 && String(p_humidityAHT21) != "-0.01"){
      uint16_t packetId4 = mqttClient.publish((DEVICE_ID+"/humidityAHT21").c_str(), 0, true, String(p_humidityAHT21).c_str()); 
      Serial.println("p_humidityAHT21 = "+String(p_humidityAHT21));
    }
     // Only transmit values, not error codes
    if (p_temperature1w != -127 && p_temperature1w != 0){
      uint16_t packetId5 = mqttClient.publish((DEVICE_ID+"/temperature1wire").c_str(), 0, true, String(p_temperature1w).c_str());
      Serial.println("p_temperature1w = "+String(p_temperature1w));
    }
      // Only transmit values, not error codes
    if (p_temperature1w2 != -127 && p_temperature1w2 != 0){
      uint16_t packetId6 = mqttClient.publish((DEVICE_ID+"/temperature1wire2").c_str(), 0, true, String(p_temperature1w2).c_str());
      Serial.println("p_temperature1w2 = "+String(p_temperature1w2));
    }

    // This is the raw NCR18650b Li-ion battery level (~1600 equivalent to 2.585V is almost empty (can't connect to WiFi anymore), ~2300 equivalent to 4.10V full battery, non linear to be tested with other batteries - Value may be 40 higher when in full charge)
    if(p_batteryLevel > 1000 && p_batteryLevel < 2700){   // to avoid false values when charging
    uint16_t packetId7 = mqttClient.publish((DEVICE_ID+"/batteryLevel").c_str(), 0, true, String(p_batteryLevel).c_str());   
    // vTaskDelay(10 * 1000);
    }   

    uint16_t packetId8 = mqttClient.publish((DEVICE_ID+"/log").c_str(), 0, true, currentLog.c_str());

    vTaskDelay(1000);   

    
    Serial.println("Finished publishing to MQTT, GOING TO SLEEP NOW");
    goToDeepSleep();
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void taskMQTT() {
  xTaskCreatePinnedToCore(TaskMQTT, "TaskMQTT",
                          8000,  // This stack size can be checked & adjusted
                                  // by reading the Stack Highwater
                          NULL,
                          0,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}




void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  // uint16_t packetIdSub = mqttClient.subscribe(topic, 2);
  // Serial.print("Subscribing at QoS 2, packetId: ");
  // Serial.println(packetIdSub);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.println(total);
  Serial.println(sizeof(payload));
  Serial.println(strlen(payload));
  Serial.println(sizeof(*payload));
  Serial.println(payload);
}

String content="";
String buffer="";
int numberOfLogs=0;

void handleOfflineData(){

  SPI.begin(14, 2, 15);
    if(!SD.begin(13)){
        Serial.println("Card Mount Failed");
        goToDeepSleep();
    }
    File tempfile2 = SD.open(offlineLogsPath2);

     if(!tempfile2){
      Serial.println("The file "+offlineLogsPath2+ " does not exist or can't be opened, no offline logs to publish.");

     }
     else {
            Serial.println("The file "+offlineLogsPath2+ " does exist, offline logs will be published and deleted:");

        
        numberOfLogs=0;
  
        while(tempfile2.available()){
          // publish each offline log
          buffer = tempfile2.readStringUntil('\n');
          mqttClient.publish((DEVICE_ID+"/log").c_str(), 0, true, buffer.c_str());
          numberOfLogs++;
          // Serial.println(buffer);
          }

         // At this point the transfer is over, close file and delete it
         tempfile2.close();
         deleteFile(SD,offlineLogsPath2.c_str());
         Serial.println("Published "+String(numberOfLogs)+" offline log(s).");
     }
  tempfile2.close();
    
}