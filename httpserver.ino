#include <WiFi.h>
#include <WebServer.h>
// Paramètres du point d'accès WiFi
const char* ssid = "Max";
const char* password = "Wifi1234";
WebServer server(80);
void setup() {
  Serial.begin(9600);
  // Configuration du point d'accès
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  // Définition des routes
  server.on("/", HTTP_GET, []() {
    // Réponse avec les paramètres LoRa au format JSON
    server.send(200, "application/json", "{\"frequency\":868E6,\"sf\":7,\"bw\":125000}");
  });
  server.begin();
  Serial.println("Serveur HTTP démarré");
}
void loop() {
  server.handleClient();
}
