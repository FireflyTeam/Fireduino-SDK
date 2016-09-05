#include "Arduino.h"

void setup(void)
{
	Serial1.begin(115200);
	Serial.begin(115200);
	while(!Serial);
	Serial1.print("\r\n FireDuino Setup ...\r\n");
	Serial.print("\r\n FireDuino Setup ...\r\n");

}
int inByte = 0;         // incoming serial byte
void loop(void)
{
	if (Serial1.available() > 0) {
	  inByte = Serial1.read();
	  Serial1.println(inByte);
	}

	if (Serial.available() > 0) {
	  inByte = Serial.read();
	  Serial.println(inByte);
	}
}
