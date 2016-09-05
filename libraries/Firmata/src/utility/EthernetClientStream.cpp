 /**
 * @file EthernetClientStream.c
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
#include "EthernetClientStream.h"
#include <Arduino.h>

#define SERIAL_DEBUG
#include "firmataDebug.h"

#define MILLIS_RECONNECT 5000

EthernetClientStream::EthernetClientStream(Client &client, IPAddress localip, IPAddress ip, const char* host, uint16_t port)
: client(client),
  localip(localip),
  ip(ip),
  host(host),
  port(port),
  connected(false)
{
}

int
EthernetClientStream::available()
{
  return maintain() ? client.available() : 0;
}

int
EthernetClientStream::read()
{
  return maintain() ? client.read() : -1;
}

int
EthernetClientStream::peek()
{
  return maintain() ? client.peek() : -1;
}

void EthernetClientStream::flush()
{
  if (maintain())
    client.flush();
}

size_t
EthernetClientStream::write(uint8_t c)
{
  return maintain() ? client.write(c) : 0;
}

void
EthernetClientStream::maintain(IPAddress localip)
{
// temporary hack to Firmata to compile for Intel Galileo
// the issue is documented here: https://github.com/firmata/arduino/issues/218
#if !defined(ARDUINO_LINUX)
  // ensure the local IP is updated in the case that it is changed by the DHCP server
  if (this->localip != localip)
    {
      this->localip = localip;
      if (connected)
        stop();
    }
#endif
}

void
EthernetClientStream::stop()
{
  client.stop();
  connected = false;
  time_connect = millis();
}

bool
EthernetClientStream::maintain()
{
  if (client && client.connected())
    return true;

  if (connected)
    {
      stop();
    }
  // if the client is disconnected, attempt to reconnect every 5 seconds
  else if (millis()-time_connect >= MILLIS_RECONNECT)
    {
      connected = host ? client.connect(host, port) : client.connect(ip, port);
      if (!connected) {
        time_connect = millis();
        DEBUG_PRINTLN("connection failed. attempting to reconnect...");
      } else {
        DEBUG_PRINTLN("connected");
      }
    }
  return connected;
}
