#include <Arduino.h>
#include "TF.h"
#include "Audio.h"
#include <WiFi.h>

char ssid[] = "nanod";     //  your network SSID (name)
char pass[] = "12345678";  // your network password
int status = WL_IDLE_STATUS;


void printWifiStatus();
void setup() {
	Serial.begin(115200);
	Serial.print("\r\narduino setup ...\r\n");

	if(!SD.begin())
	{
		Serial.println("sd init err\r\n");
		while(1);
	}

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


	Audio.begin(AUDIO_NET);
	Audio.playNetFile("http://www.t-firefly.com/FireDuino.mp3");
	//Audio.playNetFile("http://192.168.199.240/%E6%89%93%E8%80%81%E8%99%8E.MP3");

}

void loop() {
	Serial.println("loop");
	delay(2000);
	if(Audio.getStatus() == AudioStop)
	{
		Audio.playNetFile("http://www.t-firefly.com/FireDuino.mp3");
		//Audio.playNetFile("http://192.168.199.240/%E6%89%93%E8%80%81%E8%99%8E.MP3");
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


