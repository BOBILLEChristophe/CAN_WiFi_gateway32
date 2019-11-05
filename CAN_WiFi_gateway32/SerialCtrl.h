/*
   Serial.h

*/

#ifndef __serial_H__
#define __serial_H__

#include <Arduino.h>
#include "Config.h"

/****************************************************************/
/*                            SERIAL                            */
/****************************************************************/


class GW_Serial {

  private:
    uint64_t mBaudRate;
    char mInputMsg[100];
    HardwareSerial &serialControl;
    
  public:
    GW_Serial(HardwareSerial &serial);   
    void begin(uint64_t);
    void print(const char*);
    int msgRx();
    void msgTx(const char*);
    const char* getInputMsg();

};


#endif
