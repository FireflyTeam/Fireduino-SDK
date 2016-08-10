 /**
 * @file RTC.cpp
 * @brief Definitions RTC class for Fireduino
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
#include "RTC.h"
#include "Wire.h"


#define HYM8563ADDR		0x51
#define BIN2BCD(bcd)	((((bcd)/10)<<4) + ((bcd) %10))
#define BCD2BIN(bin)	(((((bin)&0xF0)>>4)*10) + ((bin)&0x0F))
RTCClass::RTCClass()
{

}

void RTCClass::getTime(RTCTime *time)
{
	unsigned char time_regs[7];

	Wire1.beginTransmission(HYM8563ADDR);
	Wire1.write(0x02);
	Wire1.endTransmission();
	Wire1.requestFrom(HYM8563ADDR,7);
	if(Wire1.available()<=7);
	{
		time_regs[0] = Wire1.read();
		time_regs[1] = Wire1.read();
		time_regs[2] = Wire1.read();
		time_regs[3] = Wire1.read();
		time_regs[4] = Wire1.read();
		time_regs[5] = Wire1.read();
		time_regs[6] = Wire1.read();
	}

	time->sec   = BCD2BIN(time_regs[0] & 0x7F);
	time->min   = BCD2BIN(time_regs[1] & 0x7F);
	time->hour  = BCD2BIN(time_regs[2] & 0x3F);
	time->day   = BCD2BIN(time_regs[3] & 0x3F);
	time->week  = BCD2BIN(time_regs[4] & 0x07);
	time->mon   = BCD2BIN(time_regs[5] & 0x1F);
	time->year  = BCD2BIN(time_regs[6] & 0xFF);
	if(time_regs[5] & 0x80)
	{
		time->year += 1900;
	}
	else
	{
		time->year += 2000;
	}

}

void RTCClass::setTime(RTCTime *time)
{
	unsigned char time_regs[7];

	if(time->sec >= 60 || time->sec < 0 )	//set  sec
		time_regs[0x00] = BIN2BCD(0x00);
	else
		time_regs[0x00] = BIN2BCD(time->sec);
	
	if(time->min >= 60 || time->min < 0 )	//set  min
		time_regs[0x01] = BIN2BCD(0x00);
	else
		time_regs[0x01] = BIN2BCD(time->min);

	if(time->hour >= 24 || time->hour < 0 )	//set  hour
		time_regs[0x02] = BIN2BCD(0x00);
	else
		time_regs[0x02] = BIN2BCD(time->hour);
	
	if(time->day > 31 || time->day < 0 )    //set day
		time_regs[0x03] = BIN2BCD(0x01);
	else
		time_regs[0x03] = BIN2BCD(time->day);

	if(time->week > 31 || time->week < 0 )	//set week
		time_regs[0x04] = BIN2BCD(0x00);
	else
		time_regs[0x04] = BIN2BCD(time->week);

	if(time->mon > 12 || time->mon < 1 )    //set mon
		time_regs[0x05] = BIN2BCD(0x01);
	else
		time_regs[0x05] = BIN2BCD(time->mon);

	if(time->year > 2100)					//set yesr
	{
		time_regs[0x06] = BIN2BCD(99);
	}
	else if(time->year >= 2000)
	{
		time_regs[0x06] = BIN2BCD(time->year-2000);
	}
	else if(time->year >= 1900)
	{
		time_regs[0x06] = BIN2BCD(time->year-1900);
		time_regs[0x05] |= 0x80;
	}
	else
	{
		time_regs[0x06] = BIN2BCD(0);
		time_regs[0x05] |= 0x80;
	}

	Wire1.beginTransmission(HYM8563ADDR);
	Wire1.write(0x02);
	Wire1.write(time_regs,7);
	Wire1.endTransmission();
}

RTCClass RTC;




