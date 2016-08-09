 /**
 * @file WiFiServer.cpp
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
#include <string.h>
#include "utility/server_drv.h"

extern "C" {
  #include "utility/debug.h"
}

#include "WiFi.h"
#include "WiFiClient.h"
#include "WiFiServer.h"

WiFiServer::WiFiServer(uint16_t port)
{
  _sock = NO_SOCKET_AVAIL;
  _port = port;
}

void WiFiServer::begin()
{
  if (_sock != NO_SOCKET_AVAIL) {
    ServerDrv::closeSocket(_sock);
  }
  for (int i = 0; i < MAX_CLIENT_SOCK_NUM; i++) {
    _clientsocks[i] = NO_SOCKET_AVAIL;
  }
  _sock = ServerDrv::createSocket();
  if (_sock != NO_SOCKET_AVAIL) {
    ServerDrv::startServer(_port, _sock);
  } else {
    Serial.println("No Socket available");
  }
}

WiFiClient WiFiServer::available(bool newConnOnly) {
  uint8_t client_index;
  uint8_t s;

  // first check available slot
  for (client_index = 0; client_index < MAX_CLIENT_SOCK_NUM; client_index++) {
    if (_clientsocks[client_index] == NO_SOCKET_AVAIL) {
      break;
    }
  }
  if (client_index < MAX_CLIENT_SOCK_NUM) {
    // do accept socket if not full
    s = ServerDrv::acceptSocket(_sock);
    if (s != NO_SOCKET_AVAIL) {
      _clientsocks[client_index] = s;
      return WiFiClient(s, this);
    }
  }

  if (newConnOnly)
	  return WiFiClient(NO_SOCKET_AVAIL);

  // check availability of client sockets
  s = ServerDrv::availData(_clientsocks, MAX_CLIENT_SOCK_NUM);
  return WiFiClient(s, this);
}


uint8_t WiFiServer::status() {
  if (_sock == NO_SOCKET_AVAIL) {
    return CLOSED;
  } else {
    return ServerDrv::getServerState(_sock);
  }
}


size_t WiFiServer::write(uint8_t b)
{
  return write(&b, 1, NULL);
}

size_t WiFiServer::write(const uint8_t *buffer, size_t size)
{
  return write(buffer, size, NULL);
}

size_t WiFiServer::write(uint8_t b, WiFiClient *c)
{
  return write(&b, 1, c);
}

size_t WiFiServer::write(const uint8_t *buffer, size_t size, WiFiClient *c)
{
  int written = 0;
  for (int i = 0; i < MAX_CLIENT_SOCK_NUM; i++) {
    if (_clientsocks[i] != NO_SOCKET_AVAIL &&
        (c == NULL || _clientsocks[i] != c->_sock)) {
      WiFiClient c(_clientsocks[i]);
      written += c.write(buffer, size);
    }
  }
  return written ? size : 0;
}

void WiFiServer::onClientStop(uint8_t s) {
  for (int i = 0; i < MAX_CLIENT_SOCK_NUM; i++) {
    if (_clientsocks[i] == s) {
      _clientsocks[i] = NO_SOCKET_AVAIL;
      break;
    }
  }
}
