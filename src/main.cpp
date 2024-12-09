#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char *ssid = "BTS_CIEL";
const char *password = "ERIR1234";

const int ledDroite = 26;  
const int ledBas = 18;     
const int ledHaut = 33;    
const int ledGauche = 15;  
const int led = 2;         

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("\n");
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
  pinMode(ledDroite, OUTPUT);
  pinMode(ledBas, OUTPUT);
  pinMode(ledHaut, OUTPUT);
  pinMode(ledGauche, OUTPUT);

  //---------------------------SPIFFS-------------------
  if (!SPIFFS.begin()) {
    Serial.println("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.print("File: ");
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }

  //-----------------------WIFI-----------------------------
  WiFi.begin(ssid, password);
  Serial.print("Tentative de connexion...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnexion etablie!");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());

  //--------------------------SERVEUR--------------------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(ledGauche, HIGH);
    digitalWrite(ledHaut, LOW);
    digitalWrite(ledBas, LOW);
    digitalWrite(ledDroite, LOW);
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/up", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(ledHaut, HIGH);
    digitalWrite(ledGauche, LOW);
    digitalWrite(ledBas, LOW);
    digitalWrite(ledDroite, LOW);
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/down", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(ledBas, HIGH);
    digitalWrite(ledHaut, LOW);
    digitalWrite(ledGauche, LOW);
    digitalWrite(ledDroite, LOW);
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(ledDroite, HIGH);
    digitalWrite(ledBas, LOW);
    digitalWrite(ledHaut, LOW);
    digitalWrite(ledGauche, LOW);
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.begin();
  Serial.println("Serveur actif!");
}
void loop()
 {}