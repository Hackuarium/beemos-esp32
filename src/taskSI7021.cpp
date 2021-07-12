#include "./common.h"
#include "./params.h"
#include "Adafruit_Si7021.h"


void TaskSI7021(void* pvParameters) {
  Adafruit_Si7021 sensor = Adafruit_Si7021();
  (void)pvParameters;

  if (!sensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    while (true)
      ;
  }


  while (true) {
    int s_temp = 100*sensor.readTemperature();
    byte tries=0;
    byte max_tries=5;
    while (s_temp == -0.01 && tries<max_tries){
      s_temp = 100*sensor.readTemperature();
      tries++;
    }

    int s_humidity = 100*sensor.readHumidity();
    tries=0;
	  setParameter(PARAM_TEMPERATURE, s_temp);
        while (s_humidity == -0.01 && tries<max_tries){
      s_humidity = 100*sensor.readHumidity();
      tries++;
    }
	  setParameter(PARAM_HUMIDITY, s_humidity);

    vTaskDelay(1000);
  }
}

void taskSI7021() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskSI7021, "TaskSI7021",
                          2048,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          0,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
