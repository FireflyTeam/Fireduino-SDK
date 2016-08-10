 /**
 * @file SPI.cpp
 * @brief Definitions SPI class for Fireduino
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
#include "SPI.h"
#include "ard_spi.h"
SPIClass::SPIClass()
{
	// Empty
}

void SPIClass::begin() {
	cs_pin = 0;
	init();
	// NPCS control is left to the user

	// Default speed set to 4Mhz
	setClockDivider(cs_pin, 6);
	setDataMode(cs_pin, SPI_MODE0);
	setBitOrder(cs_pin, MSBFIRST);
}

void SPIClass::begin(unsigned char _pin) {
	cs_pin = _pin;
	init();

	setClockDivider(_pin, 6);
	setDataMode(_pin, SPI_MODE0);
	setBitOrder(_pin, MSBFIRST);
}

void SPIClass::init() {
	ard_spi_io_init();
	ard_spi_init();
	ard_spi_pin();
}



void SPIClass::beginTransaction(unsigned char _pin, SPISettings settings)
{
	unsigned int div;
	if(_pin == cs_pin)
	{
		ard_spi_io_init();
		div = 24000000/(settings.config & 0x3fffffff);
		setClockDivider(_pin, div);
		setDataMode(_pin, (settings.config >> 30) & 0x03);
		setBitOrder(_pin, settings.border);
	}
}



void SPIClass::endTransaction(void)
{
	ard_spi_io_deinit();
}

void SPIClass::endTransaction(unsigned char _pin)
{
	if(_pin == cs_pin)
	{
		endTransaction();
	}
}

void SPIClass::end(unsigned char _pin) {
	if(_pin == cs_pin)
	{
		end();
	}

}

void SPIClass::end() {
	ard_spi_deinit();
}

void SPIClass::setBitOrder(unsigned char _pin, BitOrder _bitOrder) {
	if(_pin == cs_pin)
	{
		ard_spi_setBitOrder(_bitOrder);
	}
}

void SPIClass::setDataMode(unsigned char _pin, unsigned char _mode) {
	if(_pin == cs_pin)
	{
		ard_spi_setDataMode(_mode);
	}
}

void SPIClass::setClockDivider(unsigned char _pin, unsigned char _divider) {
	if(_pin == cs_pin)
	{
		ard_spi_setClockDivider(_divider);
	}
}

unsigned char SPIClass::transfer(unsigned char _pin, unsigned char _data, SPITransferMode _mode) {

	unsigned char temp;

	if(0 != _pin )
	{
		if(_pin == cs_pin)
		{
			//enable select pin
			if(_pin == 10)
				ard_spi_cs(0);
		}
	}


	temp = ard_spi_transfer(_data);

	if(0 != _pin && SPI_LAST == _mode)
	{
		//disable select pin
		if(_pin == 10)
			ard_spi_cs(1);
	}

	return temp;
}

unsigned short SPIClass::transfer16(unsigned char _pin, unsigned short _data, SPITransferMode _mode) {

	unsigned short temp;

	if(0 != _pin )
	{
		if(_pin == cs_pin)
		{
			//enable select pin
			if(_pin == 10)
				ard_spi_cs(0);
		}
	}


	temp = ard_spi_transfer16(_data);

	if(0 != _pin && SPI_LAST == _mode)
	{
		//disable select pin
		if(_pin == 10)
			ard_spi_cs(1);
	}

	return temp;
}

void SPIClass::transfer(unsigned char _pin, void *_buf, unsigned int _count, SPITransferMode _mode) {
	unsigned int i;
	for(i = 0; i < _count - 1;i++ )
	{
		transfer(_pin,((unsigned char *)_buf)[i],SPI_CONTINUE);
	}
	transfer(_pin,((unsigned char *)_buf)[i],_mode);
}

void SPIClass::transfer16(unsigned char _pin, void *_buf, unsigned int _count, SPITransferMode _mode) {
	unsigned int i;
	for(i = 0; i < _count - 1;i++ )
	{
		transfer16(_pin,((unsigned short *)_buf)[i],SPI_CONTINUE);
	}
	transfer16(_pin,((unsigned short *)_buf)[i],_mode);
}

void SPIClass::attachInterrupt(void) {
	// Should be enableInterrupt()
}

void SPIClass::detachInterrupt(void) {
	// Should be disableInterrupt()
}



SPIClass SPI;


