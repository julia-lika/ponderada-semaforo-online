#include <WiFi.h>
#include <PubSubClient.h>

const char* HOST = "broker.hivemq.com";  // Host do broker HiveMQ
const int MQTT_PORT = 1883;               // Porta padrão para TCP no HiveMQ
const char* CLIENT_ID = "clientId-qLqratbjjD";
const char* TOPIC = "conexao/esp";

WiFiClient espClient;               // Cliente WiFi
PubSubClient client(espClient);     // Cliente MQTT usando o WiFiClient

// Função para conectar à rede Wi-Fi
void connectWiFi() {
    delay(10);
    Serial.println();
    Serial.print("Conectando a ");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
}

// Função de callback chamada quando uma mensagem é recebida
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Mensagem recebida no tópico: ");
    Serial.println(topic);
    Serial.print("Mensagem: ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

// Função para conectar ao broker MQTT
void connectMQTT() {
    while (!client.connected()) {
        Serial.print("Conectando ao broker MQTT...");
        if (client.connect(CLIENT_ID)) {  // Conectando com o CLIENT_ID
            Serial.println("Conectado!");
            client.subscribe(TOPIC);  // Se inscreve no tópico
        } else {
            Serial.print("Falha na conexão. Estado MQTT: ");
            Serial.print(client.state());
            delay(2000);  // Aguarda 2 segundos antes de tentar novamente
        }
    }
}

// Função de configuração do servidor MQTT
void setMQTTServer() {
  client.setServer(HOST, MQTT_PORT);  // Configura o servidor MQTT e a porta TCP
}

void setup() {
    Serial.begin(115200);
    connectWiFi();  // Conecta à rede Wi-Fi

    setMQTTServer();  // Configura o servidor MQTT
    client.setCallback(callback);  // Define a função de callback para mensagens recebidas
}

void loop() {
    if (!client.connected()) {  // Se não estiver conectado ao MQTT, tenta reconectar
        connectMQTT();
    }
    client.loop();  // Mantém a conexão MQTT ativa

    // Publicando uma mensagem no tópico "conexao/esp"
    String mensagem = "Olá MQTT em C++!";
    client.publish(TOPIC, mensagem.c_str());  // Publica a mensagem

    delay(10000);  // Aguarda 10 segundos antes de publicar novamente
}