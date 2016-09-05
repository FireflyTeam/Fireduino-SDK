 /**
 * @file WebsocketStream.h
 * @brief Definitions WebsocketStream class for Fireduino
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
#ifndef WEBSOCK_STREAM_H
#define WEBSOCK_STREAM_H

#include <inttypes.h>
#include <Stream.h>
#include <WiFi.h>
#include <WebSocket.h>

#ifndef WEBSOCKSTREAM_PORT
#define WEBSOCKSTREAM_PORT 3030
#endif

#ifndef WEBSOCKSTREAM_PREFIX
#define WEBSOCKSTREAM_PREFIX "/"
#endif

#define BUFFER_SIZE (512)

// Simple circular queue, with read write support
class Queue {
  size_t _read_pos, _write_pos;
public:
  // uint8_t _buf[BUFFER_SIZE];
  uint8_t *_buf;

  Queue() {
    reset();
    _buf = new uint8_t[BUFFER_SIZE];
  }

  inline void reset() {
    _read_pos = _write_pos = 0;
  }

  inline int length() {
    int len = (int)_write_pos - (int)_read_pos;
    if (len < 0)
      len += BUFFER_SIZE;
    return len;
  }

  inline int peek() {
    if (length() == 0)
      return -1;
    return _buf[_read_pos];
  }

  inline int read() {
    if (length() == 0)
      return -1;
    int c = _buf[_read_pos];
    if (++_read_pos >= BUFFER_SIZE)
      _read_pos = 0;
    if (_read_pos == _write_pos)
      reset();
    return c;
  }

  inline size_t write(const uint8_t b) {
    if (length() == BUFFER_SIZE-1)
      return 0;
    _buf[_write_pos++] = b;
    if (_write_pos >= BUFFER_SIZE)
      _write_pos = 0;
  }

  inline size_t write(const uint8_t *buf, size_t size) {
    if (size <= 0 || length() == BUFFER_SIZE-1)
      return 0;
    size_t written, ttl;
    const uint8_t *p = buf;
    ttl = 0;
    if (_write_pos >= _read_pos) {
      written = min(BUFFER_SIZE - _write_pos, size);
      if (written > 0) {
        memcpy(&_buf[_write_pos], p, written);
        size -= written;
        p += written;
        _write_pos += written;
        if (_write_pos >= BUFFER_SIZE)
          _write_pos = 0;
        if (size == 0)
          return written;
        ttl += written;
      }
    }
    written = min(_read_pos - _write_pos - 1, size);
    if (written > 0) {
      memcpy(&_buf[_write_pos], p, written);
      _write_pos += written;
      ttl += written;
    }
    return ttl;
  }
};

class WebsocketStream : public Stream
{
private:
  WebSocketServer _server;

  //configuration members
  IPAddress _local_ip;
  const char *_pass = NULL;  //WPA
  char *_ssid = NULL;
  static void (*_event_callback)(int is_connected);

  inline int connect_client()
  {
    byte count = _server.connectionCount();
    if( count == 0)
    {
      _server.listen();
      count = _server.connectionCount();
    }
    return count;
  }

  inline bool is_ready()
  {
    uint8_t status = WiFi.status();
    return !( status == WL_NO_SHIELD || status == WL_CONNECTED );
  }

public:
  WebsocketStream() : _server(WEBSOCKSTREAM_PREFIX, WEBSOCKSTREAM_PORT) {
    _server.registerConnectCallback(&onConnect);
    _server.registerDataCallback(&onData);
    _server.registerDisconnectCallback(&onDisconnect);
  }

  virtual ~WebsocketStream() {}

  static void registerEventCallback(void (*event_callback)(int is_connected)) {
    _event_callback = event_callback;
  }

  // allows another way to configure a static IP before begin is called
  inline void config(IPAddress local_ip)
  {
    _local_ip = local_ip;
    WiFi.config( local_ip );
  }

  // get DCHP IP
  inline IPAddress localIP()
  {
    return WiFi.localIP();
  }

  inline bool maintain()
  {
    if( connect_client() ) {
      WebSocket *ws = _server.getWebSocket(0);
      int len = _send_queue.length();
      if (len > 0) {
        ws->send((char *)_send_queue._buf, len);
        _send_queue.reset();
      }
      // check new conn, replace the old one
      _server.listen();
      if (_server.connectionCount() > 1) {
        ws->close();
      }

      return true;
    }

    stop();
    ensureNetworkConnect();
    return false;
  }

/******************************************************************************
 *           Connection functions with DHCP
 ******************************************************************************/

  inline int begin(char *ssid, char *pass)
  {
    if( !is_ready() ) return 0;

    _ssid = ssid;
    _pass = pass;

    ensureNetworkConnect();
    _server.begin();
    return 1;
  }

/******************************************************************************
 *             Stream implementations
 ******************************************************************************/

  inline int available()
  {
    if (connect_client()) {
      if (_recv_queue.length() == 0)
        _server.listen(false);
      return _recv_queue.length();
    } else {
      return 0;
    }
  }

  inline void flush()
  {
    _recv_queue.reset();
    _send_queue.reset();
  }

  inline int peek()
  {
    if (connect_client()) {
      if (_recv_queue.length() == 0)
        _server.listen(false);
      return _recv_queue.peek();
    } else {
      return 0;
    }
  }

  inline int read()
  {
    if (connect_client()) {
      if (_recv_queue.length() == 0)
        _server.listen(false);
      return _recv_queue.read();
    } else {
      return 0;
    }
  }

  inline void stop()
  {
    _recv_queue.reset();
    _send_queue.reset();
  }

  inline size_t write(uint8_t byte)
  {
    if( connect_client() ) {
      _send_queue.write(byte);
      int len = _send_queue.length();
      if (len >= 64) {
        WebSocket *ws = _server.getWebSocket(0);
        ws->send((char *)_send_queue._buf, len);
        _send_queue.reset();
      }
      return 1;
    } else {
      return 0;
    }
  }

private:
  static Queue _recv_queue;
  static Queue _send_queue;

  void ensureNetworkConnect() {
    int status;
    if( (status = WiFi.status()) != WL_CONNECTED )
    {
      while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WiFi SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
      }
    }
  }

  static void onConnect(WebSocket &socket) {
    Serial.println("onConnect called");
    if (_event_callback)
      _event_callback(1);
  }


  // You must have at least one function with the following signature.
  // It will be called by the server when a data frame is received.
  static void onData(WebSocket &socket, char* dataString, byte frameLength) {

#ifdef DEBUG
    Serial.print("Got data: ");
    Serial.write((unsigned char*)dataString, frameLength);
    Serial.println();
#endif

    // save in queue
    _recv_queue.write((uint8_t *)dataString, frameLength);
  }

  static void onDisconnect(WebSocket &socket) {
    Serial.println("onDisconnect called");
    if (_event_callback) {
      _event_callback(0);
    }
  }
};

Queue WebsocketStream::_recv_queue;
Queue WebsocketStream::_send_queue;
void (*WebsocketStream::_event_callback)(int is_connected);

#endif //WEBSOCK_STREAM_H
