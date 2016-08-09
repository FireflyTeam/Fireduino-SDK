#include <Arduino.h>
#include "TF.h"
#include "Audio.h"

void setup() {
	Serial.begin(115200);
	Serial.print("\r\narduino setup ...\r\n");

	if(!SD.begin())
	{
		Serial.println("sd init err\r\n");
		while(1);
	}

	Audio.begin(AUDIO_SD);
	if(Audio)
	{
		Audio.playFile("\\4.MP3");
	}

}

void loop() {
	Serial.println("loop");
	delay(2000);
	if(Audio.getStatus() == AudioStop)
	{
		Audio.playFile("\\3.MP3");
	}
}

