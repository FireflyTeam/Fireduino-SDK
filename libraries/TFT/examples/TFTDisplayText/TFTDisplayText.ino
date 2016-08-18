 /**
 * @file TFTDisplayText.c
 * @brief Definitions TFTDisplayText class for Fireduino
 * @author jiang<jdz@t-chip.com.cn> 
 * @version V1.0
 * @date 2016.02
 * 
 * @par Copyright:
 * Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
 *
 * For more information, please visit website <http://www.t-firefly.com/>, \n\n
 * or email to <service@t-firefly.com>.
 */ 
#include <TFT.h>  // Arduino LCD library

// create an instance of the library
TFT TFTscreen = TFT();

// char array to print to the screen
char sensorPrintout[4];

void setup() {

  // Put this line at the beginning of every sketch that uses the GLCD:
  delay(1);
  TFTscreen.begin();
  delay(1);
  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(1);
  // write the text to the top left corner of the screen
  TFTscreen.text("Sensor Value :\n ", 0, 0);
  // ste the font size very large for the loop
  TFTscreen.setTextSize(5);
}

void loop() {

  // Read the value of the sensor on A0
  String sensorVal = String(analogRead(A0));

  // convert the reading to a char array
  sensorVal.toCharArray(sensorPrintout, 4);

  // set the font color
  TFTscreen.stroke(255, 255, 255);
  // print the sensor value
  TFTscreen.text(sensorPrintout, 0, 20);
  // wait for a moment
  delay(250);
  // erase the text you just wrote
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintout, 0, 20);
}

