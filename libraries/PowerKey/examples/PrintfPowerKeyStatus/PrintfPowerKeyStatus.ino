 /**
 * @file PrintfPowerKeyStatus.ino
 * @brief Definitions TFTDisplayText class for Fireduino
 * @author huangab<hab@t-chip.com.cn>
 * @version V1.0
 * @date 2016.02
 *
 * @par Copyright:
 * Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
 *
 * For more information, please visit website <http://www.t-firefly.com/>, \n\n
 * or email to <service@t-firefly.com>.
 */
#include <PowerKey.h>
#include "Arduino.h"

#define	ADEBUG(x)	Serial.print(x)
#define	ADEBUGLN(x)	Serial.println(x)
#define	ADEBUGDEX(x)	Serial.print(x,HEX)

void callback(unsigned int even){
	switch(even)
	{
	case	EVEN_POWERKEY_PRESS_START	:
		ADEBUGLN("EVEN_POWERKEY_PRESS_START");
		break;
	case	EVEN_POWERKEY_PRESS	:
		ADEBUGLN("EVEN_POWERKEY_PRESS");
		break;
	case	EVEN_POWERKEY_RELEASE_SHORT	:
		ADEBUGLN("EVEN_POWERKEY_RELEASE_SHORT");
		break;
	case	EVEN_POWERKEY_RELEASE_LONG	:
		ADEBUGLN("EVEN_POWERKEY_RELEASE_LONG");
		break;
	case	EVEN_POWERKEY_DOUBLE_CLINK	:
		ADEBUGLN("EVEN_POWERKEY_DOUBLE_CLINK");
		break;
	default	:
		ADEBUGLN("Unknow even");
		break;
	}
}

void setup(){
	Serial.begin(115200);
	ADEBUGLN("setup");
	pinMode(3,OUTPUT);
	PowerKey.begin(callback);
}

void loop(){
	togglePin(3);
	delay(1000);
}




