/* 
 * Program pre zásuvku spínanú cez relé pomocou ESP8266 a MQTT pre IoT
 *
 * Posledna zmena(last change): 31.12.2021
 * @author Ing. Peter VOJTECH ml. <petak23@gmail.com>
 * @copyright  Copyright (c) 2016 - 2021 Ing. Peter VOJTECH ml.
 * @license
 * @link       http://petak23.echo-msz.eu
 * @version 1.0.5
 * 
 * @help https://www.hackster.io/makerrelay/esp8266-wifi-5v-1-channel-relay-delay-module-iot-smart-home-e8a437 - v diskusii:
 * Michaela Merz
 * byte relON[] = {0xA0, 0x01, 0x01, 0xA2}; //Hex command to send to serial for open relay
 * byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; //Hex command to send to serial for close relay
 * Serial.begin(9600);
 * Serial.write(relON, sizeof(relON)); // turns the relay
 *
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include "definitions.h"

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;
int mqtt_state = 0;          // Stav MQTT pripojenia podľa https://pubsubclient.knolleary.net/api#state

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

/* Hex príkazy posielané po sériovej linke */
byte relON[] = {0xA0, 0x01, 0x01, 0xA2};  // Zopnutie relé
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; // Rozopnutie relé

byte stav = 0;       // Aktuálny stav relé

void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  mqttClient.connect();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void onMqttConnect(bool sessionPresent) {
  mqtt_state = 1;                   // Nastav príznak MQTT spojenia
  // Prihlásenie sa na odber:
  mqttClient.subscribe(main_topic_set, 1);

}


void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  mqtt_state = 0;                   // Nastav príznak chýbajúceho MQTT spojenia
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

/* Táto funkcia sa spustí ak nejaké zariadenie publikovalo správu s "topic", pre ktoré je moje ESP8266
 * prihlásené na odber */
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  String messageTemp;
  for (unsigned int i = 0; i < len; i++) {
    messageTemp += (char)payload[i];
  }
  String topicTmp;
  for (int i = 0; i < int(strlen(topic)); i++) {
    topicTmp += (char)topic[i];
  }

  if(topicTmp.startsWith(main_topic_set)){  // Ak mám topic na nastavovanie
    String pub_msg = "";

    if (messageTemp == "stav") { // Priprav správu aktuálneho stavu na publikovanie 
      pub_msg = (stav == 1) ? "on" : "off";
    } else {                     // Testovanie požiadavky na zmenu stavu 
      byte new_state = 128;
      if (messageTemp == "on") {
        new_state = 1;
        Serial.write(relON, sizeof(relON)); // turns the relay
      } else if (messageTemp == "off") {
        new_state = 0;
        Serial.write(relOFF, sizeof(relOFF)); // turns the relay
      }
      if (new_state < 2) { // Iba ak je správne nastavený nový stav
        stav = new_state;
        pub_msg = (stav == 1) ? "on" : "off"; // Priprav správu zmeny stavu na publikovanie
      }
    }
    int l = pub_msg.length() + 1; // Zisti dĺžku správy pre publikovanie
    if (l > 1) {                  // Ak je čo tak publikuj
      char tmp[l];
      pub_msg.toCharArray(tmp, l);
      mqttClient.publish(main_topic_get, 0, true, tmp);  
    }
  }
}

void setup() {
  Serial.begin(9600); // Nastav rýchlosť sériovej komunikácie na 9600 lebo na doske relé
                      // je ďaľší procesor STC15F104W, ktorý pracuje len s touto rýchlosťou.
  Serial.write(relOFF, sizeof(relOFF)); // Vypni relé

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCredentials(MQTT_USER, MQTT_PASSWORD); 

  connectToWifi();    
}

void loop() {
  
}