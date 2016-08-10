 /**
 * @file SPI.c
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
#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include "wirish_constants.h"
#include <stdio.h>
#include <stdint.h>
#define SPI_MODE0 0x00
#define SPI_MODE1 0x02
#define SPI_MODE2 0x01
#define SPI_MODE3 0x03


typedef enum SPITransferMode {
	SPI_CONTINUE,
	SPI_LAST
}SPITransferMode;



class SPISettings {
public:
	SPISettings(unsigned int clock, BitOrder bitOrder, unsigned char dataMode) {
		if (__builtin_constant_p(clock)) {
			init_AlwaysInline(clock, bitOrder, dataMode);
		} else {
			init_MightInline(clock, bitOrder, dataMode);
		}
	}
	SPISettings() { init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0); }
private:
	void init_MightInline(unsigned int clock, BitOrder bitOrder, unsigned char dataMode) {
		init_AlwaysInline(clock, bitOrder, dataMode);
	}
	void init_AlwaysInline(unsigned int clock, BitOrder bitOrder, unsigned char dataMode) __attribute__((__always_inline__)) {
		border = bitOrder;
		config = clock | dataMode << 30;
	}
	unsigned int config;
	BitOrder border;
	friend class SPIClass;
};



class SPIClass {
  public:
	SPIClass();

	// Transfer functions
	unsigned char transfer(unsigned char _pin, unsigned char _data, SPITransferMode _mode = SPI_LAST);
	void transfer(unsigned char _pin, void *_buf, unsigned int _count, SPITransferMode SPI_LAST);
	// Transfer functions on default pin BOARD_SPI_DEFAULT_SS
	unsigned char transfer(unsigned char _data, SPITransferMode _mode = SPI_LAST) { return transfer(cs_pin, _data, _mode); }
	void transfer(void *_buf, unsigned int _count, SPITransferMode _mode = SPI_LAST) { transfer(cs_pin, _buf, _count, _mode); }

	// Transfer functions
	unsigned short transfer16(unsigned char _pin, unsigned short _data, SPITransferMode _mode = SPI_LAST);
	void transfer16(unsigned char _pin, void *_buf, unsigned int _count, SPITransferMode SPI_LAST);
	// Transfer functions on default pin BOARD_SPI_DEFAULT_SS
	unsigned short transfer16(unsigned short _data, SPITransferMode _mode = SPI_LAST) { return transfer16(cs_pin, _data, _mode); }
	void transfer16(void *_buf, unsigned int _count, SPITransferMode _mode = SPI_LAST) { transfer16(cs_pin, _buf, _count, _mode); }

	// Transaction Functions
	void usingInterrupt(unsigned char interruptNumber);
	void beginTransaction(SPISettings settings) { beginTransaction(cs_pin, settings); }
	void beginTransaction(unsigned char _pin, SPISettings settings);
	void endTransaction(void);
	void endTransaction(unsigned char _pin);

	// SPI Configuration methods
	void attachInterrupt(void);
	void detachInterrupt(void);

	void begin(void);
	void end(void);

	// Attach/Detach pin to/from SPI controller
	void begin(unsigned char _pin);
	void end(unsigned char _pin);

	// These methods sets a parameter on a single pin
	void setBitOrder(unsigned char _pin, BitOrder);
	void setDataMode(unsigned char _pin, unsigned char);
	void setClockDivider(unsigned char _pin, unsigned char);

	// These methods sets the same parameters but on default pin BOARD_SPI_DEFAULT_SS
	void setBitOrder(BitOrder _order) { setBitOrder(cs_pin, _order); };
	void setDataMode(unsigned char _mode) { setDataMode(cs_pin, _mode); };
	void setClockDivider(unsigned char _div) { setClockDivider(cs_pin, _div); };

  private:
	void init();
	unsigned char cs_pin;

};


extern SPIClass SPI;


// For compatibility with sketches designed for AVR @ 16 MHz
// New programs should use SPI.beginTransaction to set the SPI clock
#define SPI_CLOCK_DIV2	 3
#define SPI_CLOCK_DIV4	 6
#define SPI_CLOCK_DIV8	 12
#define SPI_CLOCK_DIV16	 24
#define SPI_CLOCK_DIV32	 48
#define SPI_CLOCK_DIV64	 96
#define SPI_CLOCK_DIV128 128

#endif
