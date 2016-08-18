 /**
 * @file TFTColorPicker.ino
 * @brief Definitions TFTColorPicker Demo for Fireduino
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

TFT TFTscreen = TFT();

void setup() {
  // begin serial communication
  Serial.begin(9600);

  // initialize the display
  delay(1);
  TFTscreen.begin();
  delay(1);
  // set the background to white
  TFTscreen.background(255, 255, 255);

}

void loop() {

  // read the values from your sensors and scale them to 0-255
  int redVal = map(analogRead(A0), 0, 1023, 0, 255);
  int greenVal = map(analogRead(A1), 0, 1023, 0, 255);
  int blueVal = map(analogRead(A2), 0, 1023, 0, 255);

  // draw the background based on the mapped values
  TFTscreen.background(redVal, greenVal, blueVal);

  // send the values to the serial monitor
  Serial.print("background(");
  Serial.print(redVal);
  Serial.print(" , ");
  Serial.print(greenVal);
  Serial.print(" , ");
  Serial.print(blueVal);
  Serial.println(")");

  // wait for a moment
  delay(33);

}

