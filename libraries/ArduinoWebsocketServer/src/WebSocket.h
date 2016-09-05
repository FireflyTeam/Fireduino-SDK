 /**
 * @file WebSocket.c
 * @brief Definitions WebSocket class for Fireduino
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


#include <Arduino.h> // Arduino 1.0 or greater is required
#include <stdlib.h>

#include <WiFi.h>

#ifndef WEBSOCKET_H_
#define WEBSOCKET_H_

// CRLF characters to terminate lines/handshakes in headers.
#define CRLF "\r\n"

class WebSocket;
class WebSocketServer {
public:
    // Constructor.
    WebSocketServer(const char *urlPrefix = "/", int inPort = 80, byte maxConnections = 4);
    
    // Callback functions definition.
    typedef void DataCallback(WebSocket &socket, char *socketString, byte frameLength);
    typedef void Callback(WebSocket &socket);
    
    // Callbacks
    void registerDataCallback(DataCallback *callback);
    void registerConnectCallback(Callback *callback);
    void registerDisconnectCallback(Callback *callback);
    
    // Start tlistening for connections.
    void begin();
    
    // Main listener for incoming data. Should be called from the loop.
    void listen(bool acceptNewConn=true);

    // Connection count
    byte connectionCount();

    // Get the Nth websocket, NULL if out of bound or not available.
    WebSocket *getWebSocket(byte idx);

    // Broadcast to all connected clients.
    void send(char *str, byte length);

private:
    WiFiServer m_server;
    const char *m_socket_urlPrefix;
    int m_maxConnections;

    byte m_connectionCount;

    // Pointer array of clients:
    WebSocket **m_connections;

protected:
friend class WebSocket;
    // Pointer to the callback function the user should provide
    DataCallback *onData;
    Callback *onConnect;
    Callback *onDisconnect;
};

class WebSocket {
    WebSocketServer *m_server;

public:
    // Constructor.
    WebSocket(WebSocketServer *server, WiFiClient cli);

    // Are we connected?
    bool isConnected();
	
    // Embeds data in frame and sends to client.
    bool send(char *str, byte length);

    // Handle incoming data.
    void listen();

    // Close websocket.
    void close() { disconnectStream(); }

private:
    WiFiClient client;
    enum State {DISCONNECTED, CONNECTED} state;

    // Discovers if the client's header is requesting an upgrade to a
    // websocket connection.
    bool doHandshake();
    
    // Reads a frame from client. Returns false if user disconnects, 
    // or unhandled frame is received. Server must then disconnect, or an error occurs.
    bool getFrame();

    // Disconnect user gracefully.
    void disconnectStream();
};

#endif
