/* 
 * Program pre zásuvku spínanú cez relé pomocou ESP8266 a MQTT pre IoT
 *
 * Posledna zmena(last change): 01.12.2020
 * @author Ing. Peter VOJTECH ml. <petak23@gmail.com>
 * @copyright  Copyright (c) 2016 - 2020 Ing. Peter VOJTECH ml.
 * @license
 * @link       http://petak23.echo-msz.eu
 * @version 1.0.1
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include "definitions.h"

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

#ifndef D5
#if defined(ESP8266)
#define D5 (14)
#define D6 (12)
#define D7 (13)
#define D8 (15)
#define TX (1)
#elif defined(ESP32)
#define D5 (18)
#define D6 (19)
#define D7 (23)
#define D8 (5)
#define TX (1)
#endif
#endif

#ifdef ESP32
#define BAUD_RATE 57600
#else
#define BAUD_RATE 9600
#endif

SoftwareSerial swSer;

byte stav = 0;       // Aktuálny stav relé

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  /*Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);*/
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  /*Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());*/
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
        swSer.println("A00101A2");
      } else if (messageTemp == "off") {
        new_state = 0;
        swSer.println("A00100A1");
      }
      if (new_state < 2) { // Iba ak je správne nastavený nový stav
        //Serial.print(" | nový stav:"); Serial.print(new_state);
        stav = new_state;
        //digitalWrite(rele, (1 - stav));       // Zmeň stav relé
        pub_msg = (stav == 1) ? "on" : "off"; // Priprav správu zmeny stavu na publikovanie
      }
    }
    int l = pub_msg.length() + 1; // Zisti dĺžku správy pre publikovanie
    if (l > 1) {                  // Ak je čo tak publikuj
      char tmp[l];
      pub_msg.toCharArray(tmp, l);
      client.publish(main_topic_get, tmp);  
      /*Serial.print(" | publikujem:"); Serial.print(main_topic_get); 
      Serial.print(" msg:"); Serial.print(tmp); */
    }
  }
  //Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    if (client.connect(mqtt_client, mqtt_broker, mqtt_password)) {
      //Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe(main_topic_set);
    } else {
      /*Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");*/
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  /*pinMode(rele, OUTPUT);
  digitalWrite(rele, stav);*/

  Serial.begin(115200);
  //Serial.begin(9600);
  swSer.begin(BAUD_RATE, SWSERIAL_8N1, D5, D6, false, 95, 11);
  swSer.println("A00100A1");

  setup_wifi();
  client.setServer(mqtt_server, 1883);
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