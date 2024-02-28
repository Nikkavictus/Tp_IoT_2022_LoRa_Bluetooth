#include <MQTT.h>
#include <MQTTClient.h>
#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>
// Définition des pins pour le module LoRa
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI0 26
extern long oldfrq = 0;
extern int oldsf=0;
extern long oldbw=0;
#define freq 868E6
// Informations de connexion WiFi
const char ssid[] = "raspi-webgui";
const char pass[] = "RASPI_TP";

// Informations du serveur MQTT
const char mqttServer[] = "10.3.141.205";
const int mqttPort = 1883;
const char mqttTopic[] = "srt5/SUPERMAX"; // Topic mis à jour

WiFiClient net;
MQTTClient client;

// Fonction appelé au démarrage du microcontrôleur 
void setup() {
  Serial.begin(9600); //Initialisation de la Communication Série débit de 9600
  // Initalisation LoRa
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(freq)) {
    Serial.println("Le démarrage de LoRa a échoué !");
    while (1);
  }
  Serial.println("LoRa Émetteur initialisé");
  connectToWiFi();
  connectToMqtt();
}
// Fonction appelé en boucle permettant d'envoyer un message LoRa tous les 5s
void loop() {
  client.loop();
  delay(5000); 
  Serial.print("Envoi d'un message Lora... ");
  LoRa.beginPacket();
  LoRa.print("Bonjour LoRa !");
  LoRa.endPacket();
  Serial.println("terminé");
}
// FOnction de connexion au Wifi 
void connectToWiFi() {
  Serial.print("Connexion WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecté");
}
//Fonction d'abonnement au topic MQTT
void connectToMqtt() {
  client.begin(mqttServer, mqttPort, net);
  client.onMessage(messageReceived);
  Serial.print("Connexion MQTT...");
  while (!client.connect("Sylvain")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nMQTT connecté");
  client.subscribe(mqttTopic); // S'abonner au topic mis à jour
}

// Fonction de reception d'un message MQTT
void messageReceived(String &topic, String &payload) {
  Serial.print("Message MQTT reçu : ");
  Serial.println(payload);
  // Cette fonction sert à analyser (parser) une chaîne de caractères formatée en JSON
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print(F("deserializeJson() a échoué : "));
    Serial.println(error.f_str());
    return;
  }
  // On affecte les paramètres recu dans les variables
  long frq = doc["freq"]; 
  int sf = doc["sf"];
  long bw = doc["bw"];
  //Serial.println(frq);
  //Serial.println(oldfrq);
  // Si les paramètres sont différents des précédents alors on reconfigure la liaison LoRa
  if( (frq!=oldfrq) && (sf!=oldsf) && (bw!=oldbw) ){
  LoRa.end();
  LoRa.setFrequency(frq);
  LoRa.setSpreadingFactor(sf);
  LoRa.setSignalBandwidth(bw);
  LoRa.begin(frq);
  oldfrq = frq; 
  oldsf = sf;
  oldbw = bw;
  Serial.println("LoRa reconfiguré avec succès avec les paramètres reçus");
  }
}



