#include "./common.h"
#include "./params.h"
#include <OneWire.h>
#include <DallasTemperature.h>


void TaskOneWire(void* pvParameters) {
  (void)pvParameters;
  OneWire oneWire(33);
  DallasTemperature sensor_oneWire(&oneWire);

  sensor_oneWire.begin();

  while (true) {
	  sensor_oneWire.requestTemperatures();

	  setParameter(PARAM_TEMPERATURE_EXT, sensor_oneWire.getTempCByIndex(0)*100);

    vTaskDelay(1000);
  }
}

void taskOneWire() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskOneWire, "TaskOneWire",
                          2048,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          0,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
