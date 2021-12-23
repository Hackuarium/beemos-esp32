#include "./common.h"
#include "./params.h"
#include "SSD1306.h"   // OLED screen model library


// SSD OLED pin definitions

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
  // Initialize screen
  display.init();
  vTaskDelay(300);
  display.flipScreenVertically();  
  display.setTextAlignment(TEXT_ALIGN_LEFT);
// display.setFont(ArialMT_Plain_16);
  display.setFont(ArialMT_Plain_24);

  while (true){

    // Fetch number of menu to display from corresponding Parameter (set up by the rotary encoder)
    int p_menu_number = getParameter(PARAM_MENU_NUMBER);
    // Serial.println("p_menu_number = "+String(p_menu_number));

    String p_temperature = String(getParameter(PARAM_TEMPERATURE)/100.0);
    String p_humidity = String(getParameter(PARAM_HUMIDITY)/100.0);
    String p_temperature2 = String(getParameter(PARAM_TEMPERATURE_EXT)/100.0);
    String p_temperature3 = String(getParameter(PARAM_TEMPERATURE_EXT2)/100.0);

    display.setFont(ArialMT_Plain_24);
 
    display.clear();

    if(p_menu_number == 0){  // Evaluate which menu should be displayed 

      if(p_temperature == "0.00" || p_temperature == "-0.01" || p_humidity == "0.00" || p_humidity == "-0.01"){
       // Serial.println("Caught error from Si7021");
        display.drawString(0 , 0 , "Si7021 not");
        display.drawStringMaxWidth(0 , 20 , 128, "detected");
      }
      else{
      display.drawString(0 , 0 , p_temperature + "°C");
      display.drawStringMaxWidth(0 , 20 , 128, p_humidity +"%");
     // Serial.println("p_temp = "+p_temperature);
     // Serial.println("p_humidity = "+p_humidity);
      }
 

    vTaskDelay(300);
    String p_wifi_rssi =String(getParameter(PARAM_WIFI_RSSI));
    // Show RSSI only once it's initialized (i.e. once WiFi is connected)
    if(p_wifi_rssi != "0"){
      display.setFont(ArialMT_Plain_24);
      display.drawString(0, 40, p_wifi_rssi+" dB");
    }
    else{
      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 50, "Connecting to WiFi");
    }
    display.display();
    vTaskDelay(300); 

   }

    if(p_menu_number == 1){ // Evaluate which menu should be displayed, use switch if there are many 
      display.clear();
      vTaskDelay(20); 
      display.drawString(0 , 0 ,  "MENU #2"); // Show "MENU #2" on OLED screen
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
