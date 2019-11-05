/*
   WiFi.cpp

   Ce code sert à creer la connexion avec la boxe et donner une adresse DNS à la carte sur le reseau

*/

#include "Wifi.h"

void Wifi::begin() {
  WiFi.begin (WIFI_SSID, WIFI_PSW);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay (500);
    dbg_output.print (".");
  }

#ifdef DEBUG
  dbg_output.print("\n");
  dbg_output.print("\nConnected to : "); dbg_output.print(WIFI_SSID);
  dbg_output.print("\nIP address :   "); dbg_output.print(WiFi.localIP());
  dbg_output.print("\n\n");
#endif

  if (!MDNS.begin(MDNS_NAME)) {
    dbg_output.print("Error setting up MDNS responder!\n");
    while (1)
      delay(1000);
  }

#ifdef DEBUG
  dbg_output.print("MDNS responder started @ http://");
  dbg_output.print(MDNS_NAME);
  dbg_output.print(".local");
  dbg_output.print("\n\n");
#endif
}
