 /**
 * @file WiFiUdp.c
 * @brief Definitions WiFiUdp class for Fireduino
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
  #include "utility/debug.h"
}
#include <string.h>
#include "utility/server_drv.h"
#include "utility/wifi_drv.h"

#include "WiFi.h"
#include "WiFiUdp.h"
#include "WiFiClient.h"
#include "WiFiServer.h"


/* Constructor */
WiFiUDP::WiFiUDP() {
  _sock = NO_SOCKET_AVAIL;
  _port = 0;
}

/* Start WiFiUDP socket, listening at local port PORT */
uint8_t WiFiUDP::begin(uint16_t port) {
  uint8_t ok = 0;

  if (_sock != NO_SOCKET_AVAIL) {
    ServerDrv::closeSocket(_sock);
  }
  _sock = ServerDrv::createSocket(UDP_MODE);
  if (_sock != NO_SOCKET_AVAIL) {
    ServerDrv::startUdpServer(port, _sock);
    _port = port;
    ok = 1;
  }
  resetBufState();
  return ok;
}

/* return number of bytes available in the current packet,
   will return zero if parsePacket hasn't been called yet */
int WiFiUDP::available() {
  // reset write state when read
  _write_len = 0;

  // return cached data
  if (_read_pos < _read_len)
    return _read_len - _read_pos;

  // real read
  _read_len = _read_pos = 0;
  uint16_t size = sizeof(_packet_buf);
  if (ServerDrv::availData(_sock) &&
      ServerDrv::getUdpDataBuf(_sock, _packet_buf, &size,
                               _remoteIP, &_remotePort)) {
    _read_len = size;
    return size;
  }
  return 0;
}

/* Release any resources being used by this WiFiUDP instance */
void WiFiUDP::stop()
{
	  if (_sock == NO_SOCKET_AVAIL)
	    return;

	  ServerDrv::closeSocket(_sock);
	  _sock = NO_SOCKET_AVAIL;
}

int WiFiUDP::beginPacket(const char *host, uint16_t port)
{
  // reset read state when write
  resetBufState();
	// Look up the host first
	IPAddress remote_addr;
	if (WiFi.hostByName(host, remote_addr)) {
		return beginPacket(remote_addr, port);
	}

	return 0;
}

int WiFiUDP::beginPacket(IPAddress ip, uint16_t port)
{
  // reset read state when write
  resetBufState();
  if (_sock != NO_SOCKET_AVAIL)
  {
	  // ServerDrv::startClient(uint32_t(ip), port, _sock);
    _remoteIP = ip;
    _remotePort = port;
	  return 1;
  }
  return 0;
}

int WiFiUDP::endPacket()
{
  int rc = 0;
  if (_write_len > 0) {
    rc = ServerDrv::sendUdpDataBuf(_sock, _packet_buf, _write_len,
                                   _remoteIP, _remotePort);
    _write_len = 0;
  }
	return rc;
}

size_t WiFiUDP::write(uint8_t byte)
{
  if (_write_len < UDP_PACKET_MAX_SIZE) {
    _packet_buf[_write_len++] = byte;
    return 1;
  }
  return 0;
}

size_t WiFiUDP::write(const uint8_t *buffer, size_t size)
{
  if (size <= 0)
    return 0;

  size_t free_size = UDP_PACKET_MAX_SIZE - _write_len;
  if (free_size <= 0)
    return 0;

  size_t written_size = min(free_size, size);
  memcpy(&_packet_buf[_write_len], buffer, written_size);
  _write_len += written_size;
  return written_size;
}

int WiFiUDP::parsePacket()
{
	return available();
}

int WiFiUDP::read()
{
  if (available()) {
    uint8_t b;
    b = _packet_buf[_read_pos++];
    return b;
  }

  return -1;
}

int WiFiUDP::read(unsigned char* buffer, size_t len)
{
  if (available()) {
	  uint16_t size = _read_len - _read_pos;
    if (size > len)
      size = len;
    memcpy(buffer, &_packet_buf[_read_pos], size);
    _read_pos += size;
	  return size;
  }else{
	  return -1;
  }
}

int WiFiUDP::peek()
{
  if (available()) {
    uint8_t b;
    b = _packet_buf[_read_pos];
    return b;
  }
  return -1;
}

void WiFiUDP::flush()
{
  while (available() >= 0)
    resetBufState();
}

IPAddress  WiFiUDP::remoteIP()
{
  return _remoteIP;
}

uint16_t  WiFiUDP::remotePort()
{
  return _remotePort;
}

void WiFiUDP::resetBufState()
{
  _read_len = _read_pos = _write_len = 0;
}
