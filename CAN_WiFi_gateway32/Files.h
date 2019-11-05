/*
   Files.h

*/

#ifndef __Files_H__
#define __Files_H__


#include "FS.h"
#include "SPIFFS.h"
#include <Arduino.h>
#include "WebServer.h"
#include "Config.h"


struct Files {

  static ESP32WebServer *webServer;
  static void begin();
  static bool handleFileRead(ESP32WebServer *);
  static String getContentType(String);
  static void listDir(fs::FS &fs, const char *, uint8_t);
  static String formatBytes(size_t);

};

#endif  /* defined(__Files_H__) */
