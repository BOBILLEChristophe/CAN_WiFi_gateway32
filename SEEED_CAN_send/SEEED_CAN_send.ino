// SEEED_CAN_send

/*
   Seeed-Studio : https://www.seeedstudio.com/CAN-BUS-Shield-V1.2-p-2256.html
   https://github.com/Seeed-Studio/CAN_BUS_Shield
*/
//
#include <SPI.h>
#include <mcp_can.h>


bool _debug = true;          // pour valider les "Serial.print" vers la Console

/***************** CAN **********************/

const uint8_t spiCS = 9;  // pour un Uno
//const uint8_t spiCS = 53; // sur un Mega
MCP_CAN canController(spiCS);             // Set CS to pin 9 for CAN
const int can0_int = 2;
volatile bool flag_Recv = false;      // pour IRQ CAN
volatile bool CANOK = false;

const int baudrate = CAN_500KBPS;       // can throughput 500 Kb/s

/***       Variables CAN        ***/
unsigned long IdR;
unsigned char lenR = 0;
unsigned char bufR[8];            // buffer reception


void setup() {
  Serial.begin(115200);

  /***************** CAN **********************/
  canController.start();

  int repeat = 10;                           // tries to open the CAN
  while (repeat > 0) {
    if (CAN_OK == canController.begin(baudrate)) {
      CANOK = true;
      break;
    }
    else {
      repeat--;
      Serial.print("Try to connect CAN\n");
    }
    delay(200);
  }
  if (CANOK)
    Serial.print(F("Can OK\n\n"));
  else {
    Serial.print(F("Can NOK!\n\n"));
    return;
  }
  attachInterrupt(digitalPinToInterrupt(can0_int), [] { flag_Recv = true; }, FALLING);
}

unsigned char stmp[8] = { 0 }; // send data

void loop() {
  canSendTest();
}

