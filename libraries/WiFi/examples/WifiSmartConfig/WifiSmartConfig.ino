/**
 * @file NetKey.ino
 * @brief Definitions NetKey_demo for Fireduino
 * @author zjy<zjy@t-chip.com.cn> 
 * @version V1.0
 * @date 2016.05
 * 
 * @par Copyright:
 * Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
 *
 * For more information, please visit website <http://www.t-firefly.com/>, \n\n
 * or email to <service@t-firefly.com>.
 */ 
#include <WiFi.h>
#include "PowerKey.h"


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

#define	ADEBUG(x)	Serial.print(x)
#define	ADEBUGLN(x)	Serial.println(x)
#define	ADEBUGDEX(x)	Serial.print(x,HEX)
unsigned char power_btn_event = 0;
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
		power_btn_event =1;
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

void setup() {
	Serial.begin(115200);

	// attempt to connect to Wifi network:
	int status = WL_IDLE_STATUS;
	Serial.print("Attempting to connect to ");
	if((status = WiFi.begin())!= WL_CONNECTED)
	{
		WiFi.smart_config();
	}

	// you're connected now, so print out the status:
	printWifiStatus();

	PowerKey.begin(callback);
}

void loop() {

	delay(100);
	if(power_btn_event)
	{
		Serial.println("Enter WiFi Smart Config...");
		WiFi.smart_config();
		Serial.println("Exit WiFi Smart Config...");
		printWifiStatus();
		
		power_btn_event = 0;

	}
}

