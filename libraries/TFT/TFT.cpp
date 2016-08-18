 /**
 * @file TFT.cpp
 * @brief Definitions TFT class for Fireduino
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
#include "TFT.h"

TFT::TFT()
  : Adafruit_ST7735()
{
  _width = ST7735_TFTHEIGHT;
  _height = ST7735_TFTWIDTH;
}

void TFT::begin() {
  initS();
  setRotation(0);
  backlight(20);
  delay(1);
}
