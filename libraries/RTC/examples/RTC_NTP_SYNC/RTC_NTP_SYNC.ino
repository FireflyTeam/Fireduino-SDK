 /**
 * @file RTC_NTP_SYNC.ino
 * @brief Definitions RTC_NTP_SYNC Demo for Fireduino
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
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Wire.h"
#include "RTC.h"

#define SECONDS_IN_DAY          86400
#define START_YEAR              1900
#define TIME_ZONE               +8
static int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


int status = WL_IDLE_STATUS;
char ssid[] = "nanod";  //  your network SSID (name)
char pass[] = "12345678";       // your network password
RTCTime time;

unsigned int localPort = 2390;      // local port to listen for UDP packets

IPAddress timeServer(131, 107, 13, 100); // time-nw.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

unsigned long sendNTPpacket(IPAddress& address);
void printWifiStatus();

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to network.
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  Udp.begin(localPort);

  Wire1.begin();

  pinMode(2,INPUT_PULLUP);

  sendNTPpacket(timeServer); // send an NTP packet to a time server

}

RTCTime * timeStamp2time(unsigned int timeStamp ,RTCTime *time);

void loop(void)
{
	delay(1000);

	if(digitalRead(2) == HIGH)
	{
		sendNTPpacket(timeServer); // send an NTP packet to a time server
	}

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
		RTC.setTime(&time);
	}

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


