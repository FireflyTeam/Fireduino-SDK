 /**
 * @file TFTPong.c
 * @brief Definitions TFTPong Demo for Fireduino
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

// variables for the position of the ball and paddle
int paddleX = 0;
int paddleY = 0;
int oldPaddleX, oldPaddleY;
int ballDirectionX = 1;
int ballDirectionY = 1;

int ballSpeed = 10; // lower numbers are faster

int ballX, ballY, oldBallX, oldBallY;

void setup() {
  // initialize the serial port
  Serial.begin(9600);

  // initialize the display
  delay(1);
  TFTscreen.begin();
  delay(1);
  // black background
  TFTscreen.background(255, 0, 0);
}
void moveBall();
boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) ;
void loop() {

  // save the width and height of the screen
  int myWidth = TFTscreen.width();
  int myHeight = TFTscreen.height();
  Serial.println(myWidth);
  Serial.println(myHeight);
  // map the paddle's location to the position of the potentiometers
  paddleX = map(analogRead(A0), 512, -512, 0, myWidth) - 20 / 2;
  paddleY = map(analogRead(A1), 512, -512, 0, myHeight) - 5 / 2;
  Serial.println(paddleX);
  Serial.println(paddleY);
  // set the fill color to black and erase the previous
  // position of the paddle if different from present
  TFTscreen.fill(0, 0, 0);

  if (oldPaddleX != paddleX || oldPaddleY != paddleY) {
    TFTscreen.rect(oldPaddleX, oldPaddleY, 20, 5);
  }

  // draw the paddle on screen, save the current position
  // as the previous.
  TFTscreen.fill(255, 255, 255);

  TFTscreen.rect(paddleX, paddleY, 20, 5);
  oldPaddleX = paddleX;
  oldPaddleY = paddleY;

  // update the ball's position and draw it on screen
  if (millis() % ballSpeed < 2) {
    moveBall();
  }
}

// this function determines the ball's position on screen
void moveBall() {
  // if the ball goes offscreen, reverse the direction:
  if (ballX > TFTscreen.width() || ballX < 0) {
    ballDirectionX = -ballDirectionX;
  }

  if (ballY > TFTscreen.height() || ballY < 0) {
    ballDirectionY = -ballDirectionY;
  }

  // check if the ball and the paddle occupy the same space on screen
  if (inPaddle(ballX, ballY, paddleX, paddleY, 20, 5)) {
    ballDirectionX = -ballDirectionX;
    ballDirectionY = -ballDirectionY;
  }

  // update the ball's position
  ballX += ballDirectionX;
  ballY += ballDirectionY;

  // erase the ball's previous position
  TFTscreen.fill(0, 0, 0);

  if (oldBallX != ballX || oldBallY != ballY) {
    TFTscreen.rect(oldBallX, oldBallY, 5, 5);
  }


  // draw the ball's current position
  TFTscreen.fill(255, 255, 255);
  TFTscreen.rect(ballX, ballY, 5, 5);

  oldBallX = ballX;
  oldBallY = ballY;

}

// this function checks the position of the ball
// to see if it intersects with the paddle
boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {
  boolean result = false;

  if ((x >= rectX && x <= (rectX + rectWidth)) &&
      (y >= rectY && y <= (rectY + rectHeight))) {
    result = true;
  }

  return result;
}


