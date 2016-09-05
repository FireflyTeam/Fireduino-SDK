 /**
 * @file ard_uart.h
 * @brief Definitions uart driver header class for Fireduino
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
#ifndef _ARDUINO_UART_H_
#define _ARDUINO_UART_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _LineInfo
{
  unsigned int 		dwDTERate;
  unsigned char  	bCharFormat;
  unsigned char  	bParityType;
  unsigned char  	bDataBits;
  unsigned char		lineState;
}LineInfo;


extern void ard_usbserial_init(void);
extern unsigned int  ard_usbserial_putc(unsigned char *buf,unsigned int len);
extern unsigned char ard_usbserial_getc(void);
extern unsigned char ard_usbserial_peek(void);
extern unsigned int ard_usbserial_data_available(void);
extern void ard_usbserial_flush(void);

extern LineInfo *ard_get_line_info(void);

#ifdef __cplusplus
}
#endif


#endif


