 /**
 * @file ard_sys.h
 * @brief Definitions system variable for Fireduino
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
#ifndef ARDUINO_LIB_ARD_SYS_H_
#define ARDUINO_LIB_ARD_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

// get systick count value
extern unsigned long get_SystickCount(void);
extern unsigned int get_sys_random_seed(void);

#ifdef __cplusplus
}
#endif



#endif /* ARDUINO_LIB_ARD_SYS_H_ */
