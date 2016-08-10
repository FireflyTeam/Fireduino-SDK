 /**
 * @file RTC.h
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
#ifndef _ARDUINO_RTC_H
#define _ARDUINO_RTC_H

//datetimeInfo struct
typedef struct
{
	unsigned short year;//year (1900 ~ 2099)
	unsigned char  mon;//month,begin from 1 to 12
	unsigned char  week;//week (0:Sunday 1:Monday ... 6:Saturday)
	unsigned char  day;//day,begin from 1 to 31
	unsigned char  hour;//hour,24-hour
	unsigned char  min;//minute
	unsigned char  sec;//second
}RTCTime;

class RTCClass
{
public:
	RTCClass();

public:
	void getTime(RTCTime *time);
	void setTime(RTCTime *time);
};

extern RTCClass RTC;
#endif


