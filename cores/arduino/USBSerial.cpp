
#include "Arduino.h"
#include "USBSerial.h"
#include "Print.h"
#include "ard_usbserial.h"


void Serial_::begin(uint32_t baud_count)
{
	(void)baud_count;
	ard_usbserial_init();
}

void Serial_::begin(uint32_t baud_count, uint8_t config)
{
	(void)baud_count;
	(void)config;
	ard_usbserial_init();
}

void Serial_::end(void)
{

}

int Serial_::available(void)
{
	return ard_usbserial_data_available();
}

int Serial_::peek(void)
{
	return ard_usbserial_peek();
}

int Serial_::read(void)
{
	while(! this->available());

	return ard_usbserial_getc();

}

void Serial_::flush(void)
{
	ard_usbserial_flush();
}

size_t Serial_::write(const uint8_t *buffer, size_t size)
{
	return ard_usbserial_putc((uint8_t *)buffer,size);
}

size_t Serial_::write(uint8_t c) {
	return ard_usbserial_putc(&c,1);
}

Serial_::operator bool()
{

	bool result = false;

	if (ard_get_line_info()->lineState > 0)
	{
		result = true;
	}
	else
	{
		delay(10);
	}
	return result;
}

unsigned long Serial_::baud() {
	return ard_get_line_info()->dwDTERate;
}

uint8_t Serial_::stopbits() {
	return ard_get_line_info()->bCharFormat;
}

uint8_t Serial_::paritytype() {
	return ard_get_line_info()->bParityType;
}

uint8_t Serial_::numbits() {
	return ard_get_line_info()->bDataBits;
}

bool Serial_::dtr() {
	return ard_get_line_info()->lineState & 0x1;
}

bool Serial_::rts() {
	return ard_get_line_info()->lineState & 0x2;
}

Serial_ Serial;

