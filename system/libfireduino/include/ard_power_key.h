 /**
 * @file TFTDisplayText.c
 * @brief Definitions TFTDisplayText class for Fireduino
 * @author huangab<hab@t-chip.com.cn>
 * @version V1.0
 * @date 2016.02
 *
 * @par Copyright:
 * Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
 *
 * For more information, please visit website <http://www.t-firefly.com/>, \n\n
 * or email to <service@t-firefly.com>.
 */

#ifndef ARDUINO_LIB_ARD_POWER_KEY_H_
#define ARDUINO_LIB_ARD_POWER_KEY_H_

#include "io.h"
#include "ard_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	KEY_COUNT_DOWN			1
#define	KEY_COUNT_PRESS_START	20
#define KEY_COUNT_PRESS_STEP	4

#define	KEY_STATUS_NONE			((unsigned int)0x0000	<< 16)
#define	KEY_STATUS_DOWN			((unsigned int)0x0001	<< 16)
#define	KEY_STATUS_PRESS_START	((unsigned int)0x0002	<< 16)
#define	KEY_STATUS_PRESS		((unsigned int)0x0003	<< 16)
#define	KEY_STATUS_SHORT_UP		((unsigned int)0x0004	<< 16)
#define	KEY_STATUS_LONG_UP		((unsigned int)0x0005	<< 16)
#define	KEY_STATUS_DOUBLE_CLICK ((unsigned int)0x0006	<< 16)

typedef struct PowerKeyEnv
{
	unsigned int key_status;
} PowerKeyEnv;

extern PowerKeyEnv PowerKeyInfo;


extern void PowerKeyIntInit(unsigned int PKIntEnable,void (*CallBack)(),enum int_tri_mode mode);
unsigned int GetPowerKeyStatus(void);
extern int PowerKeyInit(void (*CallBack)(unsigned int status));

#ifdef __cplusplus
}
#endif




#endif /* ARDUINO_LIB_ARD_POWER_KEY_H_ */
