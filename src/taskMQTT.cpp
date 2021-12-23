#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "./common.h"
#include <AsyncMqttClient.h>
#include "./params.h"

AsyncMqttClient mqttClient;

char broker[20];
char topic[20];

void onMqttPublish(uint16_t packetId);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);


void TaskMQTT(void* pvParameters) {

  getParameter("mqtt.broker", broker);
  getParameter("mqtt.topic", topic);

  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(5000);
  }


  mqttClient.setServer(broker, 1883);
  mqttClient.connect();

  Serial.println("Connecting to MQTT...");
  mqttClient.onConnect(onMqttConnect);
 // vTaskDelay(200);
// mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
//  mqttClient.onSubscribe(onMqttSubscribe);
//  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  // mqttClient.onMessage(onMqttMessage);

  while (true) {
    Serial.println("Publish to MQTT server task");
    float p_temperature = getParameter(PARAM_TEMPERATURE)/100.0;
    float p_humidity = getParameter(PARAM_HUMIDITY)/100.0;
    float p_temperature1w = getParameter(PARAM_TEMPERATURE_EXT)/100.0;
    float p_temperature1w2 = getParameter(PARAM_TEMPERATURE_EXT2)/100.0;
    int p_batteryLevel = getParameter(PARAM_BATTERY_LEVEL);
    // Only transmit values, not error codes
    if (String(p_temperature) != "0.01" && String(p_temperature) != "-0.01" && p_temperature != 0){
      uint16_t packetId1 = mqttClient.publish("esp32/temperature", 0, true, String(p_temperature).c_str());
      Serial.println("p_temperature = "+String(p_temperature));
    }
    // Only transmit values, not error codes
    if (p_humidity != 0 && String(p_humidity) != "-0.01"){
      uint16_t packetId2 = mqttClient.publish("esp32/humidity", 0, true, String(p_humidity).c_str()); 
      Serial.println("p_humidity = "+String(p_humidity));
    }
     // Only transmit values, not error codes
    if (p_temperature1w != -127 && p_temperature1w != 0){
      uint16_t packetId3 = mqttClient.publish("esp32/temperature1wire", 0, true, String(p_temperature1w).c_str());
      Serial.println("p_temperature1w = "+String(p_temperature1w));
    }
      // Only transmit values, not error codes
    if (p_temperature1w2 != -127 && p_temperature1w2 != 0){
      uint16_t packetId3 = mqttClient.publish("esp32/temperature1wire2", 0, true, String(p_temperature1w2).c_str());
      Serial.println("p_temperature1w2 = "+String(p_temperature1w2));
    }
    // This is the raw NCR18650b Li-ion battery level (~1600 equivalent to 2.585V is almost empty (can't connect to WiFi anymore), ~2300 equivalent to 4.10V full battery, non linear to be tested with other batteries - Value may be 40 higher when in full charge)
    uint16_t packetId5 = mqttClient.publish("esp32/batteryLevel", 0, true, String(p_batteryLevel).c_str());   
    vTaskDelay(10 * 1000);
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
