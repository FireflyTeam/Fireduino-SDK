 /**
 * @file TFT.h
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
#ifndef _ARDUINO_TFT_H
#define _ARDUINO_TFT_H

#include "Arduino.h"
#include "utility/Adafruit_GFX.h"
#include "utility/Adafruit_ST7735.h"

class TFT : public Adafruit_ST7735 {
public:
  TFT();
  void begin();  
};

#endif // _ARDUINO_TFT_H
