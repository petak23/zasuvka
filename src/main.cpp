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
#include "definitions.h"

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  String messageTemp;
  for (unsigned int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  /*if(topic.startsWith(main_topic)){
    int topic_length = topic.length() - 1;
    String lamp_id = topic.substring(topic_length);
    int id_lamp = lamp_id.toInt();
    byte state = messageTemp == "on" ? 1 : 0;
    
    Serial.print(" | state:"); Serial.print(state);
    Serial.print(" | lamps[");Serial.print(id_lamp);Serial.print("]:"); 
    Serial.print(bitRead(lamps, id_lamp));
    
    if (bitRead(lamps, id_lamp) != state) {
      Serial.print(" | Menim stav lamps[");Serial.print(id_lamp);Serial.print("]:"); 
      digitalWrite(pins[id_lamp], (1 - state));
      bitWrite(lamps, id_lamp, state);
      Serial.print(bitRead(lamps, id_lamp));
    }
  }*/
  Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    if (client.connect(mqtt_client, mqtt_broker, mqtt_password)) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe(main_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  
  Serial.begin(115200);
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