 /**
 * @file TFT_WDT.c
 * @brief Definitions TFT_WDT Demo for Fireduino
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
#include "Wire.h"
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


void clear_pointer(void)
{
	unsigned char i;
	for(i = 0;i<60;i++)
	{
		TFTscreen.line(Lcd_TimeX(64,48,i) ,Lcd_TimeY(64,48,i) ,64,64);
		TFTscreen.line(Lcd_TimeX(64,34,i) ,Lcd_TimeY(64,34,i) ,64,64);
		TFTscreen.line(Lcd_TimeX(64,24,i) ,Lcd_TimeY(64,24,i) ,64,64);
	}
}

void clear_txt(unsigned char flash_bit)
{
	switch(flash_bit)
	{
		case 0:
			TFTscreen.fillRect(90, 140,22,14,0);
		break;

		case 1:
			TFTscreen.fillRect(54, 140,22,14,0);
		break;

		case 2:
			TFTscreen.fillRect(18, 140,22,14,0);
		break;

		default:
			TFTscreen.fillRect(18, 140,94,14,0);
		break;
	}
}

void draw_txt(RTCTime *time,unsigned char flash_bit)
{
	char temp[9];
	switch(flash_bit)
	{
		case 0:
//			TFTscreen.fillRect(90, 140,22,14,0);
			sprintf(temp,"%02d",time->sec);
			temp[3] = 0;
			TFTscreen.text(temp, 90, 140);
		break;

		case 1:
//			TFTscreen.fillRect(54, 140,22,14,0);
			sprintf(temp,"%02d",time->min);
			temp[3] = 0;
			TFTscreen.text(temp, 54, 140);
		break;

		case 2:
//			TFTscreen.fillRect(18, 140,22,14,0);
			sprintf(temp,"%02d",time->hour);
			temp[3] = 0;
			TFTscreen.text(temp, 18, 140);
		break;

		default:
			sprintf(temp,"%02d:%02d:%02d",time->hour,time->min,time->sec);
			temp[8] = 0;
			TFTscreen.text(temp, 18, 140);
		break;
	}
}

void draw_pointer(RTCTime *time)
{
//	char temp[9];
	TFTscreen.line(Lcd_TimeX(64,48,time->sec) ,Lcd_TimeY(64,48,time->sec) ,64,64);
	TFTscreen.line(Lcd_TimeX(64,34,time->min) ,Lcd_TimeY(64,34,time->min) ,64,64);
	TFTscreen.line(Lcd_TimeX(64,24,time->min/10 + ((time->hour%12)*5)),Lcd_TimeY(64,24,time->min/10 +((time->hour%12)*5)),64,64);
//	sprintf(temp,"%02d:%02d:%02d",time->hour,time->min,time->sec);
//	temp[8] = 0;
//	TFTscreen.text(temp, 18, 140);
}

void draw_time(RTCTime *time)
{
	char temp[9];
	TFTscreen.line(Lcd_TimeX(64,48,time->sec) ,Lcd_TimeY(64,48,time->sec) ,64,64);
	TFTscreen.line(Lcd_TimeX(64,34,time->min) ,Lcd_TimeY(64,34,time->min) ,64,64);
	TFTscreen.line(Lcd_TimeX(64,24,time->min/10 + ((time->hour%12)*5)),Lcd_TimeY(64,24,time->min/10 +((time->hour%12)*5)),64,64);
	sprintf(temp,"%02d:%02d:%02d",time->hour,time->min,time->sec);
	temp[8] = 0;
	TFTscreen.text(temp, 18, 140);
}

#define BEEP  4
RTCTime time;
RTCTime alarm_time;
void setup() {


  Serial.begin(115200);

//  delay(1);
  TFTscreen.begin();
//  delay(1);
  // clear the screen with a pretty color
  TFTscreen.background(0, 0, 0);
  TFTscreen.setTextSize(2);
  TFTscreen.stroke(255,255,255);

  draw_dial();
//  draw_pointer(&time);
//  delay(100);
  Wire1.begin();
  RTC.getTime(&time);
  draw_time(&time);

  pinMode(BEEP,OUTPUT);
  digitalWrite(BEEP,HIGH);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);

	alarm_time.sec = time.sec+10;
	alarm_time.min = time.min;
	alarm_time.hour = time.hour;
	if(alarm_time.sec > 59)
	{
		alarm_time.min++;
		alarm_time.sec =- 59;
	}
}

unsigned int curr_ms;
unsigned int curr_ms_key;

unsigned char curr_mode;
unsigned char event_key;

#define MODE_DISPALY_TIME	0
#define MODE_MODIFY_TIME	2
#define MODE_MODIFY_ALARM	1
#define MODE_ALARM			3
#define MODE_DISPALY_ALARM	4

#define EVENT_MODE			1
#define EVENT_UP			2
#define EVENT_DOWN          3
#define EVENT_MODE_CHANG 	4
#define EVENT_MODE_CLICK	5

#define MODIFY_SEC			0
#define MODIFY_MIN			1
#define MODIFY_HOUR			2

#define KEY_MODE	0x01
#define KEY_UP		0x02
#define KEY_DOWN	0x04

unsigned char modify_bit;
unsigned char display_flag;
unsigned char mode_count;
unsigned char up_count;
unsigned char down_count;

unsigned char mode_count_i;
unsigned char up_count_i;
unsigned char down_count_i;

unsigned char key_trg;
unsigned char key_count;

void key_sacn(void)
{
	unsigned char pin_level = 0;
	if(digitalRead(5) == HIGH)
	{
//		Serial.println("digitalRead(5) == HIGH");
		pin_level |= KEY_MODE;
	}
	if(digitalRead(6) == HIGH)
	{
//		Serial.println("digitalRead(6) == HIGH");
		pin_level |= KEY_UP;
	}
	if(digitalRead(7) == HIGH)
	{
//		Serial.println("digitalRead(7) == HIGH");
		pin_level |= KEY_DOWN;
	}

	key_trg = pin_level&(pin_level^key_count);
	key_count = pin_level;
}

void key_proc(void)
{
	if(key_trg & KEY_UP)
	{
		Serial.println("KEY_UP click");
		event_key = EVENT_UP	;
	}

	if(key_trg & KEY_DOWN)
	{
		Serial.println("KEY_DOWN click");
		event_key = EVENT_DOWN	;
	}

	if(key_trg & KEY_MODE)
	{
		Serial.println("KEY_MODE click");
		event_key = EVENT_MODE_CLICK;
	}

	if((mode_count > 0 &&  mode_count < 50) && ( ((key_trg & KEY_MODE) == 0) & ((key_count & KEY_MODE) == 0)) )
	{
		Serial.println("KEY_MODE finsh click");
		event_key = EVENT_MODE	;
	}


	if(key_count & KEY_UP)
	{
		up_count++;
		if(up_count > 100)
		{
			up_count = 80 + up_count_i;
			if(up_count_i++ >100)
			{
				up_count_i = 100;
			}
			Serial.println("KEY_UP long  click");
			event_key = EVENT_UP;
		}
	}
	else
	{
		up_count = 0;
		up_count_i = 0;
	}

	if(key_count & KEY_DOWN)
	{
		down_count++;
		if(down_count > 100)
		{
			down_count = 80 + down_count_i;
			if(down_count_i++ >100)
			{
				down_count_i = 100;
			}
			Serial.println("KEY_DOWN long click");
			event_key = EVENT_DOWN;
		}
	}
	else
	{
		down_count = 0;
		down_count_i = 0;
	}

	if(key_count & KEY_MODE)
	{
		mode_count++;
		if(mode_count > 100)
		{
			mode_count = 50;
//			mode_count = 80 + mode_count_i;
//			if(mode_count_i++ >100)
//			{
//				mode_count_i = 100;
//			}
			Serial.println("KEY_MODE  long click");
			event_key = EVENT_MODE_CHANG;
		}
	}
	else
	{
		mode_count = 0;
		mode_count_i = 0;
	}
}

void event_proc(void)
{
	if(event_key)
	{
		if(curr_mode == MODE_ALARM)
		{
			digitalWrite(BEEP,HIGH);
			curr_mode = MODE_DISPALY_TIME;
			return;
		}
		switch(event_key)
		{
			case EVENT_MODE_CLICK :
				if(curr_mode == MODE_DISPALY_TIME)
				{
					curr_mode = MODE_DISPALY_ALARM;
					TFTscreen.stroke(0, 0, 0);
					clear_pointer();
					clear_txt(3);
					TFTscreen.stroke(255,255,0);
					draw_time(&alarm_time);
				}
			break;
			case EVENT_MODE :
				Serial.println("EVENT_MODE...");
				if(curr_mode == MODE_DISPALY_ALARM)
				{
					curr_mode = MODE_DISPALY_TIME;
					TFTscreen.stroke(0, 0, 0);
					clear_pointer();
					clear_txt(3);
					TFTscreen.stroke(255,255,255);
					draw_time(&time);
				}
				else if(curr_mode != MODE_DISPALY_TIME)
				{
					TFTscreen.stroke(0,0,0);
					clear_pointer();
					clear_txt(modify_bit);
					if(curr_mode == MODE_MODIFY_TIME)
					{
						TFTscreen.stroke(255,255,255);
						draw_pointer(&time);
						draw_txt(&time,modify_bit);
					}
					else if(curr_mode == MODE_MODIFY_ALARM)
					{
						TFTscreen.stroke(255,255,0);
						draw_pointer(&alarm_time);
						draw_txt(&alarm_time,modify_bit);
					}
					display_flag = 0;
					modify_bit ++;
					if(modify_bit > 2)
						modify_bit = 0;
					Serial.print("modify_bit:");
					Serial.println(modify_bit);
				}
				break;
			case EVENT_UP :
				Serial.println("EVENT_UP...");
				if((curr_mode & 0x03) == MODE_MODIFY_TIME)
				{
					TFTscreen.stroke(0,0,0);
					clear_pointer();
					clear_txt(modify_bit);
					switch(modify_bit)
					{
						case MODIFY_SEC :
							time.sec ++;
							if(time.sec > 59)
								time.sec = 0;
							break;
						case MODIFY_MIN	:
							time.min ++;
							if(time.min > 59)
								time.min = 0;
							break;
						case MODIFY_HOUR :
							time.hour ++;
							if(time.hour > 23)
								time.hour = 0;
							break;
					}
					TFTscreen.stroke(255,255,255);
					draw_pointer(&time);
					draw_txt(&time,modify_bit);
				}
				else if((curr_mode & 0x03) == MODE_MODIFY_ALARM)
				{
					TFTscreen.stroke(0,0,0);
					clear_pointer();
					clear_txt(modify_bit);
					switch(modify_bit)
					{
						case MODIFY_SEC :
							alarm_time.sec ++;
							if(alarm_time.sec > 59)
								alarm_time.sec = 0;
							break;
						case MODIFY_MIN	:
							alarm_time.min ++;
							if(alarm_time.min > 59)
								alarm_time.min = 0;
							break;
						case MODIFY_HOUR :
							alarm_time.hour ++;
							if(alarm_time.hour > 23)
								alarm_time.hour = 0;
							break;
					}
					TFTscreen.stroke(255,255,0);
					draw_pointer(&alarm_time);
					draw_txt(&alarm_time,modify_bit);
				}
				break;
			case EVENT_DOWN :
				Serial.println("EVENT_DOWN...");
				if((curr_mode & 0x03) == MODE_MODIFY_TIME)
				{
					TFTscreen.stroke(0,0,0);
					clear_pointer();
					clear_txt(modify_bit);
					switch(modify_bit)
					{
						case MODIFY_SEC :
							time.sec --;
							if(time.sec == 255)
								time.sec = 59;
							break;
						case MODIFY_MIN	:
							time.min --;
							if(time.min == 255)
								time.min = 59;
							break;
						case MODIFY_HOUR :
							time.hour --;
							if(time.hour == 255)
								time.hour = 23;
							break;
					}
					TFTscreen.stroke(255,255,255);
					draw_pointer(&time);
					draw_txt(&time,modify_bit);
				}
				else if((curr_mode & 0x03) == MODE_MODIFY_ALARM)
				{
					TFTscreen.stroke(0,0,0);
					clear_pointer();
					clear_txt(modify_bit);
					switch(modify_bit)
					{
						case MODIFY_SEC :
							alarm_time.sec --;
							if(alarm_time.sec == 255)
								alarm_time.sec = 59;
							break;
						case MODIFY_MIN	:
							alarm_time.min --;
							if(alarm_time.min == 255)
								alarm_time.min = 59;
							break;
						case MODIFY_HOUR :
							alarm_time.hour --;
							if(alarm_time.hour == 255)
								alarm_time.hour = 23;
							break;
					}
					TFTscreen.stroke(255,255,0);
					draw_pointer(&alarm_time);
					draw_txt(&alarm_time,modify_bit);
				}
				break;
			case EVENT_MODE_CHANG :
				Serial.println("EVENT_MODE_CHANG...");
				if(curr_mode == MODE_DISPALY_ALARM)
				{
					curr_mode = MODE_MODIFY_ALARM;
					modify_bit = 0;
				}
				else
				{
					if(curr_mode == MODE_MODIFY_TIME)
					{
						RTC.setTime(&time);
					}
					modify_bit = 0;
					curr_mode++;
					if(curr_mode > 2)
						curr_mode = 0;
				}
				TFTscreen.stroke(0,0,0);
				clear_pointer();
				clear_txt(3);

				if(curr_mode == MODE_MODIFY_TIME || curr_mode == MODE_DISPALY_TIME)
				{
					TFTscreen.stroke(255,255,255);
					draw_pointer(&time);
					draw_txt(&time,3);
				}
				else if(curr_mode == MODE_MODIFY_ALARM || curr_mode == MODE_DISPALY_ALARM)
				{
					TFTscreen.stroke(255,255,0);
					draw_pointer(&alarm_time);
					draw_txt(&alarm_time,3);
				}

				Serial.println(curr_mode);
				break;

			default:

				break;
		}

		event_key = 0;
	}
	else
	{

	}
}

void display_proc(void)
{
	switch(curr_mode)
	{
		case MODE_DISPALY_TIME:
			TFTscreen.stroke(0, 0, 0);
			clear_pointer();
			clear_txt(3);
			RTC.getTime(&time);
			TFTscreen.stroke(255,255,255);
			draw_time(&time);
			if(time.sec == alarm_time.sec && time.min == alarm_time.min && time.hour == alarm_time.hour)
			{
				curr_mode = MODE_ALARM;
			}
			break;

		case MODE_DISPALY_ALARM:
//			TFTscreen.stroke(0, 0, 0);
//			clear_pointer();
//			clear_txt(3);
//			TFTscreen.stroke(255,255,0);
//			draw_time(&alarm_time);
			if(display_flag ^= 0x01)
			{

			}else
			{

			}
			break;

		case MODE_MODIFY_TIME :
			if(display_flag ^= 0x01)
			{
				TFTscreen.stroke(0, 0, 0);
				clear_pointer();
				clear_txt(modify_bit);
				Serial.println("stroke(0, 0, 0);");
			}else
			{
				TFTscreen.stroke(255,255,255);
				draw_pointer(&time);
				draw_txt(&time,modify_bit);
				Serial.println("stroke(255,255,255);");
			}
			break;

		case MODE_MODIFY_ALARM :
			if(display_flag ^= 0x01)
			{
				TFTscreen.stroke(0, 0, 0);
				clear_pointer();
				clear_txt(modify_bit);
				Serial.println("stroke(0, 0, 0);");
			}else
			{
				TFTscreen.stroke(255,255,0);
				draw_pointer(&alarm_time);
				draw_txt(&alarm_time,modify_bit);
				Serial.println("stroke(255,255,255);");
			}
			break;
			break;
		case MODE_ALARM :
			if(display_flag ^= 0x01)
			{
				digitalWrite(BEEP,LOW);
				TFTscreen.stroke(0, 0, 0);
				clear_pointer();
				clear_txt(3);
			}else
			{
				digitalWrite(BEEP,HIGH);
				TFTscreen.stroke(255,0,0);
				draw_pointer(&alarm_time);
				draw_txt(&alarm_time,3);
			}
			break;

	}
}

void loop() {

	if(millis() > curr_ms)
	{
		curr_ms = millis() + 250;
//		Serial.println("loop");
		display_proc();
	}
	//20ms ¼ì²âÒ»´Î°´¼ü×´Ì¬
	if(millis() > curr_ms_key)
	{
		curr_ms_key = millis() + 20;
//		Serial.println(key_trg);
//		Serial.println(key_count);
		key_sacn();
		key_proc();
		event_proc();

	}
}

