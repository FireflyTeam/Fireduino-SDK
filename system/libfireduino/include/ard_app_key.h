 /**
 * @file ard_app_key.h
 * @brief Definitions app_key for Fireduino
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

#ifndef ARDUINO_LIB_ARD_APP_KEY_H_
#define ARDUINO_LIB_ARD_APP_KEY_H_

#include "io.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	APP_KEY_UDP_BOARDCAST_CONNECT_PORT		7777
#define	APP_KEY_UDP_KEY_DEAL_PORT				8888
#define	APP_KEY_TCP_HEART_PORT					9999
#define	EVENT_MOVE_CHARACTERS   				"move "
#define	EVENT_KEY_UP_CHARACTERS   				"key up "
#define	EVENT_KEY_DOWN_CHARACTERS   			"key down "
#define	EVENT_KEY_REPEAT_CHARACTERS   			"key repeat "
#define	EVENT_KEY_CLICK_CHARACTERS   			"key click "
#define HEART_ACK_SPRING 						"online_ack\r\n"
#define	DEVICE_NAME  							"Fireduino"
#define	BOARDCAST_KEY_VALUE						"love me?"
#define	HEART_KEY_SPRING 						"online"
#define	UIRECNT_EVENT_KEY_SPRING				"UI recent"
#define	SHUTDOWN_KEY_SPRING 					"shutdown"
#define	TEXT_EVEMT_KEY_SPRING 					 "TEXT "
#define	MAX_UDP_PACKET_LEN						64
#define TCP_SERVER_RX_BUFSIZE	64		//����tcp server��������ݳ���

typedef enum
{
	KEYVALUE_RETURN = 1,
	KEYVALUE_NUM1 = 2,
	KEYVALUE_NUM2 = 3,
	KEYVALUE_NUM3 = 4,
	KEYVALUE_NUM4 = 5,
	KEYVALUE_NUM5 = 6,
	KEYVALUE_NUM6 = 7,
	KEYVALUE_NUM7 = 8,
	KEYVALUE_NUM8 = 9,
	KEYVALUE_NUM9 = 10,
	KEYVALUE_NUM0 = 11,
	KEYVALUE_O = 28,
	KEYVALUE_UP = 103,
	KEYVALUE_DOWN = 108,
	KEYVALUE_LEFT = 105,
	KEYVALUE_RIGHT = 106,
	KEYVALUE_OK = 352,
	KEYVALUE_SELECT = 353,
	KEYVALUE_BUTTON_LEFT = 272,
	KEYVALUE_BUTTON_RIGHT = 273,
	KEYVALUE_VOL_MUTE = 113,
	KEYVALUE_VOL_DOWN = 114,
	KEYVALUE_VOL_UP = 115,
	KEYVALUE_MENU = 139,
	KEYVALUE_HOME = 172,
	KEYVALUE_BUTTON_A = 304,
	KEYVALUE_BUTTON_B = 305,
	KEYVALUE_BUTTON_C = 306,
	KEYVALUE_BUTTON_D = 307,
	KEYVALUE_BUTTON_SELECT = 314,
	KEYVALUE_BUTTON_START = 315,
}app_key_num_t;

typedef enum
{
	KEY_EVENT_NULL,
	KEY_EVENT_DOWN,
	KEY_EVENT_UP,
	KEY_EVENT_MOVE,
	KEY_EVNET_CLICK,
	KEY_EVENT_REPEAT,
	KEY_EVENT_TXT,
	KEY_EVENT_SHUTDOWN,
	KEY_EVENT_UIRECENT,
}app_key_type_t;

#pragma pack(1)
typedef struct
{
	int event;
	char event_data[16];
}event_struct_t;
#pragma pack()

typedef	void (*AppKeyDealCallBack)(unsigned int key_event, void* arg0, void* arg1);

extern void AppKeyInit(AppKeyDealCallBack key_process);

#ifdef __cplusplus
}
#endif

#endif /* ARDUINO_LIB_ARD_POWER_KEY_H_ */

