/** 
 * Vzorový súbor a popis nastavení v súbore definitions.h 
 * Pre reálnu aplikáciu je potrebné premenovať na: deffinitions.h
 * 
 * Posledná zmena(Last change): 31.12.2021
 * 
 * @author Ing. Peter VOJTECH ml. <petak23@gmail.com>
 * @copyright  Copyright (c) 2016 - 2021 Ing. Peter VOJTECH ml.
 * @license
 * @link       http://petak23.echo-msz.eu
 * @version 1.0.0
 */

/** Nastavenia Wifi */
#define WIFI_SSID "my-ssid-name"    
#define WIFI_PASSWORD "my-extra-strong-password"     

/** ------ Nastavenia MQTT ------- */
// Nastavenia broker-a
#define MQTT_ENBLED true    // Celkové povolenie MQTT
#define MQTT_HOST IPAddress(xxx, xxx, xxx, xxx)
#define MQTT_PORT 1883
#define MQTT_USER "my-mqtt-broker-name"
#define MQTT_PASSWORD "my-mqtt-broker-password"

// Nastavenie topic
const char* main_topic_set = "obyvacka/zasuvka/nastav"; // Pre zmenu stavu
const char* main_topic_get = "obyvacka/zasuvka/stav";   // Pre publikáciu aktuálneho stavu
/* ------- Nastavenia MQTT - Koniec ------- */

/** Prihlasovanie do OTA(AsyncElegantOTA) pre update firmware */
#define OTA_USER "ota-username";
#define OTA_PASSWORD "ota-password";

/** Ostaté Nastavenia */
#define SERIAL_PORT_ENABLED true  // Povolenie výstupu na sériový port - logovanie