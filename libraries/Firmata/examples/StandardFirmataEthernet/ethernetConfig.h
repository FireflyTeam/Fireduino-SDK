 /**
 * @file ethernetConfig.h
 * @brief Definitions ethernetConfig class for Fireduino
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
#define WIZ5100_ETHERNET

#ifdef WIZ5100_ETHERNET
#include <SPI.h>
#include <Ethernet.h>
EthernetClient client;
#endif

/*
 * OPTION B: Configure for Arduin Yun
 *
 * The Ethernet port on the Arduino Yun board can be used with Firmata in this configuration.
 *
 * To execute StandardFirmataEthernet on Yun uncomment the YUN_ETHERNET define below and make
 * sure the WIZ5100_ETHERNET define (above) is commented out.
 *
 * On Yun there's no need to configure local_ip and mac address as this is automatically
 * configured on the linux-side of Yun.
 */
//#define YUN_ETHERNET

#ifdef YUN_ETHERNET
#include <Bridge.h>
#include <YunClient.h>
YunClient client;
#endif


// STEP 2 [REQUIRED for all boards and shields]
// replace with IP of the server you want to connect to, comment out if using 'remote_host'
#define remote_ip IPAddress(10, 0, 0, 3)
// *** REMOTE HOST IS NOT YET WORKING ***
// replace with hostname of server you want to connect to, comment out if using 'remote_ip'
// #define remote_host "server.local"

// STEP 3 [REQUIRED unless using Arduin Yun]
// Replace with the port that your server is listening on
#define remote_port 3030

// STEP 4 [REQUIRED unless using Arduino Yun OR if not using DHCP]
// Replace with your board or ethernet shield's IP address
// Comment out if you want to use DHCP
#define local_ip IPAddress(10, 0, 0, 15)

// STEP 5 [REQUIRED unless using Arduino Yun]
// replace with ethernet shield mac. Must be unique for your network
const byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0x53, 0xE5};

/*==============================================================================
 * CONFIGURATION ERROR CHECK (don't change anything here)
 *============================================================================*/

#if !defined WIZ5100_ETHERNET && !defined YUN_ETHERNET
#error "you must define either WIZ5100_ETHERNET or YUN_ETHERNET in ethernetConfig.h"
#endif

#if defined remote_ip && defined remote_host
#error "cannot define both remote_ip and remote_host at the same time in ethernetConfig.h"
#endif

/*==============================================================================
 * PIN IGNORE MACROS (don't change anything here)
 *============================================================================*/

// ignore SPI pins, pin 10 (Ethernet SS) and pin 4 (SS for SD-Card on Ethernet shield)
#define IS_IGNORE_ETHERNET_SHIELD(p) ((IS_PIN_SPI(p) || (p) == 4) || (p) == 10)
