/*
   Files.cpp

   Ce code sert principalement à servir les pages web (HTML, JavaScript, CSS...)

*/


#include "Files.h"

void Files::begin() {
  SPIFFS.begin();
  listDir(SPIFFS, "/", 0);
}

bool Files::handleFileRead(ESP32WebServer* webServer) { // send the right file to the client (if it exists)
  String path = webServer->uri();
  dbg_output.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                          // If there's a compressed version available
      path = pathWithGz;                                      // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = webServer->streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    dbg_output.println(String("\tSent file: ") + path);
    return true;
  }
  dbg_output.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}


void Files::listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  dbg_output.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    dbg_output.print("- failed to open directory\n");
    return;
  }
  if (!root.isDirectory()) {
    dbg_output.print(" - not a directory\n");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      dbg_output.print("  DIR : ");
      dbg_output.print(file.name());
      dbg_output.print("\n");
      if (levels)
        listDir(fs, file.name(), levels - 1);

    } else {
      dbg_output.print("  FILE: ");
      dbg_output.print(file.name());
      dbg_output.print("\t\tSIZE: ");
      dbg_output.print(formatBytes(file.size()));
      dbg_output.print("\n");
    }
    file = root.openNextFile();
  }
  dbg_output.print("\n\n");
}

/*__________________________________________________________HELPER_FUNCTIONS__________________________________________________________*/

String Files::formatBytes(size_t bytes) { // convert sizes in bytes to KB and MB
  if (bytes < 1024) {
    return String(bytes) + " B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + " KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + " MB";
  }
}

String Files::getContentType(String filename) { // determine the filetype of a given filename, based on the extension
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
