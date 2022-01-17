#include "./common.h"
#include "./params.h"
#include <OneWire.h>
#include <DallasTemperature.h>


void TaskOneWire(void* pvParameters) {
  (void)pvParameters;
  esp_task_wdt_add(NULL); //add current thread to WDT watch
  OneWire ds(33);


  DallasTemperature sensors_oneWire(&ds);

  sensors_oneWire.begin();
      vTaskDelay(1000);
  
  
  while (true) {

    sensors_oneWire.requestTemperatures();

    // vTaskDelay(1000);
    
   // Serial.print("TEMP_DALLAS1 = ");
   int lastTemp=sensors_oneWire.getTempCByIndex(0)*100;
   // Give a second chance in case an error is produced
  // Give a second chance in case an error is produced
  byte max_tries = 5;
  byte tries = 0;
   while(lastTemp != -127 || tries < max_tries){
     lastTemp=sensors_oneWire.getTempCByIndex(0)*100;
     tries++;
     vTaskDelay(200);
  //  Serial.println("Error from OneWire temperature1, retrying...");
   } 
   // Serial.println(lastTemp);  
   // Serial.print("TEMP_DALLAS2 = ");
   int lastTemp2=sensors_oneWire.getTempCByIndex(1)*100;
  // Give a second chance in case an error is produced
   tries = 0;
   while(lastTemp2!=-127 || tries < max_tries){
     lastTemp=sensors_oneWire.getTempCByIndex(1)*100;
     tries++;
     vTaskDelay(200);
    // Serial.println("Error from OneWire temperature2, retrying...");
   }  

  // Critical section (to avoid PARAMETERS being read/written at the same time by different tasks): use mutex
    if(xSemaphoreTake(mutex,0) == pdTRUE){  
	  setParameter(PARAM_TEMPERATURE_EXT, sensors_oneWire.getTempCByIndex(0)*100);
  vTaskDelay(200);
	  setParameter(PARAM_TEMPERATURE_EXT2, sensors_oneWire.getTempCByIndex(1)*100);
    vTaskDelay(200);
    xSemaphoreGive(mutex);
   }
        
  }
  vTaskDelay(10);
}

void taskOneWire() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskOneWire, "TaskOneWire",
                          2048,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          3,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
