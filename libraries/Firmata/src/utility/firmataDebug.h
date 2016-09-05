 /**
 * @file firmataDebug.h
 * @brief Definitions firmataDebug class for Fireduino
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
#ifndef FIRMATA_DEBUG_H
#define FIRMATA_DEBUG_H

#ifdef SERIAL_DEBUG
  #define DEBUG_BEGIN(baud) Serial.begin(baud); while(!Serial) {;}
  #define DEBUG_PRINTLN(x)  Serial.println (x)
  #define DEBUG_PRINT(x)    Serial.print (x)
#else
  #define DEBUG_BEGIN(baud)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT(x)
#endif

#endif /* FIRMATA_DEBUG_H */
