#include "./common.h"
#include "./params.h"
#include "Adafruit_Si7021.h"


void TaskSI7021(void* pvParameters) {
  Adafruit_Si7021 sensor = Adafruit_Si7021();
  vTaskDelay(100);
  (void)pvParameters;

  esp_task_wdt_add(NULL); //add current thread to WDT watch
  
  while (!sensor.begin()) {                            
    Serial.println("Did not find Si7021 sensor!");
    vTaskDelay(300);  

    
  }


  while (true) {
    int s_temp = 100*sensor.readTemperature();
    byte tries=0;
    byte max_tries=5;
    while ((s_temp == 0||s_temp == -1||s_temp == 1) && tries<max_tries){
      s_temp = 100*sensor.readTemperature();
      vTaskDelay(100);
      tries++;
    }


    int s_humidity = 100*sensor.readHumidity(); 
    tries=0;

        while ((s_humidity == 0 || s_humidity == -1) && tries<max_tries){
      s_humidity = 100*sensor.readHumidity();
      vTaskDelay(100);
      tries++;
    }

    if(xSemaphoreTake(mutex,0) == pdTRUE){
    
      if (s_temp != 0 && s_temp != -1 && s_temp != 1){  // Set parameter except if there were errors despite max_tries
	    setParameter(PARAM_TEMPERATURE, s_temp);
      }
      if (s_humidity != 0 && s_humidity != -1 && s_humidity != 1){  // Set parameter except if there were errors despite max_tries
	    setParameter(PARAM_HUMIDITY, s_humidity);
      }
      
    xSemaphoreGive(mutex);
    }

     else{
            printf("semaphore is taken by someone else \n");
            vTaskDelete(0);
        }

    vTaskDelay(1000);
  }
}

void taskSI7021() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskSI7021, "TaskSI7021",
                          2048,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          2,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
