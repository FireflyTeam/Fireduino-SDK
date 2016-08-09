 /**
 * @file wirish_tone.cpp
 * @brief Definitions wirish_tone class for Fireduino
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
#include "ard_time.h"
#include "wirish_digital.h"
#include "wirish.h"

static unsigned long gTonePin;
volatile unsigned long gTonePinMask = 0x00000000;
static unsigned long gTimeCount,gTimeOut;
static unsigned int gFreq = 0;
//volatile unsigned int durationEable;


void timerCallback1(void)
{
	// Toggle the pin level of gTonePin
	togglePin(gTonePin);

	// When duration is able,calculate difference value to current time and tone start time
	if ((gTimeOut != 0) && ((millis() - gTimeCount) > gTimeOut ))
	{
		// Time to stop tone
		noTone(gTonePin);

		// Keep Low after stop
		digitalWrite(gTonePin,LOW);
	}
//	static unsigned int count;
//	if (count == 200)
//	{
//		count = 0;
//		printf("timerCallback1!\r\n");
//	}
//	count++;
}


void tone(unsigned int pin,unsigned int freq,unsigned int duration = 0)
{
	// only one tone can be generated at a time
	// if one tone is exist,when set the same pin ,it can change the freq,
	// or not the same pin,it doesn't work untill the noTone is call.

	// No pin has generated tone
	if (gTonePinMask == 0)  //this pin is playing tone
	{
		// mask has one tone is already generated
		gTonePinMask |= (0x1 << pin);

		//set pinMode
		pinMode(pin,OUTPUT);

		// Set the tone pin
		gTonePin = pin;

		// Get current time(ms) and restore in gTimeCount
		gTimeCount = millis();

		// Set the duration time
		gTimeOut = duration;

		// Save Current frequency
		gFreq = freq;

		// Init Timer 1 and start Timer 1
		ard_timer_init(1,timerCallback1);
		ard_timer_stop(1);
		ard_timer_set_count(1,(1000000/freq/2));
		ard_timer_start(1);
	}

	else
		// one pin has generated the tone and frequency is changed
		if (gTonePinMask == (0x1 << pin) && freq != gFreq)
			{
				// Get current time(ms) and restore in gTimeCount
				gTimeCount = millis();

				// Set the duration time
				gTimeOut = duration;

				// Save Current frequency
				gFreq = freq;

				// Updata the duration time
				ard_timer_stop(1);
				ard_timer_set_count(1,(1000000/freq/2));
				ard_timer_start(1);
			}
	return;
}

void noTone(unsigned int pin)
{
	// clear the gTonePinMask when a tone is playing
	gTonePinMask = 0;

	// stop Timer 1
	ard_timer_stop(1);

	// Keep Low after stop
	digitalWrite(gTonePin,LOW);

}
