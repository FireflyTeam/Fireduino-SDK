 /**
 * @file Wire.cpp
 * @brief Definitions Wire class for Fireduino
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
#include "Wire.h"
#include "ard_i2c.h"
#include "stdio.h"

TwoWire::TwoWire(unsigned char  dev) {
	dev_id = dev;
}

void TwoWire::begin(void) {
	printf("ard_i2c_init\r\n");
	ard_i2c_init(dev_id);
}

void TwoWire::end(void) {
	ard_i2c_deinit(dev_id);
}

void TwoWire::beginTransmission(uint8_t address) {
	status = MASTER_SEND;
	txAddress = address << 1;
	txBufferLength = 0;
}

void TwoWire::beginTransmission(int address) {
	beginTransmission((uint8_t) address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop) {
	uint8_t error = 0;
//	ard_i2c_start(dev_id,txAddress);

	ard_i2c_write(dev_id,txAddress,txBuffer,txBufferLength);
	txBufferLength = 0;		// empty buffer
	status = MASTER_IDLE;
	return error;
}


uint8_t TwoWire::endTransmission(void)
{
	return endTransmission(true);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop) {
	uint8_t read;

	if(quantity > BUFFER_LENGTH){
		quantity = BUFFER_LENGTH;
	}

	read = ard_i2c_read(dev_id,address << 1, rxBuffer, quantity);
	rxBufferIndex = 0;
	rxBufferLength = read;

	return read;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint32_t) 0, (uint8_t) 0, (uint8_t) sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) sendStop);
}



size_t TwoWire::write(uint8_t data) {
	if (status == MASTER_SEND) {
		if (txBufferLength >= BUFFER_LENGTH)
			return 0;
		txBuffer[txBufferLength++] = data;
		return 1;
	} else {
			return 0;
	}
}

size_t TwoWire::write(const uint8_t *data, size_t quantity) {
	if (status == MASTER_SEND) {
		for (size_t i = 0; i < quantity; ++i) {
			if (txBufferLength >= BUFFER_LENGTH)
				return i;
			txBuffer[txBufferLength++] = data[i];
		}
	} else {
		return 0;
	}
	return quantity;
}

int TwoWire::available(void) {
	return rxBufferLength - rxBufferIndex;
}

int TwoWire::read(void) {
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex++];
	return -1;
}

int TwoWire::peek(void) {
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex];
	return -1;
}

void TwoWire::flush(void) {

}

TwoWire Wire(0);
TwoWire Wire1(1);
