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
<<<<<<< HEAD
    
	  sensors_oneWire.requestTemperatures();


    int last_temp1 = sensors_oneWire.getTempCByIndex(0)*100;
   // vTaskDelay(1000);
    // Define filtering of "-127" values, using a maximum number of tries before really assigning -127
    byte tries = 0; 
    byte max_tries=5;
    while((last_temp1==0||last_temp1==-12700) && tries<max_tries){
      last_temp1 = sensors_oneWire.getTempCByIndex(0)*100;
      tries++;
    }
    tries=0;
    int last_temp2 = sensors_oneWire.getTempCByIndex(1)*100;
    while((last_temp2==0||last_temp2==-12700) && tries<max_tries){
      last_temp2 = sensors_oneWire.getTempCByIndex(1)*100;
      tries++;
    }
=======
    sensor_oneWire.requestTemperatures();
	  setParameter(PARAM_TEMPERATURE_EXT, sensor_oneWire.getTempCByIndex(0)*100);
>>>>>>> a930e0411fd1ecdc6e6a83ca4c9a458da155c72a

    // vTaskDelay(1000);
   // Serial.println(String("TEMP_DALLAS1 = " + last_temp1));
   // Serial.println(String("TEMP_DALLAS2 = " + last_temp2));
   //     Serial.println("TEST1");    
	  setParameter(PARAM_TEMPERATURE_EXT, last_temp1);
  vTaskDelay(200);
	  setParameter(PARAM_TEMPERATURE_EXT2, last_temp2);
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
