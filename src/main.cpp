/* 
 * Program pre zásuvku spínanú cez relé pomocou ESP8266 a MQTT pre IoT
 *
 * Posledna zmena(last change): 02.12.2020
 * @author Ing. Peter VOJTECH ml. <petak23@gmail.com>
 * @copyright  Copyright (c) 2016 - 2020 Ing. Peter VOJTECH ml.
 * @license
 * @link       http://petak23.echo-msz.eu
 * @version 1.0.3
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
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "definitions.h"

/* Initializácia espClient. 
 * Každé zariadenie pripojené k mqtt brokeru musí mať špecifické meno.
 * Meno je uložené v súbore definitions.h */ 
WiFiClient espClient;
PubSubClient client(espClient);


/* Hex príkazy posielané po sériovej linke */
byte relON[] = {0xA0, 0x01, 0x01, 0xA2};  // Zopnutie relé
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; // Rozopnutie relé

byte stav = 0;       // Aktuálny stav relé

/* Pripojenie ESP8266 k routeru */
void setup_wifi() {
  delay(10);
  
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

/* Táto funkcia sa spustí ak nejaké zariadenie publikovalo správu s "topic", pre ktoré je moje ESP8266
 * prihlásené na odber */
void callback(String topic, byte* message, unsigned int length) {
  String messageTemp;
  for (unsigned int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  if(topic.startsWith(main_topic_set)){  // Ak mám topic na nastavovanie
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
      client.publish(main_topic_get, tmp);  
    }
  }
}

/* Táto funkcia opätovne pripojí ESP8266 k MQTT broker-u
 * a prihlási sa na odber správ(subscribe) pre ESP8266 */
void reconnect() {
  
  while (!client.connected()) {
    if (client.connect(mqtt_client, mqtt_broker, mqtt_password)) { //Pripojenie
      client.subscribe(main_topic_set);
    } else {
      delay(5000);       // Čakaj 5 s pred opätovným pokusom o pripojenie
    }
  }
}

void setup() {
  Serial.begin(9600); // Nastav rýchlosť sériovej komunikácie na 9600 lebo na doske relé
                      // je ďaľší procesor STC15F104W, ktorý pracuje len s touto rýchlosťou.
  Serial.write(relOFF, sizeof(relOFF)); // Vypni relé

  setup_wifi();       // pripoj wifi
  client.setServer(mqtt_server, 1883); // nastav mqtt server
  client.setCallback(callback);         
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop()) {
    client.connect(mqtt_client, mqtt_broker, mqtt_password);
  }
}