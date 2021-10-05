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
   // Serial.println(String("TEMP_DALLAS1 = " + last_temp1));
   // Serial.println(String("TEMP_DALLAS2 = " + last_temp2));
   //     Serial.println("TEST1");    
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
                          2,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
