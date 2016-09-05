 /**
 * @file wirish_digital.c
 * @brief Definitions wirish_digital class for Fireduino
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
#include "io.h"
#include "ard_gpio.h"
#include "wiring.h"

void pinMode(unsigned char pin, WiringPinMode mode)
{
   ard_pin_mode(pin,mode);
}

unsigned long digitalRead(unsigned char pin) {
	return ard_gpio_read(pin);
}

void digitalWrite(unsigned char pin, unsigned char val) {

    ard_gpio_write(pin,val);
}

void togglePin(unsigned char pin)
{
    if  (digitalRead(pin))
		digitalWrite(pin,0);
	else
		digitalWrite(pin,1);
}


unsigned char isButtonPressed(unsigned char pin, unsigned long pressedLevel)
{
    if (digitalRead(pin) == pressedLevel) {
        delay(BUTTON_DEBOUNCE_DELAY);
        while (digitalRead(pin) == pressedLevel)
            ;
        return 1;
    }
    return 0;
}

unsigned char waitForButtonPress(unsigned long timeout)
{
    uint32_t start = millis();
    uint32_t time;
    if (timeout == 0) {
        while (!isButtonPressed(BOARD_BUTTON_PIN,LOW))
            ;
        return 1;
    }
    do {
        time = millis();
        /* properly handle wrap-around */
        if ((start > time && time + (0xffffffffU - start) > timeout) ||
            time - start > timeout) {
            return 0;
        }
    } while (!isButtonPressed(BOARD_BUTTON_PIN,LOW));

    return 1;
}

