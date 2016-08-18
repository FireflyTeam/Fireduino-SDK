 /**
 * @file TFTBitmapLogo.ino
 * @brief Definitions TFTBitmapLogo Demo for Fireduino
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
#include <TF.h>
#include <TFT.h>  // Arduino LCD library


TFT TFTscreen = TFT();

// this variable represents the image to be drawn on screen
PImage logo;


void setup() {
  // initialize the GLCD and show a message
  // asking the user to open the serial line
  delay(1);
  TFTscreen.begin();
  delay(1);
  TFTscreen.background(255, 255, 255);

  TFTscreen.stroke(0, 0, 255);
  TFTscreen.println();
  TFTscreen.println(F("Arduino TFT Bitmap Example"));
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.println(F("Open serial monitor"));
  TFTscreen.println(F("to run the sketch"));

  // initialize the serial port: it will be used to
  // print some diagnostic info
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
  }

  // clear the GLCD screen before starting
  TFTscreen.background(255, 255, 255);

  // try to access the SD card. If that fails (e.g.
  // no card present), the setup process will stop.
  Serial.print(F("Initializing SD card..."));
  if (!SD.begin()) {
    Serial.println(F("failed!"));
    return;
  }
  Serial.println(F("OK!"));

  // initialize and clear the GLCD screen
  TFTscreen.begin();
  TFTscreen.background(255, 255, 255);

  // now that the SD card can be access, try to load the
  // image file.
  logo = TFTscreen.loadImage("arduino.bmp");
  if (!logo.isValid()) {
    Serial.println(F("error while loading arduino.bmp"));
  }
}

void loop() {
  // don't do anything if the image wasn't loaded correctly.
  if (logo.isValid() == false) {
    return;
  }

  Serial.println(F("drawing image"));

  // get a random location where to draw the image.
  // To avoid the image to be draw outside the screen,
  // take into account the image size.
  int x = random(TFTscreen.width() - logo.width());
  int y = random(TFTscreen.height() - logo.height());

  // draw the image to the screen
  TFTscreen.image(logo, x, y);

  // wait a little bit before drawing again
  delay(1500);
}
