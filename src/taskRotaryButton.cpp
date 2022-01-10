#include "./common.h"
#include "./params.h"
#include <AiEsp32RotaryEncoder.h>  // Had to rename folder containing the library to "AiEsp32RotaryEncoder" (instead of "Ai_Esp32_Rotary_Encoder" to match ".h" file)

#define ROTARY_ENCODER_A_PIN 34
#define ROTARY_ENCODER_B_PIN 35
#define ROTARY_ENCODER_BUTTON_PIN 39
#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */

//depending on your encoder - try 1,2 or 4 to get expected behaviour
//#define ROTARY_ENCODER_STEPS 1
//#define ROTARY_ENCODER_STEPS 2
#define ROTARY_ENCODER_STEPS 4


AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

byte MENU_NUMBER = 0;  // Set default value for menu

void rotary_onButtonClick()
{
	static unsigned long lastTimePressed = 0;
	//ignore multiple press in that time milliseconds
	if (millis() - lastTimePressed < 300)
	{
		return;
	}
	lastTimePressed = millis();
	Serial.print("button pressed at ");
	Serial.println(millis());
}


void IRAM_ATTR readEncoderISR()
{
	rotaryEncoder.readEncoder_ISR();
}

void TaskRotaryButton(void* pvParameters) {
  (void)pvParameters;
  
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  //set boundaries and if values should cycle or not
  bool circleValues = false;
  rotaryEncoder.setBoundaries(0, 1, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
  // rotaryEncoder.setAcceleration(250); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration


  while (true) {
 

   if (rotaryEncoder.encoderChanged())
	{
    // Serial.println("MENU = "+String(rotaryEncoder.readEncoder()));
    // Store value of rotary encoder as paramater menu number
    MENU_NUMBER = rotaryEncoder.readEncoder();
    vTaskDelay(100);
	}
	if (rotaryEncoder.isEncoderButtonClicked())
	{
		rotary_onButtonClick();
	}
   
    vTaskDelay(50);
  }

}

void taskRotaryButton() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskRotaryButton, "TaskRotaryButton",
                          2048,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          2,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}
