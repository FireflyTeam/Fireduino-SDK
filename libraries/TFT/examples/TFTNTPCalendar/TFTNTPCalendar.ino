#include <TFT.h>  // Arduino LCD library
#include "RTC.h"
#include "WiFi.h"
#include <WiFiUdp.h>
#include "Wire.h"

// define for calculate time
#define SECONDS_IN_DAY          86400
#define START_YEAR              1900
#define TIME_ZONE               +8

// status of display
#define MODE_DISPLAY_TIME	0
#define MODE_DISPLAY_CAL	1
#define	MODE_BROWSE_CAL		2
//#define MODE_DGET_NTP_TIME_FAILD	3

// event of key
#define EVENT_MODE			1
#define EVENT_UP			2
#define EVENT_DOWN          3
#define EVENT_MODE_CHANG 	4
#define EVENT_MODE_CLICK	5

// Key flag
#define KEY_MODE	0x01
#define KEY_UP		0x02
#define KEY_DOWN	0x04

#define	UP_YEAR				0
#define	DOWN_YEAR			1
#define	UP_MON				2
#define	DOWN_MON			3

// count ms of key press
unsigned int curr_ms_test;
unsigned int curr_ms;
unsigned int curr_ms_key;

// current status
unsigned char curr_mode;
unsigned char event_key;

// counts of key press
unsigned char mode_count;
unsigned char up_count;
unsigned char down_count;

unsigned char mode_count_i;
unsigned char up_count_i;
unsigned char down_count_i;

// flag of keys
unsigned char key_trg;
unsigned char key_count;

unsigned char modif_bit = 0;

