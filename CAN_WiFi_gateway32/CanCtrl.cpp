

#include "CanCtrl.h"

CAN_device_t CAN_cfg;

GW_Can::GW_Can() {} // Constructor

int GW_Can::begin() {

  CAN_cfg.speed = CAN_BAUDRATE;
  CAN_cfg.tx_pin_id = CAN_TX_PIN_ID;
  CAN_cfg.rx_pin_id = CAN_RX_PIN_ID;
  CAN_cfg.rx_queue = xQueueCreate(10, sizeof(CAN_frame_t));

  return GW_Can::CANInit();
}

/******************************* parseMsg *******************************/

void GW_Can::parseMsg(const char* msg) {
  char *ptEnd;
  tx_frame.MsgID = strtol (msg, &ptEnd, 16);  // ID du message
  if (1 == strtol (ptEnd, &ptEnd, 10))
    tx_frame.FIR.B.FF = CAN_frame_ext;        // Trame normale ou etendue
  else
    tx_frame.FIR.B.FF = CAN_frame_std;
  tx_frame.FIR.B.DLC = strtol (ptEnd, &ptEnd, 10); // Nombre d'octets du message
  for ( int i = 0; i < tx_frame.FIR.B.DLC; i++)
    tx_frame.data.u8[i] = strtol (ptEnd, &ptEnd, 16); // Contenu du message
}


/******************************** msgTx ********************************/

void GW_Can::msgTx(const char* msg) {

  parseMsg(msg); // Parsing du message

#ifdef DEBUG
  dbg_output.print("\nEnvoi d'un message sur le bus CAN\n");
  dbg_output.printf("Id         : 0x%X\n", tx_frame.MsgID);
  dbg_output.printf("Type       : %d\n", tx_frame.FIR.B.FF);
  dbg_output.printf("Length     : %d\n", tx_frame.FIR.B.DLC);
  for (byte i = 0; i < tx_frame.FIR.B.DLC; i++)
    dbg_output.printf("tx_frame[%d] : %d\n", i, tx_frame.data.u8[i]);
#endif

  GW_Can::CANWriteFrame(&tx_frame);
}

/******************************** msgRx ********************************/

int GW_Can::msgRx() {

  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {
    switch (rx_frame.FIR.B.DLC) {
      case 0 :
        sprintf(mInputMsg, "%x %d %d", rx_frame.MsgID, rx_frame.FIR.B.FF, rx_frame.FIR.B.DLC);
        break;
      case 1 :
        sprintf(mInputMsg, "%x %d %d %x", rx_frame.MsgID, rx_frame.FIR.B.FF, rx_frame.FIR.B.DLC, rx_frame.data.u8[0]);
        break;
      case 2 :
        sprintf(mInputMsg, "%x %d %d %x %x", rx_frame.MsgID, rx_frame.FIR.B.FF, rx_frame.FIR.B.DLC, rx_frame.data.u8[0], rx_frame.data.u8[1]);
        break;
      case 3 :
        sprintf(mInputMsg, "%x %d %d %x %x %x", rx_frame.MsgID, rx_frame.FIR.B.FF, rx_frame.FIR.B.DLC, rx_frame.data.u8[0], rx_frame.data.u8[1], rx_frame.data.u8[2]);
        break;
      case 4 :
        sprintf(mInputMsg, "%x %d %d %x %x %x %x", rx_frame.MsgID, rx_frame.FIR.B.FF, rx_frame.FIR.B.DLC, rx_frame.data.u8[0], rx_frame.data.u8[1], rx_frame.data.u8[2], rx_frame.data.u8[3]);
        break;
      case 5 :
        sprintf(mInputMsg, "%x %d %d %x %x %x %x %x", rx_frame.MsgID, rx_frame.FIR.B.FF, rx_frame.FIR.B.DLC, rx_frame.data.u8[0], rx_frame.data.u8[1], rx_frame.data.u8[2], rx_frame.data.u8[3], rx_frame.data.u8[4]);
        break;
      case 6 :
        sprintf(mInputMsg, "%x %d %d %x %x %x %x %x %x", rx_frame.MsgID, rx_frame.FIR.B.FF, rx_frame.FIR.B.DLC, rx_frame.data.u8[0], rx_frame.data.u8[1], rx_frame.data.u8[2], rx_frame.data.u8[3], rx_frame.data.u8[4], rx_frame.data.u8[5]);
        break;
      case 7 :
        sprintf(mInputMsg, "%x %d %d %x %x %x %x %x %x %x", rx_frame.MsgID, rx_frame.FIR.B.FF, rx_frame.FIR.B.DLC, rx_frame.data.u8[0], rx_frame.data.u8[1], rx_frame.data.u8[2], rx_frame.data.u8[3], rx_frame.data.u8[4], rx_frame.data.u8[5], rx_frame.data.u8[6]);
        break;
      case 8 :
        sprintf(mInputMsg, "%x %d %d %x %x %x %x %x %x %x %x", rx_frame.MsgID, rx_frame.FIR.B.FF, rx_frame.FIR.B.DLC, rx_frame.data.u8[0], rx_frame.data.u8[1], rx_frame.data.u8[2], rx_frame.data.u8[3], rx_frame.data.u8[4], rx_frame.data.u8[5], rx_frame.data.u8[6], rx_frame.data.u8[7]);
        break;
    }

#ifdef DEBUG
    dbg_output.print("\Reception d'un message sur le bus CAN\n");
    dbg_output.printf("Id         : 0x%X\n", rx_frame.MsgID);
    dbg_output.printf("Type       : %d\n", rx_frame.FIR.B.FF);
    dbg_output.printf("Length     : %d\n", rx_frame.FIR.B.DLC);
    for (byte i = 0; i < rx_frame.FIR.B.DLC; i++)
      dbg_output.printf("tx_frame[%d] : 0x%X\n", i, rx_frame.data.u8[i]);
#endif
    return 0;
  }
  else
    return 1;
}


/******************************** getInputMsg ********************************/

const char* GW_Can::getInputMsg() {
  return mInputMsg;
}
