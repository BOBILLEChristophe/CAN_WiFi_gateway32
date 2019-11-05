/*
   WiFi.h

*/

#ifndef __Wifi_H__
#define __Wifi_H__

#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "config.h"

struct Wifi {

  static void begin();

};

#endif /* defined(__Wifi_H__) */
