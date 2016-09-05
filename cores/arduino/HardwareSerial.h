 /**
 * @file HardwareSerial.h
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
#ifndef _WIRISH_HARDWARESERIAL_H_
#define _WIRISH_HARDWARESERIAL_H_


#include "Print.h"
#include "Stream.h"



#define	DATA_5B		5
#define	DATA_6B		6
#define	DATA_7B		7
#define	DATA_8B		8

#define	ONE_STOPBIT					0
#define	ONE_AND_HALF_OR_TWO_STOPBIT	1

#define	PARITY_ODD		0
#define	PARITY_EVEN		1
#define	PARITY_DISABLE	2

#define SERIAL_8N1	HardwareSerial::Mode_8N1


#define SERIAL_5O1	HardwareSerial::Mode_5O1
#define SERIAL_6O1	HardwareSerial::Mode_6O1
#define SERIAL_7O1	HardwareSerial::Mode_7O1
#define SERIAL_8O1	HardwareSerial::Mode_8O1

#define SERIAL_5E1	HardwareSerial::Mode_5E1
#define SERIAL_6E1	HardwareSerial::Mode_6E1
#define SERIAL_7E1	HardwareSerial::Mode_7E1
#define SERIAL_8E1	HardwareSerial::Mode_8E1

#define SERIAL_5N1	HardwareSerial::Mode_5N1
#define SERIAL_6N1	HardwareSerial::Mode_6N1
#define SERIAL_7N1	HardwareSerial::Mode_7N1
#define SERIAL_8N1	HardwareSerial::Mode_8N1

#define SERIAL_5O2	HardwareSerial::Mode_5O2
#define SERIAL_6O2	HardwareSerial::Mode_6O2
#define SERIAL_7O2	HardwareSerial::Mode_7O2
#define SERIAL_8O2	HardwareSerial::Mode_8O2

#define SERIAL_5E2	HardwareSerial::Mode_5E2
#define SERIAL_6E2	HardwareSerial::Mode_6E2
#define SERIAL_7E2	HardwareSerial::Mode_7E2
#define SERIAL_8E2	HardwareSerial::Mode_8E2

#define SERIAL_5N2	HardwareSerial::Mode_5N2
#define SERIAL_6N2	HardwareSerial::Mode_6N2
#define SERIAL_7N2	HardwareSerial::Mode_7N2
#define SERIAL_8N2	HardwareSerial::Mode_8N2

class HardwareSerial : public Stream {

public:

	enum CONFIG_MODE{
		Mode_5O1	=	(PARITY_ODD << 5) | (ONE_STOPBIT << 4) | (DATA_5B << 0),
		Mode_6O1	=	(PARITY_ODD << 5) | (ONE_STOPBIT << 4) | (DATA_6B << 0),
		Mode_7O1	=	(PARITY_ODD << 5) | (ONE_STOPBIT << 4) | (DATA_7B << 0),
		Mode_8O1	=	(PARITY_ODD << 5) | (ONE_STOPBIT << 4) | (DATA_8B << 0),

		Mode_5E1	=	(PARITY_EVEN << 5) | (ONE_STOPBIT << 4) | (DATA_5B << 0),
		Mode_6E1	=	(PARITY_EVEN << 5) | (ONE_STOPBIT << 4) | (DATA_6B << 0),
		Mode_7E1	=	(PARITY_EVEN << 5) | (ONE_STOPBIT << 4) | (DATA_7B << 0),
		Mode_8E1	=	(PARITY_EVEN << 5) | (ONE_STOPBIT << 4) | (DATA_8B << 0),

		Mode_5N1	=	(PARITY_DISABLE << 5) | (ONE_STOPBIT << 4) | (DATA_5B << 0),
		Mode_6N1	=	(PARITY_DISABLE << 5) | (ONE_STOPBIT << 4) | (DATA_6B << 0),
		Mode_7N1	=	(PARITY_DISABLE << 5) | (ONE_STOPBIT << 4) | (DATA_7B << 0),
		Mode_8N1	=	(PARITY_DISABLE << 5) | (ONE_STOPBIT << 4) | (DATA_8B << 0),

		Mode_5O2	=	(PARITY_ODD << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_5B << 0),
		Mode_6O2	=	(PARITY_ODD << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_6B << 0),
		Mode_7O2	=	(PARITY_ODD << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_7B << 0),
		Mode_8O2	=	(PARITY_ODD << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_8B << 0),

		Mode_5E2	=	(PARITY_EVEN << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_5B << 0),
		Mode_6E2	=	(PARITY_EVEN << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_6B << 0),
		Mode_7E2	=	(PARITY_EVEN << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_7B << 0),
		Mode_8E2	=	(PARITY_EVEN << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_8B << 0),

		Mode_5N2	=	(PARITY_DISABLE << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_5B << 0),
		Mode_6N2	=	(PARITY_DISABLE << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_6B << 0),
		Mode_7N2	=	(PARITY_DISABLE << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_7B << 0),
		Mode_8N2	=	(PARITY_DISABLE << 5) | (ONE_AND_HALF_OR_TWO_STOPBIT << 4) | (DATA_8B << 0),
	};


    HardwareSerial(unsigned char  dev);
    unsigned char dev_id;
    /* Set up/tear down */
    void begin(uint32_t baud);
    void begin(uint32_t baud,uint8_t config);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    operator bool() { return true; }; // UART always active
    using Print::write;
};

extern HardwareSerial Serial1;
extern HardwareSerial Serial2;
#endif