void canSendTest() {

  /* Simule l'envoi par chaque satellite de l'état de ses capteurs
    au rythme de 1 message toutes les 100 ms (freq programmee dans SAM) */

  int id;
  int idStart = 0;
  int idEnd = 8;
  byte msgLen = 1;
  byte msgType = 0;
  byte msg = 0;
  char etats[] = { 4, 5, 6, 0 }; // seuls etats possibles des capteurs : b000, b100, b101, b110

  for (int i = idStart; i < idEnd; i++) {
    id = 10 + i; // id + n° sat => 0x1 + num sat
    for (int elem : etats) {
      stmp[0] = elem;
      canController.sendMsgBuf(id, msgType, msgLen, stmp);
      Serial.println(stmp[0]);
      delay(1000);
    }
  }
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/




//  stmp[0] = B00010000;
//  stmp[1] = B00000000;
//  stmp[2] = B10000000;// Servo
//  canController.sendMsgBuf(0x26, 0, 3, stmp);
//  delay(1000);;
//
//  stmp[0] = B10001000;
//  stmp[1] = B00000000;
//  stmp[2] = B00000000;// Servo
//  canController.sendMsgBuf(0x26, 0, 3, stmp);
//  delay(1000);





//    // Carré : Le carré commande l'arrêt et ne doit en aucun cas être franchi.
//    stmp[0] = B10001000;// 1=ROUGE HAUT 2=VERT BAS 3=ROUGE BAS 4=JAUNE
//    stmp[1] = B00000000;// 1=oeil cligno 2=oeil FIXE
//    stmp[2] = B00000000;// Servo
//    canController.sendMsgBuf(0x26, 0, 8, stmp);
//    delay(10000);
//
//    //Sémaphore : Le sémaphore commande l'arrêt, et peut être franchi en marche à vue
//    /* Si le feu rouge du sémaphore est affiché sur un panneau pouvant présenter l'indication "carré"
//      avec plaque "Nf", un petit feu blanc (nommé « œilleton ») doit aussi être allumé
//      pour confirmer que ce qui est affiché est bien un sémaphore,
//      et non un carré dont l'un des feux rouges serait grillé. */
//
//    stmp[0] = B00001000;// 1=ROUGE HAUT 2=VERT BAS 3=ROUGE BAS 4=JAUNE
//    stmp[1] = B00000010;// 1=oeil cligno 2=oeil FIXE
//    stmp[2] = B00000000;
//    canController.sendMsgBuf(0x26, 0, 8, stmp);
//    delay(5000);
//
//    // Feu vert
//    /*Le feu vert indique au mécanicien que la voie est libre, et que la marche normale
//      est autorisée si rien ne s'y oppose.*/
//    stmp[0] = B00100000;// 1=ROUGE HAUT 2=VERT BAS 3=ROUGE BAS 4=JAUNE
//    stmp[1] = B00000000;// 1=oeil cligno 2=oeil FIXE
//    stmp[2] = B10000000;
//    canController.sendMsgBuf(0x26, 0, 8, stmp);
//    delay(5000);
//
//    // Le feu jaune - avertissement
//    /*Le feu jaune fixe commande au mécanicien de pouvoir s'arrêter avant le signal suivant :
//       carré, sémaphore, carré violet; ou d'être en mesure de respecter un rouge clignotant.
//    */
//    stmp[0] = B00000010;// 1=ROUGE HAUT 2=VERT BAS 3=ROUGE BAS 4=JAUNE
//    stmp[1] = B00000000;// 1=oeil cligno 2=oeil FIXE
//    stmp[2] = B00000000;
//    canController.sendMsgBuf(0x26, 0, 8, stmp);
//    delay(5000);
//
//
//    // Le feu vert clignotant
//    /* Le feu vert clignotant est équivalent au feu vert fixe pour les trains dont la vitesse ne dépasse pas 160 Km/h.
//      Pour les trains à vitesse plus élevée, ce signal commande au mécanicien de réduire sa vitesse à 160 Km/h le plus tôt possible,
//      afin d'être en mesure de franchir le signal suivant au plus à cette vitesse.*/
//    stmp[0] = B00010000;// 1=ROUGE HAUT 2=VERT BAS 3=ROUGE BAS 4=JAUNE
//    stmp[1] = B00000000;// 1=oeil cligno 2=oeil FIXE
//    stmp[2] = B10000000;
//    canController.sendMsgBuf(0x26, 0, 8, stmp);
//    delay(5000);
//
//    // Le feu jaune clignotant
//    /* Le feu jaune clignotant commande au mécanicien d'être en mesure de s'arrêter avant le signal annoncé
//      à distance réduite par l'avertissement suivant.
//      Ce signal est implanté lorsque la distance entre l'avertissement (signal jaune) et le signal d'arrêt (signal rouge)
//      est insuffisante pour permettre l'arrêt du train (canton court).
//    */
//    stmp[0] = B00000001;// 1=ROUGE HAUT 2=VERT BAS 3=ROUGE BAS 4=JAUNE
//    stmp[1] = B00000000;// 1=oeil cligno 2=oeil FIXE
//    stmp[2] = B00000000;
//    canController.sendMsgBuf(0x26, 0, 8, stmp);
//    delay(5000);
//
//    // Le feu rouge clignotant
//    /* Le feu rouge clignotant est un signal dont l'arrêt n'est pas obligatoire. Le mécanicien qui rencontre ce signal
//        peut le franchir en marche à vue, sans marquer d'arrêt, et sans dépasser la vitesse de 15 Km/h.
//    */
//    stmp[0] = B00000100;// 1=ROUGE HAUT 2=VERT BAS 3=ROUGE BAS 4=JAUNE
//    stmp[1] = B00000000;// 1=oeil cligno 2=oeil FIXE
//    stmp[2] = B10000000;
//    canController.sendMsgBuf(0x26, 0, 8, stmp);
//    delay(5000);

// Config satellite

//    stmp[0] = B00000000; // Temporaire servo
//    stmp[1] = B00000000;
//    stmp[2] = B00000000;
//    canController.sendMsgBuf(0x1FFFFF26, 1, 3, stmp);
//    delay(5000);
