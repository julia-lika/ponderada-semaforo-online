#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <WebServer.h>

const char* ssid = "Apollo";
const char* password = "runawafe2018";

const int mqtt_port = 8883;
const char* mqtt_server = "53a6963180994f47ae948ac516fe0611.s1.eu.hivemq.cloud";
const char* mqtt_topic = "faroldatata";
const char* mqtt_user = "Fernando777";
const char* mqtt_pass = "Fe!n7777";

WebServer server(80);
WiFiClientSecure espClient;
PubSubClient client(espClient);

#define RED 5
#define GREEN 25
#define YELLOW 32
#define LDR 34

bool isDayMode = false;
int ldrThreshold = 1000;
bool isSimulationMode = false;

unsigned long previousMillis = 0;
unsigned long intervalRed = 4000;
unsigned long intervalYellow = 2000;
unsigned long intervalGreen = 4000;

int stage = 0;
int valorldr = 0;

void configurarPinos() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(LDR, INPUT);
}

void enviarMensagemMQTT(const char* mensagem) {
  if (client.connected()) {
    client.publish(mqtt_topic, mensagem);
    Serial.println(String("Mensagem enviada: ") + mensagem);
  } else {
    Serial.println("MQTT desconectado. Não foi possível enviar a mensagem.");
  }
}

void handleSetMode() {
  if (server.hasArg("mode")) {
    String mode = server.arg("mode");
    if (mode == "simulation") {
      isSimulationMode = true;
      resetarLEDs();
      isDayMode = false;
      stage = 0;
      previousMillis = millis();
      server.send(200, "text/plain", "Modo de simulação ativado.");
      Serial.println("Modo alterado para SIMULAÇÃO.");
    } else if (mode == "real") {
      isSimulationMode = false;
      resetarLEDs();
      isDayMode = false;
      stage = 0;
      previousMillis = millis();
      server.send(200, "text/plain", "Modo de LDR físico ativado.");
      Serial.println("Modo alterado para REAL.");
    } else {
      server.send(400, "text/plain", "Modo inválido.");
    }
  } else {
    server.send(400, "text/plain", "Argumento 'mode' não fornecido.");
  }
}

void handleSetSimulatedLDR() {
  if (server.hasArg("value")) {
    valorldr = server.arg("value").toInt();
    server.send(200, "text/plain", "Valor LDR simulado ajustado para: " + String(valorldr));
    Serial.println("Valor LDR (simulado) ajustado para: " + String(valorldr));
  } else {
    server.send(400, "text/plain", "Valor inválido.");
  }
}

void resetarLEDs() {
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
}

void modoNoite() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 2000) {
    bool estado = digitalRead(YELLOW);
    digitalWrite(YELLOW, !estado);
    enviarMensagemMQTT(!estado ? "LED Amarelo LIGADO no modo noite." : "LED Amarelo DESLIGADO no modo noite.");
    previousMillis = currentMillis;
  }
}

void modoDia() {
  unsigned long currentMillis = millis();
  switch (stage) {
    case 0:
      resetarLEDs();
      digitalWrite(RED, HIGH);
      enviarMensagemMQTT("LED Vermelho LIGADO no modo dia.");
      if (currentMillis - previousMillis >= intervalRed) {
        previousMillis = currentMillis;
        stage = 1;
      }
      break;
    case 1:
      resetarLEDs();
      digitalWrite(YELLOW, HIGH);
      enviarMensagemMQTT("LED Amarelo LIGADO no modo dia.");
      if (currentMillis - previousMillis >= intervalYellow) {
        previousMillis = currentMillis;
        stage = 2;
      }
      break;
    case 2:
      resetarLEDs();
      digitalWrite(GREEN, HIGH);
      enviarMensagemMQTT("LED Verde LIGADO no modo dia.");
      if (currentMillis - previousMillis >= intervalGreen) {
        previousMillis = currentMillis;
        stage = 0;
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  configurarPinos();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP do servidor: ");
  Serial.println(WiFi.localIP());

  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.connect("ESP32Client", mqtt_user, mqtt_pass);

  server.on("/setMode", handleSetMode);
  server.on("/setLdr", handleSetSimulatedLDR);
  server.begin();
}

void loop() {
  if (!client.connected()) {
    client.connect("ESP32Client", mqtt_user, mqtt_pass);
  }
  client.loop();
  server.handleClient();

  if (isSimulationMode) {
    Serial.println("Modo simulação ativo.");
    Serial.print("Valor LDR (simulado): ");
    Serial.println(valorldr);
  } else {
    Serial.println("Modo real ativo.");
    valorldr = analogRead(LDR); // Lê o valor real do LDR físico
    Serial.print("Valor LDR (real): ");
    Serial.println(valorldr);
  }

  if (valorldr > ldrThreshold) {
    if (!isDayMode) {
      resetarLEDs();
      stage = 0;
      previousMillis = millis();
      isDayMode = true;
      Serial.println("Mudança para MODO DIA (LDR acima do limite)");
    }
    modoDia();
  } else {
    if (isDayMode) {
      resetarLEDs();
      previousMillis = millis();
      isDayMode = false;
      Serial.println("Mudança para MODO NOITE (LDR abaixo do limite)");
    }
    modoNoite();
  }
}