// for display the month
static int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
String mon_list[] = {"   ","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

// WIFI information
int status = WL_IDLE_STATUS;
char ssid[] = "Fireduino";  //  your network SSID (name)
char pass[] = "12345678";       // your network password

// Get NTP time status
unsigned char getNtpTimeSuccess;

unsigned int localPort = 2390;      // local port to listen for UDP packets

IPAddress timeServer(128, 138, 140, 44); // time-nw.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

RTCTime time;
RTCTime browseTime;
String cal_char[6][7];
String browse_cal_char[6][7];

TFT TFTscreen = TFT();

// function for WIFI
void printWifiStatus();

// function for NTP Time
unsigned long sendNTPpacket(IPAddress& address);

RTCTime * timeStamp2time(unsigned int timeStamp ,RTCTime *time);

void ntpTime(void);

// function for display
unsigned char Lcd_TimeX(unsigned char circle_x,unsigned char Length,unsigned char Angle);
unsigned char Lcd_TimeY(unsigned char circle_y,unsigned char Length,unsigned char Angle);
void clear_all(void);
void clear_dial(void);
void clear_time_pointer(void);
void clear_time_txt(unsigned char flash_bit);
void draw_dial(void);
void draw_time_pointer(RTCTime *time);
void draw_time_txt(RTCTime *time);
void draw_cal(RTCTime *time);
void draw_browse_cal(RTCTime *time);

// function of key event
void key_sacn(void);
void key_proc(void);
void event_proc(void);
void display_proc(void);

void setup() {

  // initialize the serial port
  Serial.begin(115200);

  // initialize the TFT Screen
  delay(1);
  TFTscreen.begin();
  Serial.println("TFTscreen init success");

  delay(1);
  // clear the screen with a pretty color
  TFTscreen.background(0, 0, 0);
  TFTscreen.setTextSize(1);
  TFTscreen.stroke(255,255,255);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    TFTscreen.text("Connecting  ",28,70);
    TFTscreen.text("    ...     ",28,90);
    // Connect to network.
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
  TFTscreen.fillRect(0,70,128,40,0);

  // initialize UDP
  Serial.println("\nStarting connection to server...");
  Udp.begin(localPort);

  // Init Wire and RTC will use I2C Interface
  Wire1.begin();

  getNtpTimeSuccess = 0;

  // Get NTP time
  ntpTime();

  // if failed ,note user
  if (!getNtpTimeSuccess)
  {
	  TFTscreen.stroke(255,0,0);
	  TFTscreen.textSize(1);
	  TFTscreen.text("get NTP Time",28,70);
	  TFTscreen.text("failed",46,80);
	  delay(3000);
	  TFTscreen.stroke(0,0,0);
	  TFTscreen.text("get NTP Time",28,70);
	  TFTscreen.text("failed",46,80);
	  TFTscreen.stroke(255,255,255);
	  TFTscreen.textSize(2);
  }
  else
	  RTC.setTime(&time);

  // display the dial
  draw_dial();
}

void loop() {

	if(millis() > curr_ms)
	{
		curr_ms = millis() + 250;
//		displaytime();
		display_proc();
	}
	//20ms ¼ì²âÒ»´Î°´¼ü×´Ì¬
	if(millis() > curr_ms_key)
	{
		curr_ms_key = millis() + 20;
		key_sacn();
		key_proc();
		event_proc();

	}


}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;


  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Serial.print("Query NTP server ");
  Serial.print(address);
  Serial.print(" at port ");
  Serial.println(123);
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

boolean isLeapYear(unsigned int year) {
  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

RTCTime * timeStamp2time(unsigned int timeStamp ,RTCTime *time)
{
	  unsigned int week = (((timeStamp / SECONDS_IN_DAY) + 1)% 7);

	  unsigned int year = START_YEAR;
	  while(1) {
	    uint32_t seconds;
	    if(isLeapYear(year)) seconds = SECONDS_IN_DAY * 366;
	    else seconds = SECONDS_IN_DAY * 365;
	    if(timeStamp >= seconds) {
	      timeStamp -= seconds;
	      year++;
	    } else break;
	  }

	  unsigned int month = 0;
	  while(1) {
	    uint32_t seconds = SECONDS_IN_DAY * days_in_month[month];
	    if(isLeapYear(year) && month == 1) seconds = SECONDS_IN_DAY * 29;
	    if(timeStamp >= seconds) {
	      timeStamp -= seconds;
	      month++;
	    } else break;
	  }
	  month++;

	  unsigned int day = 1;
	  while(1) {
	    if(timeStamp >= SECONDS_IN_DAY) {
	      timeStamp -= SECONDS_IN_DAY;
	      day++;
	    } else break;
	  }

	  unsigned int hour = timeStamp / 3600;
	  unsigned int minute = (timeStamp - (uint32_t)hour * 3600) / 60;
	  unsigned int second = (timeStamp - (uint32_t)hour * 3600) - minute * 60;


	  time->year = year;//year (1900 ~ 2099)
	  time->mon  = month;//month,begin from 1 to 12
	  time->week = week;//week (0:Sunday 1:Monday ... 6:Saturday)
	  time->day  = day;//day,begin from 1 to 31
	  time->hour = hour;//hour,24-hour
	  time->min  = minute;//minute
	  time->sec  = second;//second

	  return time;
}

void upgrade_cal(unsigned int modif_mode,RTCTime *time){
//	static String browse_cal[][];
	switch(modif_mode)
	{
		case	UP_MON	:
			time->mon++;
			if (time->mon > 12)
				time->mon = 1;
			break;
		case	UP_YEAR	:
			time->year++;
			break;
		case	DOWN_MON	:
			time->mon--;
			if (time->mon < 1 || time->mon > 12)
				time->mon = 12;
			break;
		case	DOWN_YEAR	:
			time->year--;
			if (time->year < 1583)
				time->year = 1583;
			break;
		default	:
			break;

	}
}

void ntpTime(){

	sendNTPpacket(timeServer); // send an NTP packet to a time server

	TFTscreen.stroke(255,255,255);
	TFTscreen.textSize(1);
	TFTscreen.text("waitting for get",16,70);
	TFTscreen.text("NTP Time",40,80);
	delay(1000);

	if (Udp.parsePacket()) {
		Serial.println("packet received");
		// We've received a packet, read the data from it
		Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

		//the timestamp starts at byte 40 of the received packet and is four bytes,
		// or two words, long. First, esxtract the two words:
		unsigned long highWord = ((unsigned long)packetBuffer[40] << 8) | (unsigned long)packetBuffer[41];
		unsigned long lowWord = ((unsigned long)packetBuffer[42] << 8) | (unsigned long)packetBuffer[43];
		// combine the four bytes (two words) into a long integer
		// this is NTP time (seconds since Jan 1 1900):
		unsigned long secsSince1900 = (highWord << 16) | lowWord;
		Serial.print("Seconds since Jan 1 1900 = ");
		Serial.println(secsSince1900);

		timeStamp2time(secsSince1900 + (3600 * (TIME_ZONE)) , &time);
		browseTime = time;
		RTC.setTime(&time);
		getNtpTimeSuccess = 1;
	}
	else
		getNtpTimeSuccess = 0;

	RTC.getTime(&time);
	Serial.println();
	Serial.print("Y:");
	Serial.print(time.year,DEC);
	Serial.print(" M:");
	Serial.print(time.mon,DEC);
	Serial.print(" d:");
	Serial.print(time.day,DEC);
	Serial.print(" W:");
	Serial.print(time.week,DEC);
	Serial.print("    h:");
	Serial.print(time.hour,DEC);
	Serial.print(" m:");
	Serial.print(time.min,DEC);
	Serial.print(" s:");
	Serial.print(time.sec,DEC);
	Serial.println();

	TFTscreen.stroke(0,0,0);
	TFTscreen.text("waitting for get",16,70);
	TFTscreen.text("NTP Time",40,80);
	TFTscreen.stroke(255,255,255);
	TFTscreen.textSize(2);

}

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

void clear_all(void){
	TFTscreen.fillRect(0,0,128,160,0);
}

void clear_dial(void){
	unsigned int i ;

	TFTscreen.circle(64,48,46);
	TFTscreen.circle(64,48,45);

	for(i=0;i<60;i++)
	{
	  if((i%5) == 0)
	  {
		  TFTscreen.line(Lcd_TimeX(64,45,i),Lcd_TimeY(48,45,i),Lcd_TimeX(64,42,i),Lcd_TimeY(48,42,i));
	  }

	}

}

void clear_time_pointer(void)
{
	unsigned char i;
	for(i = 0;i<60;i++)
	{
		TFTscreen.line(Lcd_TimeX(64,36,i) ,Lcd_TimeY(48,36,i) ,64,48);
		TFTscreen.line(Lcd_TimeX(64,30,i) ,Lcd_TimeY(48,30,i) ,64,48);
		TFTscreen.line(Lcd_TimeX(64,20,i) ,Lcd_TimeY(48,20,i) ,64,48);
	}
}

void clear_time_txt(unsigned char flash_bit)
{
	switch(flash_bit)
	{
		case 0:
			TFTscreen.fillRect(44,100,33,14,0);
		break;

		case 1:
			TFTscreen.fillRect(6,120,110,14,0);
		break;

		case 2:
			TFTscreen.fillRect(18, 140,94,14,0);
		break;

		case 3:
			TFTscreen.fillRect(0, 100,128,60,0);
		break;

		default:
			TFTscreen.fillRect(18, 140,94,14,0);
		break;
	}
}

void draw_dial(void)
{
	unsigned int i ;

	TFTscreen.circle(64,48,46);
	TFTscreen.circle(64,48,45);

	for(i=0;i<60;i++)
	{
	  if((i%5) == 0)
	  {
		  TFTscreen.line(Lcd_TimeX(64,45,i),Lcd_TimeY(48,45,i),Lcd_TimeX(64,42,i),Lcd_TimeY(48,42,i));
	  }

	}
}

void draw_time_pointer(RTCTime *time)
{
	TFTscreen.line(Lcd_TimeX(64,36,time->sec) ,Lcd_TimeY(48,36,time->sec) ,64,48);
	TFTscreen.line(Lcd_TimeX(64,30,time->min) ,Lcd_TimeY(48,30,time->min) ,64,48);
	TFTscreen.line(Lcd_TimeX(64,20,time->min/10 + ((time->hour%12)*5)),Lcd_TimeY(48,20,time->min/10 +((time->hour%12)*5)),64,48);
}

void draw_time_txt(RTCTime *time){

	char temp[9];

	String week[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

	TFTscreen.text(week[time->week].c_str(), (64-18), 100);

	sprintf(temp,"%04d-%02d-%02d",time->year,time->mon,time->day);
	temp[10] = 0;
	TFTscreen.text(temp, 6, 120);


	sprintf(temp,"%02d:%02d:%02d",time->hour,time->min,time->sec);
	temp[8] = 0;
	TFTscreen.text(temp, 18, 140);
}

unsigned int get_ref_week_for_mon(RTCTime *time){
	// w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
//	unsigned int week;
	int year;
	int century;
	int mon;
	int ref_week;

	year = time->year%100;
	century = time->year/100;
	if (time->mon < 3)
	{
		mon = time->mon + 12;
		year -= 1;
	}
	else
		mon = time->mon;

	ref_week = ((year + (year/4) + (century/4) - 2*century + (26*(mon + 1)/10)) % 7);

	while (ref_week < 0)
		ref_week += 7;

//	Serial.print("century : ");
//	Serial.println(century);
//	Serial.print("year : ");
//	Serial.println(year);
//	Serial.print("mon : ");
//	Serial.println(mon);
//	Serial.print("ref_week : ");
//	Serial.println(ref_week);


	return (unsigned int)ref_week;
}

unsigned int get_cal_char(RTCTime *time,String *cal){
	unsigned int ref_week;
	unsigned int last_month;
	unsigned int next_day;
	unsigned int mon_tail;
	unsigned int cnt_x,cnt_y;
	unsigned int cur_x,cur_y;

	ref_week = get_ref_week_for_mon(time);
//
//	if (time->day-ref_week > 0)
//		cur_x = (time->day-ref_week)/7;
//	else
	if (time->mon == 1 || time->mon == 3 || time->mon == 5 || time->mon == 7 || time->mon == 8 || \
			time->mon == 10 || time->mon == 12)
		mon_tail = 31;
	else if (time->mon == 4 || time->mon == 6 || time->mon == 9 || time->mon == 11)
		mon_tail = 30;
	else if (isLeapYear(time->year))//time->mon == 2 && time->year/4 == 0)
	{
//		if (time->year/100 == 0 && time->year/400 != 0)
		mon_tail = 29;
//		else
//		mon_tail = 28;
	}
	else
		mon_tail = 28;

//	Serial.print("mon_tail : ");
//	Serial.println(mon_tail);
	for (cnt_y = 0;cnt_y < ref_week;cnt_y++)
		*(cal + cnt_y) = " ";
	for (cnt_y = ref_week;cnt_y < 7 ;cnt_y++)
		*(cal + cnt_y)  = String(cnt_y-ref_week + 1);
	next_day = cnt_y-ref_week + 1;
	for (cnt_x = 1;cnt_x < 6;cnt_x++)
	{
		if ((mon_tail - next_day) >= 7)
		{
			for (cnt_y = 0;cnt_y<7;cnt_y++)
				*(cal + cnt_x*7+cnt_y)  = String(next_day+cnt_y);
			next_day += 7;
//			Serial.print("next_day : ");
//			Serial.println(next_day);
		}
		else
		{
			for (cnt_y = 0;cnt_y < (mon_tail - next_day + 1);cnt_y++)
				*(cal + cnt_x*7+cnt_y) = String(next_day+cnt_y);
			for (cnt_y = (mon_tail - next_day + 1);cnt_y < 7;cnt_y++)
				*(cal + cnt_x*7+cnt_y) = " ";
			cnt_x++;
			while (cnt_x < 6)
			{
				for (cnt_y = 0;cnt_y < 7;cnt_y++)
					*(cal + cnt_x*7+cnt_y) = " ";
				cnt_x++;
			}
		}
	}
	return ref_week;

}

void draw_cal(RTCTime *time){
	//save the cal string to display
//	String cal_char[5][7];
	// cale the week of day 1
	unsigned int ref_week;
	unsigned int first_sat;
	unsigned int cnt_x,cnt_y;
	int cur_x,cur_y;

	ref_week = get_cal_char(time,&cal_char[0][0]);
	first_sat = 7 - ref_week;
//	Serial.print("ref_week : ");
//	Serial.println(ref_week);
//	Serial.print("first_sat : ");
//	Serial.println(first_sat);
//	Serial.print("time->day : ");
//	Serial.println(time->day);

	cur_x = (time->day - first_sat + 6)/7;
	cur_y = time->week;


	for (cnt_x = 0;cnt_x <6;cnt_x++)
		for (cnt_y = 0;cnt_y < 7;cnt_y++)
		{
			TFTscreen.text(cal_char[cnt_x][cnt_y].c_str(),cnt_y*18+4,cnt_x*18+60);
//			Serial.print("X:");
//			Serial.print(cnt_x);
//			Serial.print("y:");
//			Serial.print(cnt_y);
//			Serial.print("value:");
//			Serial.println(cal_char[cnt_x][cnt_y].c_str());
		}
//
//	Serial.print("cur_x : ");
//	Serial.print(cur_x);
//	Serial.print("  cur_y : ");
//	Serial.println(cur_y);

	TFTscreen.stroke(0,0,0);
	TFTscreen.fillRect(cur_y*18+1,cur_x*18+57,18,12,0);
	TFTscreen.stroke(255,255,255);
	TFTscreen.fillRect(cur_y*18+1,cur_x*18+57,18,12,255);
	TFTscreen.stroke(0,0,0);
	TFTscreen.text(cal_char[cur_x][cur_y].c_str(),(cur_y)*18+4,((cur_x)*18+60));
}

void draw_browse_cal(RTCTime *time){
	//save the cal string to display
//	String cal_char[5][7];
	// cale the week of day 1
	unsigned int ref_week;
	unsigned int cnt_x,cnt_y;
	unsigned int cur_x,cur_y;

	ref_week = get_cal_char(time,&browse_cal_char[0][0]);

	for (cnt_x = 0;cnt_x <6;cnt_x++)
		for (cnt_y = 0;cnt_y < 7;cnt_y++)
		{
			TFTscreen.text(browse_cal_char[cnt_x][cnt_y].c_str(),cnt_y*18+4,cnt_x*18+60);
//			Serial.print("X:");
//			Serial.print(cnt_x);
//			Serial.print("y:");
//			Serial.print(cnt_y);
//			Serial.print("value:");
//			Serial.println(cal_char[cnt_x][cnt_y].c_str());
		}
}

void draw_mon(RTCTime *time){
	TFTscreen.text(String(time->year).c_str(),8,15);

}

void draw_year(RTCTime *time){
	TFTscreen.text(mon_list[time->mon].c_str(),80,15);
}

void clear_mon(){
	TFTscreen.fill(0,0,0);
	TFTscreen.fillRect(80,15,33,15,0);
}

void clear_year(){
	TFTscreen.fill(0,0,0);
	TFTscreen.fillRect(8,15,44,15,0);
}

void draw_modif_cal(unsigned char modif_bit){
	static unsigned char modif_flag = 0;
	clear_year();
	clear_mon();
	if (modif_flag)
	{
		if (modif_bit)
		{
			TFTscreen.stroke(0,255,0);
			draw_year(&browseTime);
		}
		else
		{
			TFTscreen.stroke(0,0,255);
			draw_mon(&browseTime);
		}
	}
	else
	{
		TFTscreen.stroke(0,0,255);
		draw_mon(&browseTime);
		TFTscreen.stroke(0,255,0);
		draw_year(&browseTime);
	}
	modif_flag = ~modif_flag;
}

void clear_modif_cal(unsigned char modif_bit){
	clear_year();
	clear_mon();
}

void key_sacn(void)
{
	unsigned char pin_level = 0;
	if(digitalRead(5) == HIGH)
	{
		pin_level |= KEY_MODE;
	}
	if(digitalRead(6) == HIGH)
	{
		pin_level |= KEY_UP;
	}
	if(digitalRead(7) == HIGH)
	{
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
		switch(event_key)
		{
			case EVENT_MODE_CLICK :
			break;
			case EVENT_MODE :
				if((curr_mode & 0x03) == MODE_DISPLAY_TIME)
				{
					Serial.println("updata NTP time");
					getNtpTimeSuccess = 0;

				    TFTscreen.stroke(0,0,0);
				    clear_dial();
				    clear_time_pointer();
				    clear_time_txt(3);

					ntpTime();

					if (!getNtpTimeSuccess)
					{
					    TFTscreen.stroke(255,0,0);
					    TFTscreen.textSize(1);
						TFTscreen.text("get NTP Time",28,70);
						TFTscreen.text("failed",46,80);
						delay(3000);
					    TFTscreen.stroke(0,0,0);
						TFTscreen.text("get NTP Time",28,70);
						TFTscreen.text("failed",46,80);
					    TFTscreen.stroke(255,255,255);
					    TFTscreen.textSize(2);
					}

					TFTscreen.fillRect(0,70,128,10,0);
				}

				if ((curr_mode & 0x03) == MODE_BROWSE_CAL)
					modif_bit = ~ modif_bit;

//				if (curr_mode == MODE_DISPLAY_TIME)
//					curr_mode = MODE_DISPLAY_CAL;
//				else
//					curr_mode = MODE_DISPLAY_TIME;
//				break;
				break;
			case EVENT_UP :
				if (curr_mode == MODE_BROWSE_CAL)
				{
					if (!modif_bit)
						upgrade_cal(UP_MON,&browseTime);
					else
						upgrade_cal(UP_YEAR,&browseTime);
				}
				break;
			case EVENT_DOWN :
				if (curr_mode == MODE_BROWSE_CAL)
				{
					if (!modif_bit)
						upgrade_cal(DOWN_MON,&browseTime);
					else
						upgrade_cal(DOWN_YEAR,&browseTime);
				}
				break;
			case EVENT_MODE_CHANG :
				if (curr_mode == MODE_BROWSE_CAL)
					curr_mode = MODE_DISPLAY_TIME;
				else
					curr_mode++;
				browseTime = time;
				unsigned char i,j;
				for (i = 0; i < 6;i++)
					for (j = 0;j < 7; j++)
						browse_cal_char[i][j] = cal_char[i][j];
				break;
				break;

			default:

				break;
		}

		event_key = 0;
	}
}

void display_proc(void)
{
	static unsigned char modif_flag = 0;
	switch(curr_mode)
	{
		case MODE_DISPLAY_TIME:
			TFTscreen.stroke(0, 0, 0);
			clear_all();
			RTC.getTime(&time);
			TFTscreen.stroke(255,255,255);
			if (!getNtpTimeSuccess)
			{
				TFTscreen.stroke(0,255,0);
				TFTscreen.textSize(1);
				TFTscreen.text("-_-||",90,100);
				TFTscreen.stroke(255,255,255);
				TFTscreen.textSize(2);
			}
			draw_dial();
			draw_time_pointer(&time);
			draw_time_txt(&time);
			break;

		case MODE_DISPLAY_CAL:
			TFTscreen.stroke(0, 0, 0);
			clear_all();
			RTC.getTime(&time);
			TFTscreen.stroke(0,0,255);
			TFTscreen.textSize(1);
			TFTscreen.text("Calendar",40,0);
			TFTscreen.textSize(2);
			TFTscreen.stroke(0,255,0);
			draw_year(&time);
			TFTscreen.stroke(0,0,255);
			draw_mon(&time);
			TFTscreen.textSize(1);
			TFTscreen.stroke(0,255,0);
			TFTscreen.text("S  M  T  W  T  F  S ",4,40);
			draw_cal(&time);
			TFTscreen.stroke(255,255,255);
			TFTscreen.textSize(2);

			break;
		case MODE_BROWSE_CAL :
			TFTscreen.stroke(0, 0, 0);
			clear_all();
			TFTscreen.stroke(0,0,255);
			TFTscreen.textSize(1);
			TFTscreen.text("Calendar",40,0);
			TFTscreen.textSize(2);
//			if (modif_flag)
//				clear_modif_cal(modif_bit);
//				clear_mon();
//			else
				draw_modif_cal(modif_bit);
//				clear_mon();

//			TFTscreen.stroke(0,255,0);
//			TFTscreen.text(String(time.year).c_str(),8,15);
//			TFTscreen.stroke(0,0,255);
//			TFTscreen.text(mon_list[time.mon].c_str(),80,15);
			TFTscreen.textSize(1);
			TFTscreen.stroke(0,255,0);
			TFTscreen.text("S  M  T  W  T  F  S ",4,40);
			draw_browse_cal(&browseTime);
			TFTscreen.stroke(255,255,255);
			TFTscreen.textSize(2);
			modif_flag = ~modif_flag;
			break;
		default :
			break;
	}
}
