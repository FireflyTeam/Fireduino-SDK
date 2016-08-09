/*
 * ard_debug.h
 *
 *  Created on: 2016Äê3ÔÂ15ÈÕ
 *      Author: dujw
 */

#ifndef ARDUINO_LIB_ARD_DEBUG_H_
#define ARDUINO_LIB_ARD_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

void dbg_print(const char *s);
void dbg_println(const char *x);
void dbg_print_hex(int x);
void dbg_println_num(int x);
void dbg_print_num(int x);


void dbg_trace_start();
void dbg_trace_stop();

extern int __dbg_trace_start;

#define dbg_trace() \
	do { \
		if (__dbg_trace_start) { \
		dbg_print("==> TRACE "); \
		dbg_print(__FILE__); \
		dbg_print(":"); \
		dbg_print( __FUNCTION__ ); \
		dbg_print(":"); \
		dbg_println_num(__LINE__); \
		} \
	} while (0)
#ifdef __cplusplus
}
#endif

#endif /* ARDUINO_LIB_ARD_DEBUG_H_ */
