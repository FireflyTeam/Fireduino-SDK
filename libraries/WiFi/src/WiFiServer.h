 /**
 * @file WiFiServer.h
 * @brief Definitions WiFiServer class for Fireduino
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
#ifndef wifiserver_h
#define wifiserver_h

extern "C" {
  #include "utility/wl_definitions.h"
}

#include "Server.h"
#define MAX_CLIENT_SOCK_NUM 16

class WiFiClient;

class WiFiServer : public Server {
private:
  uint8_t _sock;
  uint16_t _port;
  uint8_t _clientsocks[MAX_CLIENT_SOCK_NUM];
  void onClientStop(uint8_t s);
public:
  WiFiServer(uint16_t);
  virtual ~WiFiServer() {}
  WiFiClient available(bool newConnOnly=false);
  void begin();
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  size_t write(uint8_t b, WiFiClient *c);
  size_t write(const uint8_t *buf, size_t size, WiFiClient *c);
  uint8_t status();

  using Print::write;

  friend class WiFiClient;
};

#endif
