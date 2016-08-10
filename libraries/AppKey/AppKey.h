 /**
 * @file AppKey.h
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

#ifndef ARDUINO_LIBRARIES_APPKEY_H_
#define ARDUINO_LIBRARIES_APPKEY_H_
#include "ard_app_key.h"

typedef enum
{
	VALUE_RETURN = 1,
	VALUE_NUM1 = 2,
	VALUE_NUM2 = 3,
	VALUE_NUM3 = 4,
	VALUE_NUM4 = 5,
	VALUE_NUM5 = 6,
	VALUE_NUM6 = 7,
	VALUE_NUM7 = 8,
	VALUE_NUM8 = 9,
	VALUE_NUM9 = 10,
	VALUE_NUM0 = 11,
	VALUE_O = 28,
	VALUE_UP = 103,
	VALUE_DOWN = 108,
	VALUE_LEFT = 105,
	VALUE_RIGHT = 106,
	VALUE_OK = 352,
	VALUE_SELECT = 353,
	VALUE_BUTTON_LEFT = 272,
	VALUE_BUTTON_RIGHT = 273,
	VALUE_VOL_MUTE = 113,
	VALUE_VOL_DOWN = 114,
	VALUE_VOL_UP = 115,
	VALUE_MENU = 139,
	VALUE_HOME = 172,
	VALUE_BUTTON_A = 304,
	VALUE_BUTTON_B = 305,
	VALUE_BUTTON_C = 306,
	VALUE_BUTTON_D = 307,
	VALUE_BUTTON_SELECT = 314,
	VALUE_BUTTON_START = 315,
}key_num_t;

typedef enum
{
	EVENT_NULL,
	EVENT_DOWN,
	EVENT_UP,
	EVENT_MOVE,
	EVNET_CLICK,
	EVENT_REPEAT,
	EVENT_TXT,
	EVENT_SHUTDOWN,
	EVENT_UIRECENT,
}key_type_t;

class AppKeyClass
{

public:
	AppKeyClass();

	void begin(AppKeyDealCallBack key_event_process);
 private:

};

extern AppKeyClass AppKey;

#endif /* ARDUINO_LIBRARIES_POWERKEY_POWERKEY_H_ */

