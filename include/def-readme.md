# Popis nastavení v definitions.h
## Nastavenia Wifi
const char* host = "my-host-name";     
const char* ssid = "my-ssid";    
const char* password = "my-extra-stronf-password";    

## Nastavenia MQTT broker-a
const char* mqtt_server = "my-mqtt-server-ip-adress";    
const char* mqtt_client = "my-mqtt-defice-name"; // Potrebné nastaviť pre každé zariadenie unikátne    
const char* mqtt_broker = "my-mqtt-broker-name";    
const char* mqtt_password = "my-mqtt-broker-password";    
const char* main_topic_set = "obyvacka/zasuvka/nastav"; // Topic - pre zmenu stavu    
const char* main_topic_get = "obyvacka/zasuvka/stav";   // Topic - pre publikáciu aktuálneho stavu    