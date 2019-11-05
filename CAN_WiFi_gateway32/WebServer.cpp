/*
   WebSockets.cpp

   Ce code permet de servir les pages web stockees dans la memoire SPIFFS de la carte 

*/


#include "WebServer.h"

/****************************** Constructeur ******************************/

GW_WebServer::GW_WebServer(uint16_t port) {
  mPort = port;
}

/******************************** begin ********************************/

void GW_WebServer::start() { // Start WebSocket server

  ESP32WebServer GW_WebServer(mPort);

  GW_WebServer::on("/" , [&]() {
    GW_WebServer::send(200, "text/plain", "this works as well");
  });

  GW_WebServer::onNotFound([&]() {
    !Files::handleFileRead(this);
  });

  GW_WebServer::begin();                             // start the HTTP server


#ifdef DEBUG
  dbg_output.printf("WebServer started on port %d.\n", mPort);
#endif
}
