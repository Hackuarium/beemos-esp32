#include "./common.h"
#include "./params.h"
#include "SSD1306.h"   // OLED screen model library


// LORA pin definitions

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    868E6 // LoRa MHz


// Create OLED instance "display"
SSD1306 display(0x3c, 21, 22);

void TaskOLEDscreen(void* pvParameters) {
  (void)pvParameters;
  esp_task_wdt_add(NULL); // add current thread to WDT watch
  // Initialize screen
  display.init();
  vTaskDelay(500);  // increase if text is shown on OLED but truncated or misplaced
  display.flipScreenVertically();  
  display.setTextAlignment(TEXT_ALIGN_LEFT);
// display.setFont(ArialMT_Plain_16);
  display.setFont(ArialMT_Plain_24);
 
  String msg = "Connecting to WiFi  ";
  byte symToDisplay;   // Number of symbols to show on screen (while trying to connect to WiFi)
  String p_temperature;
  String p_humidity;
  String p_temperature2;
  String p_temperature3;
  String p_temperatureAHT21;
  String p_humidityAHT21;
  String p_wifi_rssi;
  
  while (true){


    // Critical section (to avoid PARAMETERS being read/written at the same time by different tasks): use mutex
    if(xSemaphoreTake(mutex,0) == pdTRUE){

    p_temperature = String(getParameter(PARAM_TEMPERATURE)/100.0);
    p_humidity = String(getParameter(PARAM_HUMIDITY)/100.0);
    p_temperature2 = String(getParameter(PARAM_TEMPERATURE_EXT)/100.0);
    p_temperature3 = String(getParameter(PARAM_TEMPERATURE_EXT2)/100.0);
    p_temperatureAHT21 = String(getParameter(PARAM_TEMPERATURE_AHT21)/100.0);
    p_humidityAHT21 = String(getParameter(PARAM_HUMIDITY_AHT21)/100.0);
    p_wifi_rssi =String(getParameter(PARAM_WIFI_RSSI));
    xSemaphoreGive(mutex);
    }


    display.setFont(ArialMT_Plain_24);


    display.clear();
 
    if(MENU_NUMBER == 0){  // Evaluate which menu should be displayed 

      if(p_temperature == "0.00" || p_temperature == "-0.01" || p_humidity == "0.00" || p_humidity == "-0.01"){
       // Serial.println("Caught error from Si7021");
        display.drawString(0 , 0 , "Si7021 not");
        display.drawStringMaxWidth(0 , 20 , 128, "detected");
              }
      else {
      display.drawString(0 , 0 , p_temperature + "°C");
      display.drawStringMaxWidth(0 , 20 , 128, p_humidity +"%");
     // Serial.println("p_temp = "+p_temperature);
     // Serial.println("p_humidity = "+p_humidity);
      }
 

   

    
    // Show RSSI only once it's initialized (i.e. once WiFi is connected)
    if(p_wifi_rssi != "0"){
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 45, "WiFi OK");
     // display.display();
  
      display.setFont(ArialMT_Plain_24);
      display.drawString(70, 42, p_wifi_rssi);
      display.setFont(ArialMT_Plain_16);
      display.drawString(108, 48, "dB");
     // display.display();
      vTaskDelay(50);  // increase delay if text is shown on OLED but truncated or misplaced
    }
    else{

      

      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 50, msg);
      //for (byte i = 0; i < pointsToDisplay; i++){
        msg = msg + ">";
     // }
      symToDisplay++;
      vTaskDelay(150);  // slow down how fast points are shown sequentially
      
        if(symToDisplay == 6){
          symToDisplay=0;
          msg = "Connecting to WiFi  ";
        }
      
      vTaskDelay(50);   // increase delay if text is shown on OLED but truncated or misplaced
      
    }

    display.display();
    vTaskDelay(250); 

   }

   // display.setFont(ArialMT_Plain_24); 

    if(MENU_NUMBER == 1){ // Evaluate which menu should be displayed, use switch if there are many 
      display.clear();
      vTaskDelay(20);
      
    if(p_temperatureAHT21 == "0.00" || p_temperatureAHT21 == "-0.01" || p_humidityAHT21 == "0.00" || p_humidityAHT21 == "-0.01"){
      
        display.drawString(0 , 0 , "AHT21 not");
        display.drawStringMaxWidth(0 , 20 , 128, "detected");
      }
      else{
      display.drawString(0 , 0 , p_temperatureAHT21 + "°C");
      display.drawStringMaxWidth(0 , 20 , 128, p_humidityAHT21 +"%");
      }

      display.display();
      vTaskDelay(100); 
    }


  }

}

void taskOLEDscreen() {
  xTaskCreatePinnedToCore(TaskOLEDscreen, "TaskOLEDscreen",
                          2500,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          3,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
