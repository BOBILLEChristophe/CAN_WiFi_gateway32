/*
   Can.h

*/

#ifndef _gatewayCan_H_
#define _gatewayCan_H_

#include <Arduino.h>
#include "Config.h"
#include <CAN_config.h>
#include <ESP32CAN.h>



class GW_Can : public ESP32CAN {

  public:
    GW_Can();                     // Constructor
    int begin();
    void handle();
    int msgRx();
    void msgTx(const char*);
    void parseMsg(const char*);
    const char *getInputMsg();

  private :

    uint16_t mBaudRate;
    uint8_t mTx_pin_id;
    uint8_t mRx_pin_id;
    
    CAN_frame_t rx_frame;
    CAN_frame_t tx_frame;

    char mInputMsg[255];

};

#endif
