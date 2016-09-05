 /**
 * @file wirish_pulse.c
 * @brief Definitions wirish_pulse class for Fireduino
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
#include <wirish_pulse.h>
#include "wirish_digital.h"
#include "wiring.h"

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse. */
uint32_t pulseIn( uint32_t pin, uint32_t state, uint32_t timeout )
{
	// cache the port and bit of the pin in order to speed up the
	// pulse width measuring loop and achieve finer resolution.  calling
	// digitalRead() instead yields much coarser resolution.
//	uint32_t width = 0; // keep initialization out of time critical area
	uint32_t start = 0;
	uint32_t startMicros = micros();


	pinMode(pin,INPUT);

	// wait for any previous pulse to end
	while (digitalRead(pin) == state)
	{
		if (micros() - startMicros > timeout)
			return 0;
	}


	// wait for the pulse to start
	while (digitalRead(pin) != state)
	{
		if (micros() - startMicros > timeout)
			return 0;
	}

	start = micros();

	// wait for the pulse to stop
	while (digitalRead(pin) == state)
	{
		if (micros() - startMicros > timeout)
			return 0;
	}
	return (micros() - start);
}


