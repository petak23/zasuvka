# Projekt esp8266-zasuvka

Zásuvka spínaná cez relé pomocou ESP8266 a MQTT pre IoT

## Špecifikácia 

pre verziu 1.0:

**mqtt client name** = "ESP8266-zasuvka"

### Pre zmenu stavu len ako vstup

**topic** = "obyvacka/zasuvka/nastav"   
**message** = "on|off|stav"   
 - "on" - zapnutie relé,
 - "off" - vypnutie relé,
 - "stav" - zistenie aktuálneho stavu relé,
 - čokoľvek iné je ignorované.

### Pre publikovanie aktuálneho stavu len ako výstup

**topic** = "obyvacka/zasuvka/stav"   
**message** = "on|off"

## Odkazy na použité knižnice:   
 - **PubSubClient**: [https://pubsubclient.knolleary.net/](https://pubsubclient.knolleary.net/)

## Použitý relé modul:
 - [Relé modul s WiFi ESP8266](https://techfun.sk/produkt/rele-modul-s-wifi-esp8266/)

## Nosná časť kódu pre spínanie relé:
[Michaela Merz](https://www.hackster.io/mischmerz)   

byte relON[] = {0xA0, 0x01, 0x01, 0xA2};  //Hex command to send to serial for open relay      
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; //Hex command to send to serial for close relay     
Serial.begin(9600);      
Serial.write(relON, sizeof(relON)); // turns the relay     

This works perfectly on a ESP8266 (Arduino environment).

## Odkazy a referencie:
 - [https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)
 - [https://www.hackster.io/makerrelay/esp8266-wifi-5v-1-channel-relay-delay-module-iot-smart-home-e8a437](https://www.hackster.io/makerrelay/esp8266-wifi-5v-1-channel-relay-delay-module-iot-smart-home-e8a437) - hlavne diskusia pod článkom
