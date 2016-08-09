 /**
 * @file WiFiClient.cpp
 * @brief Definitions WiFiClient class for Fireduino
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
extern "C" {
  #include "utility/wl_definitions.h"
  #include "utility/wl_types.h"
  #include "string.h"
  #include "utility/debug.h"
}

#include "WiFi.h"
#include "WiFiClient.h"
#include "WiFiServer.h"
#include "utility/server_drv.h"


uint16_t WiFiClient::_srcport = 1024;

WiFiClient::WiFiClient() : WiFiClient(NO_SOCKET_AVAIL, NULL) {
}

WiFiClient::WiFiClient(uint8_t sock) : WiFiClient(sock, NULL) {
}

WiFiClient::WiFiClient(uint8_t sock, WiFiServer *server) :
  _sock(sock),
  _server(server) {
}

int WiFiClient::connect(const char* host, uint16_t port) {
	IPAddress remote_addr;
	if (WiFi.hostByName(host, remote_addr))
	{
		return connect(remote_addr, port);
	}
	return 0;
}

int WiFiClient::connect(IPAddress ip, uint16_t port) {
	if (_sock != NO_SOCKET_AVAIL) {
    Serial.println("Socket already used.");
    return 0;
  }
  _sock = ServerDrv::createSocket();
  if (_sock != NO_SOCKET_AVAIL) {
    ServerDrv::startClient(uint32_t(ip), port, _sock);

    unsigned long start = millis();

    // wait 8 second for the connection to close
    while (!connected() && millis() - start < 8000)
      delay(100);

    if (connected()) {
      return 1;
    } else {
      stop();
      return 0;
    }
  } else {
    Serial.println("No Socket available");
    return 0;
  }
}

size_t WiFiClient::write(uint8_t b) {
	  return write(&b, 1);
}

size_t WiFiClient::write(const uint8_t *buf, size_t size) {
  if (!ServerDrv::sendData(_sock, buf, size)) {
	  setWriteError();
    return 0;
  }
  return size;
}

int WiFiClient::available() {
  return ServerDrv::availData(_sock) && peek() >= 0;
}


int WiFiClient::read() {
	uint8_t b;

	if (ServerDrv::getData(_sock, &b, 0)) {
		return b;
	} else {
		return -1;
	}
}


int WiFiClient::read(uint8_t* buf, size_t size) {
  // sizeof(size_t) is architecture dependent
  // but we need a 16 bit data type here
  uint16_t _size = size;
  if (!ServerDrv::getDataBuf(_sock, buf, &_size))
    return -1;
  return 0;
}

int WiFiClient::peek() {
	uint8_t b;

	if (ServerDrv::getData(_sock, &b, 1)) {
		return b;
	} else {
		return -1;
	}
}

void WiFiClient::flush() {
  while (available())
    read();
}

void WiFiClient::stop() {
  if (_server != NULL)
    _server->onClientStop(_sock);
  ServerDrv::closeSocket(_sock);
  _sock = NO_SOCKET_AVAIL;
  _server = NULL;
}

uint8_t WiFiClient::connected()
{
  uint8_t s = status();

  return !(s == LISTEN || s == CLOSED || s == FIN_WAIT_1 ||
           s == FIN_WAIT_2 || s == TIME_WAIT ||
           s == SYN_SENT || s== SYN_RCVD ||
           (s == CLOSE_WAIT));
}

uint8_t WiFiClient::status()
{
  return ServerDrv::getClientState(_sock);
}

WiFiClient::operator bool()
{
  return _sock != NO_SOCKET_AVAIL;
}
