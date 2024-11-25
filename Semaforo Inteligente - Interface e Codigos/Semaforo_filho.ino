#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Configurações Wi-Fi
const char* ssid = "Apollo";
const char* password = "runawafe2018";

// Configurações MQTT
const int mqtt_port = 8883;
const char* mqtt_server = "53a6963180994f47ae948ac516fe0611.s1.eu.hivemq.cloud";
const char* mqtt_topic = "faroldatata";
const char* mqtt_user = "Fernando777";
const char* mqtt_pass = "Fe!n7777";

static unsigned long lastPublish = 0;
String message = "";


#define RED 5
#define GREEN 25
#define YELLOW 32

// Configuração SSL para HiveMQ Cloud
WiFiClientSecure espClient; // Defina globalmente
PubSubClient client(espClient);

void setupWiFi() {
  Serial.println("Conectando WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("\nMensagem recebida:");
  Serial.print("Tópico: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  String receivedMessage;
  for (int i = 0; i < length; i++) {
    receivedMessage += (char)payload[i];
  }
  Serial.println(receivedMessage);

  // Ações baseadas na mensagem recebida
  if (receivedMessage == "LED Verde LIGADO no modo dia.") {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    Serial.println("LED Verde ligado!");
  } else if (receivedMessage == "LED Verde DESLIGADO no modo dia.") {
    digitalWrite(GREEN, LOW);
    Serial.println("LED Verde desligado!");
  } else if (receivedMessage == "LED Vermelho LIGADO no modo dia.") {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    Serial.println("LED Vermelho ligado!");
  } else if (receivedMessage == "LED Vermelho DESLIGADO no modo dia.") {
    digitalWrite(RED, LOW);
    Serial.println("LED Vermelho desligado!");
  } else if (receivedMessage == "LED Amarelo LIGADO no modo dia.") {
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    Serial.println("LED Amarelo ligado!");
  } else if (receivedMessage == "LED Amarelo DESLIGADO no modo dia.") {
    digitalWrite(YELLOW, LOW);
    Serial.println("LED Amarelo desligado!");
   }else if (receivedMessage == "LED Amarelo LIGADO no modo noite.") {
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    Serial.println("LED Amarelo ligado!");
  } else if (receivedMessage == "LED Amarelo DESLIGADO no modo noite.") {
    digitalWrite(YELLOW, LOW);
    Serial.println("LED Amarelo desligado!");
  } else {
    Serial.println("Comando desconhecido.");
  }
}

// Configuração dos pinos
void configurarPinos() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
}

// Reconexão MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando ao MQTT...");
    if (client.connect("clientId-NiczoLeBwO", mqtt_user, mqtt_pass)) {
      Serial.println("MQTT conectado!");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Falha na conexão MQTT. Código de erro: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// Configuração inicial
void setup() {
  Serial.begin(115200);
  configurarPinos();
  setupWiFi();

  espClient.setInsecure(); // Desativa a verificação do certificado SSL
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial.println("Setup concluído!");
}

// Loop principal
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}