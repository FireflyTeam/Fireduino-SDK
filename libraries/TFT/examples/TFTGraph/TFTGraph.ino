 /**
 * @file TFTGraph.ino
 * @brief Definitions TFTGraph Demo for Fireduino
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

// position of the line on screen
int xPos = 0;

void setup() {
  // initialize the serial port
  Serial.begin(9600);

  // initialize the display
  delay(1);
  TFTscreen.begin();
  delay(1);
  // clear the screen with a pretty color
  TFTscreen.background(250, 16, 200);
}

void loop() {
  // read the sensor and map it to the screen height
  int sensor = analogRead(A0);
  int drawHeight = map(sensor, 0, 1023, 0, TFTscreen.height());

  // print out the height to the serial monitor
  Serial.println(drawHeight);

  // draw a line in a nice color
  TFTscreen.stroke(250, 180, 10);
  TFTscreen.line(xPos, TFTscreen.height() - drawHeight, xPos, TFTscreen.height());

  // if the graph has reached the screen edge
  // erase the screen and start again
  if (xPos >= 160) {
    xPos = 0;
    TFTscreen.background(250, 16, 200);
  } else {
    // increment the horizontal position:
    xPos++;
  }

  delay(16);
}

