 /**
 * @file w5x00_version.ino
 * @brief Definitions w5x00_version Demo for Fireduino
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
#include "SPI.h"

#define SPI_CS
const int chipSelectPin = 10;
const int chipResetPin = 9;
void setup() {
	Serial.begin(115200);

#ifdef SPI_CS
	Serial.print("\r\nSPI_CS  YES...\r\n");
	SPI.begin(10);
#else
	Serial.print("\r\nSPI_CS  NO...\r\n");
	SPI.begin();
	pinMode(chipSelectPin, OUTPUT);
	digitalWrite(chipSelectPin, LOW);
#endif

	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(2);
	SPI.setDataMode(SPI_MODE0);

	pinMode(chipResetPin, OUTPUT);
	digitalWrite(chipResetPin, LOW);
	delay(10);
	digitalWrite(chipResetPin, HIGH);
	delay(10);
}

void loop() {

	unsigned char version;
#ifdef SPI_CS
	SPI.transfer(0x00,SPI_CONTINUE);
	SPI.transfer(0x39,SPI_CONTINUE);
	SPI.transfer(0x00,SPI_CONTINUE);
	version = SPI.transfer(0x00);
#else
	digitalWrite(chipSelectPin, LOW);
	SPI.transfer(0x00);
	SPI.transfer(0x39);
	SPI.transfer(0x00);
	version = SPI.transfer(0x00);
	digitalWrite(chipSelectPin, HIGH);
#endif


	Serial.print("version:");
	Serial.println(version);
	delay(1000);
}



