/*
   OTA.h

*/

#ifndef __OTA_H__
#define __OTA_H__


#include <Arduino.h>
#include <ArduinoOTA.h>
#include "Config.h"


struct OTA {
  static void begin();
};


void OTA::begin() { // Start the OTA service
  ArduinoOTA.setHostname(OTAName);
  ArduinoOTA.setPassword(OTAPassword);

  ArduinoOTA.onStart([]() {
    dbg_output.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    dbg_output.println("\r\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    dbg_output.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    dbg_output.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) dbg_output.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) dbg_output.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) dbg_output.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) dbg_output.println("Receive Failed");
    else if (error == OTA_END_ERROR) dbg_output.println("End Failed");
  });
  ArduinoOTA.begin();
  dbg_output.println("OTA ready");
}

#endif /* defined(__OTA_H__) */
