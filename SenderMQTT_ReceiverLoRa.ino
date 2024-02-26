#include <MQTT.h>
#include <MQTTClient.h>
#include <WiFi.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI0 26
#define freq 868E6
#define sf 8
#define bw 125E3

const char ssid[] = "raspi-webgui";
const char pass[] = "RASPI_TP";
const char mqttServer[] = "10.3.141.205";
const int mqttPort = 1883;
const char mqttTopic[] = "srt5/SUPERMAX";
WiFiClient net;
MQTTClient client;

void setup() {
  Serial.begin(9600);
  Serial.println("Récepteur LoRa Initialisé");
  pinMode(DI0, INPUT);
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  LoRa.setSpreadingFactor(sf);
  if (!LoRa.begin(freq)) {
    Serial.println("Le démarrage de LoRa a échoué !");
    while (1);
  }
  Serial.println("LoRa en écoute...");
  connectToWiFi();
  connectToMqtt();
}

unsigned long lastSendTime = 0; 
const long sendInterval = 5000; 
void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Message reçu de: '");
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    Serial.print("' avec RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime > sendInterval) {
    publishMessage();
    lastSendTime = currentTime; 
  }
}

void connectToWiFi() {
  Serial.print("Connexion WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecté");
}

void connectToMqtt() {
  client.begin(mqttServer, mqttPort, net);
  Serial.print("Connexion MQTT...");
  while (!client.connect("Maxime")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nMQTT connecté");
}

void publishMessage() {
  StaticJsonDocument<200> doc;
  doc["freq"] = freq;
  doc["sf"] = sf;
  doc["bw"] = bw;
  String message;
  serializeJson(doc, message);
  client.publish(mqttTopic, message.c_str());
  Serial.println("Message publié: " + message);
}

