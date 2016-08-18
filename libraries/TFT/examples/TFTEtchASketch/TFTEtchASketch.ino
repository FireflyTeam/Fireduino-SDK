 /**
 * @file TFTEtchASketch.ino
 * @brief Definitions TFTEtchASketch Demo for Fireduino
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

// initial position of the cursor
int xPos = TFTscreen.width() / 2;
int yPos = TFTscreen.height() / 2;

// pin the erase switch is connected to
int erasePin = 2;

void setup() {
  // declare inputs
  pinMode(erasePin, INPUT);
  // initialize the screen
  delay(1);
  TFTscreen.begin();
  delay(1);
  // make the background black
  TFTscreen.background(0, 0, 0);
}

void loop() {
  // read the potentiometers on A0 and A1
  int xValue = analogRead(A0);
  int yValue = analogRead(A1);

  // map the values and update the position
  xPos = xPos + (map(xValue, 0, 1023, 2, -2));
  yPos = yPos + (map(yValue, 0, 1023, -2, 2));

  // don't let the point go past the screen edges
  if (xPos > 159) {
    (xPos = 159);
  }

  if (xPos < 0) {
    (xPos = 0);
  }
  if (yPos > 127) {
    (yPos = 127);
  }

  if (yPos < 0) {
    (yPos = 0);
  }

  // draw the point
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.point(xPos, yPos);

  // read the value of the pin, and erase the screen if pressed
  if (digitalRead(erasePin) == HIGH) {
    TFTscreen.background(0, 0, 0);
  }

  delay(33);
}
