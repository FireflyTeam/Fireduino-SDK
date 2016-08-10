 /**
 * @file PowerKey.cpp
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
#include <PowerKey.h>


PowerKeyClass::PowerKeyClass()
{

}


void PowerKeyClass::begin(void (*CallBack)(unsigned int status))
{
	PowerKeyInit(CallBack);
}


PowerKeyClass	PowerKey;
