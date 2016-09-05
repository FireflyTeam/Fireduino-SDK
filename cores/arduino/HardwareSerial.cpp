 /**
 * @file HardwareSerial.c
 * @brief Definitions HardwareSerial class for Fireduino
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
#include "HardwareSerial.h"
//#include "DriverInclude.h"
#include "ard_uart.h"
HardwareSerial::HardwareSerial(unsigned char  dev) {
	dev_id = dev;
}

void HardwareSerial::begin(uint32_t baud)
{
	begin(baud,SERIAL_8N1);
}

void HardwareSerial::begin(uint32_t baud, uint8_t config) 
{
	uint8_t databit = (config >> 0) & 0x0F;
	uint8_t stopbit = (config >> 4) & 0x01;
	uint8_t parity  = (config >> 5) & 0x03;
	ard_uart_init((uint8_t)dev_id,baud ,databit,stopbit,parity);
}

void HardwareSerial::end(void) 
{
	ard_uart_disable(dev_id);
}

int HardwareSerial::read(void) 
{
	while(! this->available());

	return ard_uart_getc(dev_id);
}

int HardwareSerial::available(void) 
{
	return ard_uart_data_available(dev_id);
}


int HardwareSerial::peek(void)
{
	return ard_uart_peek(dev_id);
}

int HardwareSerial::availableForWrite(void)
{
  return 1;
}

size_t HardwareSerial::write(unsigned char ch) {
	return ard_uart_putc(dev_id,ch);
}

void HardwareSerial::flush(void) {

	ard_uart_flush(dev_id);
}

HardwareSerial Serial2(1);
HardwareSerial Serial1(2);

