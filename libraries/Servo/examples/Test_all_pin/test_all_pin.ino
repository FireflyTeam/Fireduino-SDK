 /**
 * @file test_all_pin.ino
 * @brief Definitions test_all_pin Demo for Fireduino
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
#include "Arduino.h"
#include <Servo.h>

Servo myservo[30];

void setup()
{
	int i;
	for(i = 0;i < 30;i++)
	{
		myservo[i].attach((i));
	}

	for(i = 0;i<30;i++)
	{
		myservo[i].write(90);
//		myservo[i].writeMicroseconds(1500);
	}

}

void loop()
{
	delay(1000);
}

