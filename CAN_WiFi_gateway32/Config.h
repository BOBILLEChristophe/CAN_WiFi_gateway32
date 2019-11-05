/*
   Config.h

*/


#ifndef __config_H__
#define __config_H__


#define DEBUG                           // Ne pas oublier de commenter en production
//#define LCD                             // Dans le cas ou il y a un ecran LCD


// Selection des redirections souhaitees
#define SEND_TO_SERIAL              true           
#define SEND_TO_CAN                 true
#define SEND_TO_TCP                 true

#define RECEIVE_FROM_SERIAL         true           
#define RECEIVE_FROM_CAN            true
#define RECEIVE_FROM_TCP            true


// WIFI
#define WIFI_SSID           "xxxxxxxxxx"
#define WIFI_PSW            "xxxxxxxxxx"

#define MDNS_NAME            "cangw"

#define OTAName              "cangw"           
#define OTAPassword          "xxxxxxxxxx"


// SERIAL
#define dbg_output                Serial1
#define SERIAL_BAUDRATE           115200
#define DBG_RX_PIN_ID             GPIO_NUM_15                 // GPIO15
#define DBG_TX_PIN_ID             GPIO_NUM_2                  // GPIO2

// CAN
#define CAN_TX_PIN_ID             GPIO_NUM_5                  // GPI05
#define CAN_RX_PIN_ID             GPIO_NUM_4                  // GPI04
#define CAN_BAUDRATE              CAN_SPEED_500KBPS



#endif
