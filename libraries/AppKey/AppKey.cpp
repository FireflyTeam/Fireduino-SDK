 /**
 * @file AppKey.cpp
 * @brief Definitions AppKey class for Fireduino
 * @author jiangdz<jdz@t-chip.com.cn>
 * @version V1.0
 * @date 2016.06
 *
 * @par Copyright:
 * Copyright (c) 2016 T-CHIP INTELLIGENCE TECHNOLOGY CO.,LTD. \n\n
 *
 * For more information, please visit website <http://www.t-firefly.com/>, \n\n
 * or email to <service@t-firefly.com>.
 */
#include <AppKey.h>
#include "ard_app_key.h"

AppKeyClass::AppKeyClass()
{

}

void AppKeyClass::begin(AppKeyDealCallBack key_event_process)
{
	AppKeyInit(key_event_process);
}


AppKeyClass	AppKey;

