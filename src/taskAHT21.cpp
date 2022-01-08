#include "./common.h"
#include "./params.h"
#include <Wire.h>
#include <SparkFun_Qwiic_Humidity_AHT20.h>

AHT20 AHT21Sensor;

void TaskAHT21(void* pvParameters) {
  (void)pvParameters;


  vTaskDelay(100);
   Wire.begin(21,22);  
   vTaskDelay(50);  

  while (AHT21Sensor.begin() == false) {                            
    Serial.println("Did not find AHT21 sensor!");
    vTaskDelay(500);  
  }


  while (true) {

    while(AHT21Sensor.available() == true){
 
  int s_temp = 100*AHT21Sensor.getTemperature();
 // Serial.println(String("")+"AHT21 temp = "+AHT21Sensor.getTemperature()+"℃");

  vTaskDelay(500);
    byte tries=0;
    byte max_tries=5;
    while ((s_temp == 0||s_temp == -1||s_temp == 1) && tries<max_tries){
      s_temp = 100*AHT21Sensor.getTemperature();
      vTaskDelay(100);
      tries++;
    }
    if (s_temp != 0 && s_temp != -1 && s_temp != 1){  // Set parameter except if there were errors despite max_tries
	    setParameter(PARAM_TEMPERATURE_AHT21, s_temp);
    }

    int s_humidity = 100*AHT21Sensor.getHumidity(); 
   // Serial.println(String("")+"AHT21 hygro = "+AHT21Sensor.getHumidity()+"%");
    tries=0;

        while ((s_humidity == 0 || s_humidity == -1) && tries<max_tries){
      s_humidity = 100*AHT21Sensor.getHumidity(); ;
      vTaskDelay(100);
      tries++;
    }
    if (s_humidity != 0 && s_humidity != -1 && s_humidity != 1){  // Set parameter except if there were errors despite max_tries
	    setParameter(PARAM_HUMIDITY_AHT21, s_humidity);
    }

    }
    vTaskDelay(1000);
  }
}

void taskAHT21() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskAHT21, "TaskAHT21",
                          1000,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          2,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
