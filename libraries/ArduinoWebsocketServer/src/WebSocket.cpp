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
#include "WebSocket.h"
#include "sha1.h"
#include "Base64.h"

#define DEBUG 1

#define MAX_FRAME_DATA_LENGTH 512

struct Frame {
    bool isMasked;
    bool isFinal;
    byte opcode;
    byte mask[4];
    int length;
    char *data;
} frame;

WebSocketServer::WebSocketServer(const char *urlPrefix, int inPort, byte maxConnections) :
    m_server(inPort),
    m_socket_urlPrefix(urlPrefix),
    m_maxConnections(maxConnections),
    m_connectionCount(0)
{
    m_connections = new WebSocket*[ m_maxConnections ];
    for( byte x=0; x < m_maxConnections; x++ )
        m_connections[x] = NULL;

    onConnect = NULL;
    onData = NULL;
    onDisconnect = NULL;
}

void WebSocketServer::registerConnectCallback(Callback *callback) {
    onConnect = callback;
}
void WebSocketServer::registerDataCallback(DataCallback *callback) {
    onData = callback;
}
void WebSocketServer::registerDisconnectCallback(Callback *callback) {
    onDisconnect = callback;
}

void WebSocketServer::begin() {
    m_server.begin();
}

byte WebSocketServer::connectionCount()
{
    return m_connectionCount;
}

void WebSocketServer::send( char *data, byte length )
{
    m_server.write((uint8_t) 0x81); // Txt frame opcode
    m_server.write((uint8_t) length); // Length of data
    m_server.write( (const uint8_t *)data, length );
}

void WebSocketServer::listen(bool acceptNewConn) {
    // First check existing connections:
    for( byte x=0; x < m_maxConnections; x++ )
    {
        if( !m_connections[x] )
            continue;

        WebSocket *s = m_connections[x];
        if( !s->isConnected() )
        {
            m_connectionCount--;
            delete s;
            m_connections[x] = NULL;
            continue;
        }
        s->listen();
    }

    if ( !acceptNewConn )
        return;

    WiFiClient cli = m_server.available(true);
    if( !cli )
        return;

#ifdef DEBUG
    Serial.println(F("New WiFiCLient"));
#endif
    // Find a slot:
    for( byte x=0; x < m_maxConnections; x++ )
    {
        if( m_connections[x] )
            continue;

        WebSocket *s = new WebSocket(this, cli);
        m_connections[x] = s;
        m_connectionCount++;
#ifdef DEBUG
        Serial.println(F("Websocket client connected."));
#endif
        return;
    }

    // No room!
#ifdef DEBUG
    Serial.println(F("Cannot accept new websocket client, maxConnections reached!"));
#endif
    cli.stop();
}

WebSocket *WebSocketServer::getWebSocket(byte idx)
{
    // First check existing connections:
    for( byte x=0; x < m_maxConnections; x++ )
    {
        if( !m_connections[x] )
            continue;

        WebSocket *s = m_connections[x];
        if( !s->isConnected() )
        {
            m_connectionCount--;
            delete s;
            m_connections[x] = NULL;
            continue;
        }
        if (idx == 0)
            return s;
        idx--;
    }
    return NULL;
}

WebSocket::WebSocket( WebSocketServer *server, WiFiClient cli ) :
    m_server(server),
    client(cli)
{
    if( doHandshake() )
    {
        state = CONNECTED;
        if( m_server->onConnect )
            m_server->onConnect(*this);

        return;
    }

    disconnectStream();
}

void WebSocket::listen()
{
    if ( !client.connected() ) {
        disconnectStream();
        return;
    }

    if( !client.available() )
        return;

    if( !getFrame() )
        // Got unhandled frame, disconnect
        disconnectStream();
}

bool WebSocket::isConnected() {
    return (state == CONNECTED);
}


void WebSocket::disconnectStream() {
#ifdef DEBUG
    Serial.println(F("Disconnecting"));
#endif
    state = DISCONNECTED;
    if( m_server->onDisconnect )
        m_server->onDisconnect(*this);

    client.write((uint8_t) 0x08);
    client.write((uint8_t) 0x02);
    client.write((uint8_t) 0x03);
    client.write((uint8_t) 0xf1);
    client.flush();
    delay(100);
    client.stop();
}

