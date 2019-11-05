/*


    Passerelle Wifi/CAN avec un ESP32 et un module SN65HVD230 CAN Bus Transceiver      Passerelle Wifi/CAN avec un ESP32 et un module SN65HVD230 CAN Bus Transceiver https://www.ebay.fr/itm/1pcs-SN65HVD230-CAN-bus-transceiver

    http://forum.locoduino.org/index.php?topic=371.msg4520#msg4520

    Passerelle entre un reseau ETHERNET/WiFi, un bus CAN et un port serie.

    http://www.locoduino.org - Christophe BOBILLE sept 2018 - oct 2019
    christophe.bobille@gmail.com

    Exemple de message echange avec les satellites  : unsigned char mInputMsg[]  = "0x25 0 3 0x93 0x00  0x9F";
    ou avec identifiant long : unsigned char mInputMsg[]  = "0xFF00112 1 3 0x93 0x00  0x9F";

    Bibliotheques :
    - <ESP32CAN.h> @ https://github.com/nhatuan84/esp32-can-protocol-demo
*/

#define VERSION  "1.1"                              
#define PROJECT  "CAN_WiFi_gateway ESP32 (cwg32)"


#include <Arduino.h>
#include <HardwareSerial.h>
#include "Config.h"
#include "CanCtrl.h"
#include "Files.h"
#include "OTA.h"
#include "SerialCtrl.h"
#include "WebServer.h"
#include "WebSockets.h"
#include "Wifi.h"
#include <Wire.h>
#ifdef LCD
#include <LiquidCrystal_I2C.h>
#endif


GW_Can canCtrl;                     // Instance de GW_Can
GW_Serial serialCtrl(Serial);       // Instance de GW_Serial avec l'objet Serial
Wifi wifi;                          // Instance de Wifi
Files files;                        // Instance de File

GW_WebServer webServer(80);         // Instance de WebServer sur le port 80
GW_WebSockets webSocketsCtrl(81);   // Instance de WebSocket sur le port 81


#ifdef LCD
LiquidCrystal_I2C lcd(0x3F, 20, 4); // Instance de LiquidCrystal_I2C avec l'adresse 0x3F (20 chars and 4 line display)
#endif

/*__________________________________________________________SETUP__________________________________________________________*/

void setup() {

  // Port serie
  dbg_output.begin(SERIAL_BAUDRATE, SERIAL_8N1, DBG_RX_PIN_ID, DBG_TX_PIN_ID);
  dbg_output.setDebugOutput(true); // Debug + complet
  // Infos du projet
  dbg_output.print("\n\n\n");
  dbg_output.print("Project :      ");
  dbg_output.print(PROJECT);
  dbg_output.print("\nVersion :      ");
  dbg_output.print(VERSION);
  dbg_output.print("\nCompiled :     ");
  dbg_output.print(__DATE__);
  dbg_output.print(" ");
  dbg_output.print(__TIME__);
  dbg_output.print("\n\n");

  // Initialisations
#ifdef LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
#endif
  serialCtrl.begin(115200);
  wifi.begin();
  files.begin();
  webServer.start();
  webSocketsCtrl.begin();

  //Bus CAN
  int err = canCtrl.begin();
  switch (err) {
    case 0 :
      dbg_output.print("CAN ok !\n\n");
#ifdef LCD
      lcd.print("CAN ok !");
#endif
      break;
    case -1 :
      dbg_output.print("Probleme de connexion CAN !\n");
      dbg_output.print("Programme interrompu.");
#ifdef LCD
      lcd.print("Probleme de connexion CAN !\n");
      lcd.setCursor(0, 1);
      lcd.print("Programme interrompu.");
#endif
      return;
  }

#ifdef LCD
  // Retour de l'adresse IP sur LCD
  lcd.setCursor(0, 1);
  lcd.print("IP : ");
  lcd.print(WiFi.localIP());
  delay(100);
#endif

  dbg_output.print("End of setup.\n\n");

} // End setup




/*__________________________________________________________LOOP__________________________________________________________*/

void loop() {

  static int err = 1;

  webServer.handleClient();                    // Ecoute du port 80
  webSocketsCtrl.handleClient();               // Ecoute du port 81


  if (RECEIVE_FROM_SERIAL)
    // INCOMING messages sur le port Serial et envoi sur les bus CAN et wifi
    err = serialCtrl.msgRx();
  switch (err) {
    case 0 :  // Message sur port serie
#ifdef LCD
      lcd.setCursor(0, 2);
      lcd.print("Msg from Serial port");
#endif
      if (SEND_TO_CAN)
        canCtrl.msgTx(serialCtrl.getInputMsg());    // Envoi sur le bus CAN
      if (SEND_TO_TCP)
        webSocketsCtrl.broadcastTXT(serialCtrl.getInputMsg()); // Envoi sur le reseau wifi
      break;
  } // -> end RECEIVE_FROM_SERIAL


  if (RECEIVE_FROM_CAN) {
    //INCOMING messages sur le bus CAN et envoi sur le port Serial et wifi
    err = canCtrl.msgRx();
    switch (err) {
      case 0 :
#ifdef LCD
        lcd.setCursor(0, 2);
        lcd.print("Message from CAN bus");
#endif
        if (SEND_TO_SERIAL)
          serialCtrl.msgTx(canCtrl.getInputMsg()); // Envoi sur le port serie
        if (SEND_TO_TCP)
          webSocketsCtrl.broadcastTXT(canCtrl.getInputMsg()); // Envoi sur le reseau wifi
        break;
    }
  } // -> end RECEIVE_FROM_CAN

  if (RECEIVE_FROM_TCP) {
    // INCOMING messages en Wifi et envoi sur le port Serial et bus Can
    err = webSocketsCtrl.msgRx();
    switch (err) {
      case 0 :
#ifdef LCD
        lcd.setCursor(0, 2);
        lcd.print("Message from Wifi");
#endif
        if (SEND_TO_SERIAL)
          serialCtrl.msgTx(webSocketsCtrl.getInputMsg()); // Envoi sur le port serie
        if (SEND_TO_CAN)
          canCtrl.msgTx(webSocketsCtrl.getInputMsg());    // Envoi sur le bus CAN

        webSocketsCtrl.setState(1);
        err = 1;
        break;
    }
  } // -> end RECEIVE_FROM_TCP

  delay(10);

} // End loop
