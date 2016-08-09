 /**
 * @file server_drv.c
 * @brief Definitions server_drv class for Fireduino
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
#ifndef Server_Drv_h
#define Server_Drv_h

#include <inttypes.h>
#include "IPAddress.h"

enum wl_tcp_state {
  CLOSED      = 0,
  LISTEN      = 1,
  SYN_SENT    = 2,
  SYN_RCVD    = 3,
  ESTABLISHED = 4,
  FIN_WAIT_1  = 5,
  FIN_WAIT_2  = 6,
  CLOSE_WAIT  = 7,
  CLOSING     = 8,
  LAST_ACK    = 9,
  TIME_WAIT   = 10
};

typedef enum eProtMode {TCP_MODE, UDP_MODE}tProtMode;

class ServerDrv
{
public:
    static uint8_t createSocket(uint8_t protMode=TCP_MODE);

    static uint8_t acceptSocket(uint8_t sock);

    // Start server TCP on port specified
    static void startServer(uint16_t port, uint8_t sock);

    static void startClient(uint32_t ipAddress, uint16_t port, uint8_t sock);

    static void closeSocket(uint8_t sock);

    // Start server UDP on port specified
    static void startUdpServer(uint16_t port, uint8_t sock);

    static uint8_t getServerState(uint8_t sock);

    static uint8_t getClientState(uint8_t sock);

    static bool getData(uint8_t sock, uint8_t *data, uint8_t peek = 0);

    static bool getDataBuf(uint8_t sock, uint8_t *data, uint16_t *len);

    static bool sendData(uint8_t sock, const uint8_t *data, uint16_t len);

    static bool sendUdpDataBuf(uint8_t sock_index, const uint8_t *data, uint16_t len, uint32_t remoteIP, uint16_t remotePort);

    static bool getUdpDataBuf(uint8_t sock_index, uint8_t *_data, uint16_t *_dataLen, IPAddress& remoteIP, uint16_t *remotePort);

    static bool availData(uint8_t sock);

    static uint8_t availData(uint8_t *sock_indexes, int num);
};

extern ServerDrv serverDrv;

#endif