bool WebSocket::doHandshake() {
    char temp[128];
    char key[80];
    char bite;
    
    bool hasUpgrade = false;
    bool hasConnection = false;
    bool isSupportedVersion = false;
    bool hasHost = false;
    bool hasKey = false;

    byte counter = 0;

    // wait for data
    for (int i = 0; i < 3; i++) {
      delay(300);
      if (client.available())
        break;
    }
    while (client.available() && (bite = client.read()) != -1) {
        temp[counter++] = bite;

        if (counter > 2 && (bite == '\n' || counter >= 127)) { // EOL got, or too long header. temp should now contain a header string
            temp[counter - 2] = 0; // Terminate string before CRLF
            
            #ifdef DEBUG
                Serial.print("Got header: ");
                Serial.println(temp);
            #endif
            
            // Ignore case when comparing and allow 0-n whitespace after ':'. See the spec:
            // http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html
            if (!hasUpgrade && strstr_P(temp, PSTR("Upgrade: "))) {
                // OK, it's a websockets handshake for sure
                hasUpgrade = true;	
            } else if (!hasConnection && strstr_P(temp, PSTR("Connection: "))) {
                hasConnection = true;
            } else if (!hasHost && strstr_P(temp, PSTR("Host: "))) {
                hasHost = true;
            } else if (!hasKey && strstr_P(temp, PSTR("Sec-WebSocket-Key: "))) {
                hasKey = true;
                strtok(temp, " ");
                strcpy(key, strtok(NULL, " "));
            } else if (!isSupportedVersion && strstr_P(temp, PSTR("Sec-WebSocket-Version: ")) && strstr_P(temp, PSTR("13"))) {
                isSupportedVersion = true;
            }
            
            counter = 0; // Start saving new header string
        }
    }

    // Assert that we have all headers that are needed. If so, go ahead and
    // send response headers.
    if (hasUpgrade && hasConnection && isSupportedVersion && hasHost && hasKey) {
        strcat_P(key, PSTR("258EAFA5-E914-47DA-95CA-C5AB0DC85B11")); // Add the omni-valid GUID
        Sha1.init();
        Sha1.print(key);
        uint8_t *hash = Sha1.result();
        base64_encode(temp, (char*)hash, 20);
        #if 0
	char buf[132];
        snprintf_P( buf, sizeof(buf), PSTR("HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n"), temp );
	client.print( buf );
  #else
  client.print( F("HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ") );
  client.print( temp );
  client.print( F("\r\n\r\n") );
  #endif
    } else {
        // Nope, failed handshake. Disconnect
#ifdef DEBUG
        Serial.print(F("Handshake failed! Upgrade:"));
        Serial.print( hasUpgrade );
        Serial.print(F(", Connection:"));
        Serial.print( hasConnection );
        Serial.print(F(", Host:"));
        Serial.print( hasHost );
        Serial.print(F(", Key:"));
        Serial.print( hasKey );
        Serial.print(F(", Version:"));
        Serial.println( isSupportedVersion );
#endif
        return false;
    }
    
    return true;
}

bool WebSocket::getFrame() {
    byte bite;
    byte opcode;
    byte reserve;
    
    if (frame.data == NULL) {
      frame.data = new char[MAX_FRAME_DATA_LENGTH];
      if (frame.data == NULL) {
        Serial.println("WebSocket: Malloc buffer failed!");
        return false;
      }
    }
    // Get opcode
    bite = client.read();
        
    opcode = bite & 0xf; // Opcode
    reserve = (bite >> 4) & 0x7;
    frame.isFinal = bite & 0x80; // Final frame?
    bite = client.read();
    frame.isMasked = bite & 0x80;
    frame.length = bite & 0x7f; // Length of payload
    if (frame.length == 126) {
      // use 16bits length
      frame.length = ((int)client.read() << 8) | client.read();
    } else if (frame.length == 127) {
      // too large, not support here
      frame.length = MAX_FRAME_DATA_LENGTH + 1;
    }
    if (frame.length > MAX_FRAME_DATA_LENGTH) {
#ifdef DEBUG
        Serial.print(F("Too big frame to handle. Length: "));
        Serial.println(frame.length);
#endif
        client.write((uint8_t) 0x08);
        client.write((uint8_t) 0x02);
        client.write((uint8_t) 0x03);
        client.write((uint8_t) 0xf1);
        return false;
    }
    // Client should always send mask, but check just to be sure
    if (frame.isMasked) {
        frame.mask[0] = client.read();
        frame.mask[1] = client.read();
        frame.mask[2] = client.read();
        frame.mask[3] = client.read();
    }

    // Clear any frame data that may have come previously
    memset(frame.data, 0, sizeof(frame.data)/sizeof(char));

    
    // Get message bytes and unmask them if necessary
    for (int i = 0; i < frame.length; i++) {
        if (frame.isMasked) {
            frame.data[i] = client.read() ^ frame.mask[i % 4];
        } else {
            frame.data[i] = client.read();
        }
    }
    
    //
    // Frame complete!
    //
    // With fragment frames, the first FIN=0 will have none-0 opcode, subsequent
    // frames (last with FIN=1) will have opcode set to 0.

    if (opcode != 0)
      frame.opcode = opcode;

    switch (frame.opcode) {
        case 0x01: // Txt frame
        case 0x02: // Binary frame
            // Call the user provided function
            if( m_server->onData )
                m_server->onData(*this, frame.data, frame.length);
            break;
            
        case 0x08:
            // Close frame. Answer with close and terminate tcp connection
            // TODO: Receive all bytes the client might send before closing? No?
#ifdef DEBUG
            Serial.println(F("Close frame received. Closing in answer."));
#endif
            client.write((uint8_t) 0x08);
            return false;
            break;

        case 0x09: // Ping frame
          client.print((uint8_t) 0x8a);
          client.write((uint8_t) frame.length);
          for (int i = 0; i < frame.length; i++) {
            client.write((uint8_t)frame.data[i]);
          }
          break;

        default:
            // Unexpected. Ignore. Probably should blow up entire universe here, but who cares.
#ifdef DEBUG
            Serial.println(F("Unhandled frame ignored."));
            Serial.print(F("frame.opcode="));
            Serial.println(frame.opcode);
#endif
			return false;
            break;
    }
    return true;
}



bool WebSocket::send(char *data, byte length)
{
    if( state != CONNECTED )
    {
#ifdef DEBUG
        Serial.println(F("No connection to client, no data sent."));
#endif
        return false;
    }

    // client.write((uint8_t) 0x81); // Txt frame opcode
    client.write((uint8_t) 0x82); // Binary frame opcode
    client.write((uint8_t) length); // Length of data
    client.write( (const uint8_t *)data, length );
    return true;
}
