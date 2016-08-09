 /**
 * @file ard_wifi.h
 * @brief Definitions wifi driver headers for Fireduino
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
#ifndef _ARDUINO_WIFI_H_
#define _ARDUINO_WIFI_H_

#include "IPAddress.h"

#ifdef __cplusplus
extern "C" {
#endif

void ard_wifi_init(void);
int ard_wifi_connect(const char* ssid, const char *pass);
int ard_wifi_smart_config(unsigned int s,char *ssid);
int ard_wifi_info_start(char *ssid);



#ifdef __cplusplus
}
#endif

IPAddress ard_wifi_get_ip_addr(void);

#endif


