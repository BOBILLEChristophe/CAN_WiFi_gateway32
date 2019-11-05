# CAN_WiFi_gateway32

Le projet CAN/WiFi_gateway32 permet de réaliser une passerelle entre un bus CAN et un réseau en TCP (Ethernet et WiFi).

Elle autorise donc des applications qui communiquent en TCP à envoyer et à recevoir des commandes CAN et elle permet à des terminaux équipés en CAN de recevoir et d’envoyer des informations sur un réseau TCP.

En TCP, les applications devront maintenir un canal de communication ouvert en permanence. Le cas le plus courant est réalisé au travers de websockets. Cela peut aussi être réalisé par exemple dans une base de données en ouvrant un port TCP « propriétaire », en maintenant ce port ouvert et en réalisant une lecture du port à intervalles réguliers.

La messagerie CAN reconnaît les identifiants courts (11bits) et longs (29bits). Les messages peuvent comporter entre 1 et 8 octets de données.

Cette passerelle permet également d’envoyer des messages CAN à partir d’une application permettant de communiquer en série avec la passerelle et de lire des messages CAN sur un terminal série.

Typiquement, il s’agit d’application comme CoolTerm sur Mac mais il peut tout aussi bien s’agir de bases de données reliées par un port série à l’ESP32.

A noter que l’ESP32 dispose de 3 ports série. 

Le hardware est simple et très économique puisqu’il s’agit d’un ESP32 dans sa version prototypage (environ 4,50€) : https://www.ebay.fr/itm/Espressif-ESP-WROOM-32-ESP-32-ESP-32S-IoT-Wifi-Wlan-BLE-Module-mit-Adapter-Board/263158557355?ssPageName=STRK%3AMEBIDX%3AIT&var=562156698450&_trksid=p2057872.m2749.l2649


et d’un module CAN SN65HVD230 (1,50€) : https://www.ebay.fr/itm/1pcs-SN65HVD230-CAN-bus-transceiver-communication-module-for-Arduino/183514043696?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649


Le logiciel est écrit en C/C++ au travers de l’IDE Arduino. Il comporte un fichier principal : CAN_WiFi_gateway32.ino et différents fichiers complémentaires auxquels il n’est pas nécessaire d’apporter de modifications à l’exclusion du fichier Config.h où seront renseignés vos paramètres spécifiques, l’identifiant du réseau wifi et le mot de passe :


	#define WIFI_SSID           "xxxxxxxxxx"
	#define WIFI_PSW            "xxxxxxxxxx"
	

	#define MDNS_NAME            "cangw"
	

	#define OTAName              "cangw"           
	#define OTAPassword          "xxxxxxxxxx"


Une vidéo est disponible ici : https://youtu.be/MusBfFuk-hw

Le dossier data CAN_WiFi_gateway32 qui se trouve dans le dossier CAN_WiFi_gateway32 peut être copié dans la mémoire SPIFFS de l'ESP32. Il sera alors possible de charger la page web à l'url : http://cangw.local/tco/tco_v06.html

Une application test dans le dossier SEEED_CAN_send permet à partir d'un Arduino Uno ou Mega et d'un module CAN MCP2515 d'envoyer des messages à la passerelle qui va les rerouter vers la page html et ainsi permettre un affichage dynamique en temps réel.


















