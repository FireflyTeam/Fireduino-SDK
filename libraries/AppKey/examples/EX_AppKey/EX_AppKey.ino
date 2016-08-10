/**
* @file Appkey.ino
* @brief Definitions Appkey for Fireduino
* @author zjy<zjy@t-chip.com.cn>
* @version V1.0
* @date 2016.06
*
* @par Copyright:
* Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
*
* For more information, please visit website <http://www.t-firefly.com/>, \n\n
* or email to <service@t-firefly.com>.
*/

#include <Arduino.h>
#include "AppKey.h"
#include <WiFi.h>
 
 char ssid[] = "yourSSID"; 	//	your network SSID (name)
 char pass[] = "yourpassword";	// your network password
 int status = WL_IDLE_STATUS;
 
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

/* app_key_proc
par1:key event -	EVENT_NULL,
					EVENT_DOWN,
					EVENT_UP,
					EVENT_MOVE,
					EVNET_CLICK,
					EVENT_REPEAT,
					EVENT_TXT,
					EVENT_SHUTDOWN,
					EVENT_UIRECENT,

par2:arg0			it means key num when key event is EVENT_DOWN,EVENT_UP,EVENT_MOVE,EVNET_CLICK or EVENT_REPEAT.
					it means x value when key event is EVENT_MOVE.
					it means text spring when key event is EVENT_TXT.
					it means none and can't be used when key event is EVENT_NULL, EVENT_SHUTDOWN or EVENT_UIRECENT.

par3:arg1			it means y value when key event is EVENT_MOVE.
					it means text spring len when key event is EVENT_TXT.
					it means none and can't be used when key event is EVENT_NULL,EVENT_DOWN,EVENT_UP,EVENT_MOVE,EVNET_CLICK,EVENT_REPEAT,EVENT_SHUTDOWN or EVENT_UIRECENT.
*/
void app_key_proc(unsigned int key_event, void* arg0, void* arg1)
 {
	switch((key_type_t)key_event)
	{
		case EVENT_DOWN:
			Serial.print("Press ");
			{
				key_num_t *key_value = (key_num_t *)arg0;
				switch(*key_value)
				{
					case VALUE_RETURN:Serial.println("RETURN");break;
					case VALUE_O:Serial.println("O");break;
					case VALUE_UP:Serial.println("UP");break;
					case VALUE_DOWN:Serial.println("DOWN");break;
					case VALUE_LEFT:Serial.println("LEFT");break;
					case VALUE_RIGHT:Serial.println("RIGHT");break;
					case VALUE_OK:Serial.println("OK");break;
					case VALUE_SELECT:Serial.println("SELECT");break;
					case VALUE_BUTTON_LEFT:Serial.println("BUTTON_LEFT");break;
					case VALUE_BUTTON_RIGHT:Serial.println("BUTTON_RIGHT");break;
					case VALUE_VOL_MUTE:Serial.println("MUTE");break;
					case VALUE_VOL_DOWN:Serial.println("VOL DOWN");break;
					case VALUE_VOL_UP:Serial.println("VOL UP");break;
					case VALUE_MENU:Serial.println("MENU");break;
					case VALUE_HOME:Serial.println("HOME");break;
					case VALUE_BUTTON_A:Serial.println("A");break;
					case VALUE_BUTTON_B:Serial.println("B");break;
					case VALUE_BUTTON_C:Serial.println("C");break;
					case VALUE_BUTTON_D:Serial.println("D");break;
					case VALUE_BUTTON_SELECT:Serial.println("BUTTON_SELECT");break;
					case VALUE_BUTTON_START:Serial.println("BUTTON_START");break;
					default:break;
				}
			}
			break;

		case EVENT_UP:
			Serial.print("Relaese ");
			{
				key_num_t *key_value = (key_num_t *)arg0;
				switch(*key_value)
				{
					case VALUE_RETURN:Serial.println("RETURN");break;
					case VALUE_O:Serial.println("O");break;
					case VALUE_UP:Serial.println("UP");break;
					case VALUE_DOWN:Serial.println("DOWN");break;
					case VALUE_LEFT:Serial.println("LEFT");break;
					case VALUE_RIGHT:Serial.println("RIGHT");break;
					case VALUE_OK:Serial.println("OK");break;
					case VALUE_SELECT:Serial.println("SELECT");break;
					case VALUE_BUTTON_LEFT:Serial.println("BUTTON_LEFT");break;
					case VALUE_BUTTON_RIGHT:Serial.println("BUTTON_RIGHT");break;
					case VALUE_VOL_MUTE:Serial.println("MUTE");break;
					case VALUE_VOL_DOWN:Serial.println("VOL DOWN");break;
					case VALUE_VOL_UP:Serial.println("VOL UP");break;
					case VALUE_MENU:Serial.println("MENU");break;
					case VALUE_HOME:Serial.println("HOME");break;
					case VALUE_BUTTON_A:Serial.println("A");break;
					case VALUE_BUTTON_B:Serial.println("B");break;
					case VALUE_BUTTON_C:Serial.println("C");break;
					case VALUE_BUTTON_D:Serial.println("D");break;
					case VALUE_BUTTON_SELECT:Serial.println("BUTTON_SELECT");break;
					case VALUE_BUTTON_START:Serial.println("BUTTON_START");break;
					default:break;
				}
			}
			break;
			
		case EVENT_MOVE:
			{
				int* x_value =(int*)arg0;
				int* y_value =(int*)arg1;
				Serial.print("x:");
				Serial.print(*x_value);
				Serial.print(" y:");
				Serial.println(*y_value);
			}
			
			break;

		case EVNET_CLICK:
			Serial.print("key kick ");
			{
				key_num_t *key_value = (key_num_t *)arg0;
				switch(*key_value)
				{
					case VALUE_RETURN:Serial.println("RETURN");break;
					case VALUE_NUM1:Serial.println("NUM1");break;
					case VALUE_NUM2:Serial.println("NUM2");break;
					case VALUE_NUM3:Serial.println("NUM3");break;
					case VALUE_NUM4:Serial.println("NUM4");break;
					case VALUE_NUM5:Serial.println("NUM5");break;
					case VALUE_NUM6:Serial.println("NUM6");break;
					case VALUE_NUM7:Serial.println("NUM7");break;
					case VALUE_NUM8:Serial.println("NUM8");break;
					case VALUE_NUM9:Serial.println("NUM9");break;
					case VALUE_NUM0:Serial.println("NUM0");break;
					case VALUE_UP:Serial.println("UP");break;
					case VALUE_DOWN:Serial.println("DOWN");break;
					case VALUE_LEFT:Serial.println("LEFT");break;
					case VALUE_RIGHT:Serial.println("RIGHT");break;
					case VALUE_SELECT:Serial.println("SELECT");break;
					case VALUE_VOL_MUTE:Serial.println("MUTE");break;
					case VALUE_VOL_DOWN:Serial.println("VOL DOWN");break;
					case VALUE_VOL_UP:Serial.println("VOL UP");break;
					default:break;
				}
			}
			break;

		case EVENT_REPEAT:
			Serial.print("Keep Press ");
			{
				key_num_t *key_value = (key_num_t *)arg0;
				switch(*key_value)
				{
					case VALUE_RETURN:Serial.println("RETURN");break;
					case VALUE_O:Serial.println("O");break;
					case VALUE_UP:Serial.println("UP");break;
					case VALUE_DOWN:Serial.println("DOWN");break;
					case VALUE_LEFT:Serial.println("LEFT");break;
					case VALUE_RIGHT:Serial.println("RIGHT");break;
					case VALUE_VOL_DOWN:Serial.println("VOL DOWN");break;
					case VALUE_VOL_UP:Serial.println("VOL UP");break;
					case VALUE_BUTTON_A:Serial.println("A");break;
					case VALUE_BUTTON_B:Serial.println("B");break;
					case VALUE_BUTTON_C:Serial.println("C");break;
					case VALUE_BUTTON_D:Serial.println("D");break;
					default:break;
				}
			}
			break;
			
		case EVENT_TXT:
			{
				 char* recv_string = (char*)arg0;
				 int* recv_len = (int*)arg1;
				 Serial.print("Recvive ");
				 Serial.print(*recv_len);
				 Serial.println(" characters:");
				 Serial.println(recv_string);
			}
			break;

		case EVENT_SHUTDOWN:
			Serial.println("shutdown ");
			break;
			
		case EVENT_UIRECENT:
			Serial.println("ui recent");
			break;

		default:break;
	}
 }
 
 
 void setup() {
	 Serial.begin(115200);
	 Serial.print("\r\narduino setup ...\r\n");
 
	 // check for the presence of the shield:
   if (WiFi.status() == WL_NO_SHIELD) {
	 Serial.println("WiFi shield not present");
	 // don't continue:
	 while (true);
   }
 
   String fv = WiFi.firmwareVersion();
   if (fv != "1.1.0") {
	 Serial.println("Please upgrade the firmware");
   }
 
   // attempt to connect to Wifi network:
   while (status != WL_CONNECTED) {
	 Serial.print("Attempting to connect to SSID: ");
	 Serial.println(ssid);
	 // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
	 status = WiFi.begin(ssid, pass);
 
	 // wait 10 seconds for connection:
	 // delay(10000);
   }
   Serial.println("Connected to wifi");
   printWifiStatus();
   AppKey.begin(app_key_proc);
 }
 
 void loop() {
	// Serial.println("loop");
	 delay(2000);
 }

