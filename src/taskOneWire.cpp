#include "./common.h"
#include "./params.h"
#include <OneWire.h>
#include <DallasTemperature.h>


void TaskOneWire(void* pvParameters) {
  (void)pvParameters;
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
   if(lastTemp==-127){
    lastTemp=sensors_oneWire.getTempCByIndex(0)*100;
     vTaskDelay(200);
   }  
   // Serial.println(lastTemp);  
   // Serial.print("TEMP_DALLAS2 = ");
   int lastTemp2=sensors_oneWire.getTempCByIndex(1)*100;
  // Give a second chance in case an error is produced
   if(lastTemp2==-127){
     lastTemp=sensors_oneWire.getTempCByIndex(1)*100;
     vTaskDelay(200);
   }  
   // Serial.println(lastTemp2);    
	  setParameter(PARAM_TEMPERATURE_EXT, sensors_oneWire.getTempCByIndex(0)*100);
  vTaskDelay(200);
	  setParameter(PARAM_TEMPERATURE_EXT2, sensors_oneWire.getTempCByIndex(1)*100);
      vTaskDelay(1000);
        
  }
  vTaskDelay(10);
}

void taskOneWire() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskOneWire, "TaskOneWire",
                          4048,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          3,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
