 /**
 * @file RTC_set_get.ino
 * @brief Definitions RTC_set_get Demo for Fireduino
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
#include "Wire.h"
#include "RTC.h"

RTCTime time;

void setup(void)
{
	Serial.begin(115200);
	Serial.print("\r\n FireDuino Setup ...\r\n");
	time.year  = 2016;//year (1900 ~ 2099)
	time.mon   = 3;//month,begin from 1 to 12
	time.week  = 1 ;//week (0:Sunday 1:Monday ... 6:Saturday)
	time.day   = 14;//day,begin from 1 to 31
	time.hour  = 23;//hour,24-hour
	time.min   = 59;//minute
	time.sec   = 50;//second
	Wire1.begin();
	RTC.setTime(&time);

}

void loop(void)
{
	delay(1000);

	RTC.getTime(&time);
	Serial.println();
	Serial.print("Y:");
	Serial.print(time.year,DEC);
	Serial.print(" M:");
	Serial.print(time.mon,DEC);
	Serial.print(" W:");
	Serial.print(time.week,DEC);
	Serial.print(" d:");
	Serial.print(time.day,DEC);
	Serial.print(" h:");
	Serial.print(time.hour,DEC);
	Serial.print(" m:");
	Serial.print(time.min,DEC);
	Serial.print(" s:");
	Serial.print(time.sec,DEC);
	Serial.println();

}




