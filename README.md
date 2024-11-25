# Semáforos Inteligentes 

Este projeto implementa semáforos inteligentes usando ESP32, sensores de luz (LDR), e comunicação via MQTT. Nosso projeto é capaz de alternar entre modos dia e noite, se comunicar com outros semáforos em tempo real e interagir com uma plataforma online, criada em HTML, pelo grupo. 

## Objetivos do Projeto

- **Controle automático de semáforos**: Alternar entre vermelho, amarelo e verde com base em condições predefinidas.
- **Modo noturno**: Ligar/desligar LEDs amarelos em intervalos regulares quando a luz ambiente é baixa.
- **Comunicação MQTT**: Semáforos sincronizados que ajustam o comportamento com base em mensagens recebidas.
- **Dashboard**: Integração com uma interface online. Embora o escopo mencionasse o Ubidots, tivemos alguns problemas e preferimos fazer a interface com HTML. 

---

## Materiais

| Componente         | Quantidade | Descrição                                                                 |
|---------------------|------------|---------------------------------------------------------------------------|
| ESP32              | 2          | Microcontrolador com suporte a Wi-Fi e MQTT                              |
| LEDs (Vermelho)    | 2          | LED para o sinal vermelho                                      |
| LEDs (Verde)       | 2          | LED para o sinal verde                                         |
| LEDs (Amarelo)     | 2          | LED para o sinal amarelo                                       |
| LDR (Sensor de Luz)| 1          | Sensor para detectar a luz ambiente                                      |
| Resistores         | 6          | Para limitar a corrente nos LEDs e no LDR                               |
| Protoboard         | 1          | Para facilitar as conexões                                              |
| Cabos Jumper       | Diversos (Macho-Macho e Macho-Fêmea)   | Para realizar as conexões entre componentes                              |

---

## Conexões

| ESP32 Pino | Componente       | Descrição                                |
|------------|------------------|------------------------------------------|
| GPIO 5     | LED Vermelho    | Controle do LED Vermelho                 |
| GPIO 25    | LED Verde       | Controle do LED Verde                    |
| GPIO 32    | LED Amarelo     | Controle do LED Amarelo                  |
| GPIO 35    | LDR             | Entrada do sensor de luz ambiente        |
| GND        | LEDs e LDR       | GND comum                                |

Para o segundo semáforo, replicamos o uso das portas para os leds e não necessitamos do uso de um outro LDR. 

---

## Funcionalidades

1. **Modo Dia:**
   - Os LEDs alternam entre vermelho, amarelo e verde em ciclos.
   - Sincronização entre dois semáforos para gerenciar o tráfego. **Isso significa que enquanto um farol estiver piscando verde, o outro piscará vermelho, eles só estarão na mesma cor quando a cor for a amarela.**
    

2. **Modo Noite:**
   - O LED amarelo pisca em intervalos regulares.
   - Ativado automaticamente com base nos valores de luz ambiente do LDR.
   - Imaginamos o modo noturno dessa maneira com base nas experiências dos integrantes pela cidade de São Paulo. Essa medida foi tomada contra os assaltos e os integrantes pensaram como referência. Vídeo sobre o assunto: https://www.youtube.com/watch?v=wvQnDp5n-Gs 

3. **Comunicação MQTT:**
   - O ESP32 publica e recebe mensagens no tópico MQTT `faroldatata`.
   - Mensagens como `LED Vermelho LIGADO` ou `LED Verde DESLIGADO` permitem que os semáforos coordenem suas ações.
   - As condições presentes no código são ativadas e desativadas de acordo com essas mensagens. 

4. **Interface HTML:**
   - A proposta é que hajam dois modos: **Simulação** e **Real**.
   - Real: a luz que atinge o sensor LDR é quem comanda o comportamento do circuito.
   - Simulação: o número definido na interface é quem comanda o comportamento do circuito.
   - A ideia é controlar o protótipo remotamente, se desvinvulando da ideia original de monitoramento.
   - O HTML se tornou a opção mais viável com os eventuais problemas que surgiram com o Ubidots. 

---

## 🚀 Como Rodar o Projeto

1. **Configurar o Ambiente de Desenvolvimento:**
   - Instale o [Arduino IDE](https://www.arduino.cc/en/software).
   - Configure a placa ESP32 seguindo [este tutorial](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/).

2. **Carregar o Código:**
   - Clone este repositório:
     ```bash
     git clone https://github.com/seu-usuario/semaforo-inteligente.git
     cd semaforo-inteligente
     ```
   - Abra o código no Arduino IDE.
   - Atualize as credenciais Wi-Fi e informações MQTT no código:
     ```cpp
     const char* ssid = "SeuWiFi";
     const char* password = "SuaSenha";
     const char* mqtt_server = "SeuBrokerMQTT";
     const char* mqtt_user = "SeuUsuario";
     const char* mqtt_pass = "SuaSenhaMQTT";
     ```
    - O grupo recomenda o uso do HiveMQ, em especial o seu uso através de Clusters, com o HiveMq Cloud. 

3. **Montar o Circuito:**
   - Conecte os componentes de acordo com a tabela de conexões.

4. **Testar a Comunicação:**
   - Suba o código nos dois ESP32 e verifique a sincronização entre os semáforos.
   - Use um cliente MQTT (como o [HiveMQ](https://www.hivemq.com/)) que citamos anteriormente para monitorar o tráfego de mensagens.

---

## Interface em HTML

1. Tenha acesso ao LiveService ou semelhante.
2. Atualize o código para certificar-se de que o ip, presente em um dos trechos, de fato corresponde ao ESP32 que você está utilizando. No nosso código do dispositivo pai, fazemos uma breve verificação de qual o ip do esp32 que está rodando o código. 
3. Divirta-se alterando entre os modos. 

---

## Demonstração

Todas estas coisas citadas podem ser observadas através dos vídeos de demonstração, que podem ser acessados pelo seguinte link:
([Drive]([https://www.hivemq.com/](https://drive.google.com/drive/folders/1ndNf0GCn0g66sDceawAxodwj5g5HO9kM?usp=sharing))

---
