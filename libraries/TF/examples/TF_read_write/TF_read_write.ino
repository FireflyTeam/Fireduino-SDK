 /**
 * @file TF_read_write.ino
 * @brief Definitions TF_read_write Demo for Fireduino
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
#include "Arduino.h"
#include "TF.h"

File root;

void setup(void)
{
	Serial.begin(115200);
	Serial.print("\r\narduino setup ...\r\n");

	if(!SD.begin())
	{
		Serial.println("\r\nSD.begin err ...\r\n");
		while(1);
	}

	root = SD.open("test.txt",FILE_WRITE);

	if(root)
	{
		Serial.println("SD.open(\"test.txt\") ok!");
		root.write((unsigned char *)"01234567890",10);
		root.write(65);
		root.write(66);
		root.write(65);
		root.write(66);
		root.write((unsigned char *)"01234567890",10);
		root.seek(0);

		Serial.print("root size:");
		Serial.println(root.size());
		while(root.available())
		{
			Serial.println(root.read(),BYTE);
		}
		root.close();

	}
	else
	{
		Serial.println("SD.open(\"test.txt\") err!\n");
	}

}

void loop(void)
{
	Serial.println("loop ... \r\n");
	delay(1000);
}




