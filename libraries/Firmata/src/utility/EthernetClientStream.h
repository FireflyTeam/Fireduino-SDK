 /**
 * @file EthernetClientStream.h
 * @brief Definitions EthernetClientStream class for Fireduino
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
#ifndef ETHERNETCLIENTSTREAM_H
#define ETHERNETCLIENTSTREAM_H

#include <inttypes.h>
#include <stdio.h>
#include <Stream.h>
#include <Client.h>
#include <IPAddress.h>

class EthernetClientStream : public Stream
{
public:
  EthernetClientStream(Client &client, IPAddress localip, IPAddress ip, const char* host, uint16_t port);
  virtual ~EthernetClientStream() {}
  int available();
  int read();
  int peek();
  void flush();
  size_t write(uint8_t);
  void maintain(IPAddress localip);
  bool maintain();

private:
  Client &client;
  IPAddress localip;
  IPAddress ip;
  const char* host;
  uint16_t port;
  bool connected;
  uint32_t time_connect;
  void stop();
};

#endif
