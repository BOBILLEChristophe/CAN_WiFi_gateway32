/*
   WebSockets.cpp

   Ce code sert à creer gerer les requetes envoyes en Websockets

*/


#include "WebSockets.h"

/****************************** Constructeur ******************************/

GW_WebSockets::GW_WebSockets(uint16_t port) {
  mPort = port;
  mInputMsg = NULL;
  ptWss = NULL;
  mState = 1;
}

/******************************** begin ********************************/

void GW_WebSockets::begin() { // Start WebSocket server
  ptWss = new WebSocketsServer(mPort);
  ptWss->begin();
  ptWss->onEvent([&](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
      case WStype_DISCONNECTED:
        delete ptWss;
#ifdef DEBUG
        dbg_output.printf("Wifi [%u] Disconnected!\n", num);
#endif
        break;
      case WStype_CONNECTED: {
          IPAddress ip = ptWss->remoteIP(num);
#ifdef DEBUG
          dbg_output.printf("Wifi [%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
#endif
          ptWss->sendTXT(num, "Connected");
        }
        break;
      case WStype_TEXT:
#ifdef DEBUG
        dbg_output.printf("Wifi [%u] get Text: %s\n", num, payload);
#endif
        if ('0' == payload[0]) {
          mInputMsg = (char*)payload;
          mState = 0;
        }
        break;
    }
  });
#ifdef DEBUG
  dbg_output.printf("WebSocket started on port %d.\n", mPort);
#endif
}

/******************************** loop ********************************/

void GW_WebSockets::handleClient() {
  ptWss->loop();
}

/******************************** getInputMsg ********************************/

const char* GW_WebSockets::getInputMsg() {
  return mInputMsg;
}

/******************************** msgRx ********************************/

int GW_WebSockets::msgRx() {
  return mState;
}

/******************************** broadcastTXT ********************************/

bool GW_WebSockets::broadcastTXT(const char * text) {
  ptWss->broadcastTXT(text);
}

void GW_WebSockets::setState(bool state) {
  mState = state;
}
