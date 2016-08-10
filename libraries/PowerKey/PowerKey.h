 /**
 * @file PowerKey.h
 * @brief Definitions PowerKey class for Fireduino
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

#ifndef ARDUINO_LIBRARIES_POWERKEY_POWERKEY_H_
#define ARDUINO_LIBRARIES_POWERKEY_POWERKEY_H_

#include <ard_power_key.h>

#define		EVEN_POWERKEY_PRESS_START		((unsigned int)0x0002	<< 16)
#define		EVEN_POWERKEY_PRESS				((unsigned int)0x0003	<< 16)
#define		EVEN_POWERKEY_RELEASE_SHORT		((unsigned int)0x0004	<< 16)
#define		EVEN_POWERKEY_RELEASE_LONG		((unsigned int)0x0005	<< 16)
#define		EVEN_POWERKEY_DOUBLE_CLINK		((unsigned int)0x0006	<< 16)



class PowerKeyClass
{

public:
	PowerKeyClass();

	void begin(void (*CallBack)(unsigned int status));
 private:

};

extern PowerKeyClass PowerKey;

#endif /* ARDUINO_LIBRARIES_POWERKEY_POWERKEY_H_ */
