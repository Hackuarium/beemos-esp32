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
	  setParameter(PARAM_TEMPERATURE, sensor.readTemperature()*100);
	  setParameter(PARAM_HUMIDITY, sensor.readHumidity()*100);

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
