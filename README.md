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