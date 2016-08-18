 /**
 * @file TFT_Time.ino
 * @brief Definitions TFT_Time Demo for Fireduino
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
#include "RTC.h"

TFT TFTscreen = TFT();


unsigned char Lcd_TimeX(unsigned char circle_x,unsigned char Length,unsigned char Angle)
{
   unsigned char x;
   if((Angle>0) && (Angle<=15))
   {
     x = circle_x + Length * (sin(PI * Angle / 30));
   }
   else if(Angle > 15 && Angle <= 30)
   {
      x = circle_x + Length * cos((PI * Angle) / 30 - (PI / 2 ));
   }
   else if(Angle > 30 && Angle <= 45)
   {
       x = circle_x - Length * sin((PI * Angle) / 30- PI);
   }
   else
   {
       x = circle_x-Length * cos((PI * Angle) / 30 - ((3 * PI) / 2));
   }
        return x;
}

unsigned char Lcd_TimeY(unsigned char circle_y,unsigned char Length,unsigned char Angle)
{
   unsigned char y;
   if((Angle>0) && (Angle<=15))
   {
      y = circle_y - Length * (cos(PI * Angle / 30));
   }
   else if(Angle > 15 && Angle <= 30)
   {
      y = circle_y + Length * sin((PI * Angle) / 30 - (PI / 2 ));
   }
   else if(Angle > 30 && Angle <= 45)
   {
       y = circle_y + Length * cos((PI * Angle) / 30- PI);
   }
   else
   {
       y = circle_y - Length * sin((PI * Angle) / 30 - ((3 * PI) / 2));
   }
   return y;
}

void draw_dial(void)
{
	unsigned int i ;
	TFTscreen.circle(64,64,62);
	TFTscreen.circle(64,64,63);

	for(i=0;i<60;i++)
	{
	  if((i%5) == 0)
	  {
		  TFTscreen.line(Lcd_TimeX(64,62,i),Lcd_TimeY(64,62,i),Lcd_TimeX(64,58,i),Lcd_TimeY(64,58,i));
	  }

	}
}

void draw_pointer(RTCTime *time)
{
	char temp[9];
	TFTscreen.line(Lcd_TimeX(64,48,time->sec) ,Lcd_TimeY(64,48,time->sec) ,64,64);
	TFTscreen.line(Lcd_TimeX(64,34,time->min) ,Lcd_TimeY(64,34,time->min) ,64,64);
	TFTscreen.line(Lcd_TimeX(64,24,time->min/10 + ((time->hour%12)*5)),Lcd_TimeY(64,24,time->min/10 +((time->hour%12)*5)),64,64);
	sprintf(temp,"%02d:%02d:%02d",time->hour,time->min,time->sec);
	temp[8] = 0;
	TFTscreen.text(temp, 18, 140);
}

RTCTime time;
void setup() {

	time.hour  = 00;//hour,24-hour
	time.min   = 00;//minute
	time.sec   = 00;//second
  // initialize the serial port
  Serial.begin(115200);

  delay(1);
  TFTscreen.begin();
  delay(1);
  // clear the screen with a pretty color
  TFTscreen.background(0, 0, 0);
  TFTscreen.setTextSize(2);
  TFTscreen.stroke(255,255,255);

  draw_dial();
  draw_pointer(&time);

}

void loop() {

	delay(1000);
	Serial.println("loop");
	TFTscreen.stroke(0, 0, 0);
	draw_pointer(&time);

	time.sec++;
	if(time.sec >= 60)
	{
		time.sec = 0;
		time.min++;
		if(time.min >=60)
		{
			time.min = 0;
			time.hour ++;
			if(time.hour >= 24)
			{
				time.hour = 0;
			}
		}
	}

	TFTscreen.stroke(255,255,255);
	draw_pointer(&time);

}

