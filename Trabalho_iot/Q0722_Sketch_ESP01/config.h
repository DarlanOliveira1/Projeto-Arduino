#include "AdafruitIO_WiFi.h"

// CONFIGURAÇÃO DA ADAFRUITIO
#define IO_USERNAME "darlan_cartaxo"
#define IO_KEY "aio_Uwto87SAsN9OF823t5QSBAZTMAqx"

//Rede casa
//#define WIFI_SSID "TNET2.4G"
//#define WIFI_PASS "Flornina"

//rede pc hp
#define WIFI_SSID "hp_2135"
#define WIFI_PASS "Darlan@10"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
