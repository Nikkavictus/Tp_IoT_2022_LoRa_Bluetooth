# Tp_IoT_2023_LoRa_Bluetooth
### Maxime Goubault - Louis Tamisier - Enes Kocer - Antoine Raffray - Sylvain Madre
## Introduction 
Dans le cadre de la mise en place d'un système de communication combinant l'utilisation du WiFi, du protocole MQTT via un broker Mosquitto et de la technologie LoRa, deux cartes ESP32 sont configurées pour exécuter des tâches spécifiques. Ce TP vise à établir différentes communications entre les deux cartes. 
### Carte 1 : L'Émetteur MQTT et Client LoRa
La première carte ESP32 agit comme un recepteur LoRa tout en étant éditeur d'un broker MQTT. Elle commence par se connecter à un réseau WiFi, ce qui lui permet de communiquer avec le broker MQTT. Une fois connectée, la carte publie régulièrement, toutes les 5 secondes, les paramètres de communication LoRa sur le broker MQTT. En parrallèle, la carte est en écoute LoRa afin d'afficher les messages recu. 
### Carte 2 : L'Émetteur LoRa et Client MQTT
La seconde carte agit comme un émetteur LoRa tout en étant client d'un broker MQTT. Elle commence par se connecter à un réseau WiFi, ce qui lui permet de communiquer avec le broker MQTT à l'adresse 10.3.141.205 sur le topic srt5/SUPERMAX. Une fois connectée, la carte recoie régulièrement, toutes les 5 secondes, les paramètres de communication LoRa sur le broker MQTT. Ces paramètres incluent des informations  nécessaires à la communication LoRa : Frequeny, Spreading Factor et Bandwith. Si les paramètres de connexion sont différents des derniers utilisés alors on reconfigure nos paramètres LoRa puis on initialise la connexion et on continue d'envoyer "Bonjour LoRa" en boucle toutes les 5 secondes. 
![image](https://github.com/Nikkavictus/Tp_IoT_2022_LoRa_Bluetooth/assets/150132808/2c927f57-6744-4011-9ef1-d955be5a4c94)

## Bibliotèques utilisés
### Wifi.h
Cette bibliothèque permet à l'ESP32 de se connecter à un réseau sans fil. Elle fournit des fonctions pour gérer la connexion WiFi, telles que la connexion à un réseau WiFi en utilisant un SSID et un mot de passe, la vérification de l'état de la connexion, et l'obtention de l'adresse IP de l'ESP32 sur le réseau.
### MQTT.h et MQTTCLient.h
Ces bibliothèques me permettent d'intégrer les fonctionnalités du protocole MQTT dans mon projet. Grâce à elles, mon ESP32 peut communiquer avec un broker MQTT, publier des messages sur des topics spécifiques et s'abonner à des topics pour recevoir des messages. Elles sont cruciales pour l'échange de messages bidirectionnel fiable nécessaire à mes applications IoT.
### LoRa.h
Enfin, la bibliothèque LoRa permet d'établir une communication à longue portée et faible consommation entre nos ESP32 en utilisant le protocole LoRa.

## SenderMQTT_ReceiverLoRa
### Fonctions (Les différentes fonctions sont commentés dans le code) : 
#### void setup() : Configure les communications série, SPI, LoRa, se connecte au réseau WiFi et au serveur MQTT au démarrage du dispositif.
#### void loop() : Vérifie les paquets LoRa reçus et publie les messages sur le serveur MQTT à intervalles réguliers.
#### void connectToWiFi() : Établit la connexion WiFi avec les identifiants fournis.
#### void connectToMqtt() : Connecte le dispositif au serveur MQTT et prépare la communication MQTT.
#### void publishMessage() : Publie un message MQTT contenant les paramètres LoRa actuels.
## ReceiverMQTT_SenderLoRa
### Fonctions :
#### void setup() : Initialise les communications série, SPI, et LoRa, se connecte au WiFi et au serveur MQTT.
#### void loop() : Boucle principale qui envoie périodiquement des messages LoRa et maintient la connexion MQTT active.
#### void connectToWiFi() : Établit la connexion au réseau WiFi spécifié.
#### void connectToMqtt() : Connecte le dispositif au serveur MQTT, s'abonne à un topic, et définit une fonction de rappel pour les messages MQTT.
#### void messageReceived() : Fonction de rappel pour gérer les messages MQTT reçus, parse les données JSON, et reconfigure LoRa si nécessaire.

## Axes d'améliorations 
### Utilisation d'un serveur TCP 
L'utilisation d'un serveur TCP présente un avantage majeur sur MQTT en termes de taille de données transmises, grâce à sa capacité à envoyer des données brutes avec un overhead minimal, optimisant ainsi l'efficacité en bande passante pour les communications IoT.
### Utilisation d'une structure de données

## Bonus : 
Lorsque nous avons commencer le TP une erreur c'était glissé dans l'énoncé nous avons donc mis en place un serveur http ournissant les paramètres json de connexion LoRa en lieu et place de mqtt. Ce code est dipsonible dans le fichier : httpserver.ino
