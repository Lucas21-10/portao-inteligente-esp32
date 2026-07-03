/*
Projeto: Sistema Inteligente de Controle de Portão Automatizado

Componentes:
1 - ESP32: controlador principal
2 - Relé: acionamento do motor
3 - LED verde: portão aberto
4 - LED vermelho: portão fechado
5 - Botão aberto (GPIO 4): sensor fim de curso aberto
6 - Botão fechado (GPIO 5): sensor fim de curso fechado
*/

// Biblioteca Wi-Fi
#include <WiFi.h>

// biblioteca para transformar ESP32 em servidor Web
#include <WebServer.h>

// reponsável pela conexão HTTPS
#include <WiFiClientSecure.h>

// permite fazer requisições HTTP
#include <HTTPClient.h>



// LEDs
#define LED_VERDE 2
#define LED_VERMELHO 16

// Sensores
#define SENSOR_ABERTO 4
#define SENSOR_FECHADO 5

// Relé
#define RELE 18

// Wi-Fi do Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Telegram
const String BOT_TOKEN = "8849483743:AAEMKqqXxfATXzFhPV0Z9DxDdkHsXBZkkZA";
const String CHAT_ID = "8842562245";


// função para enviar mensagem
void enviarTelegram(String mensagem) {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado!");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;

  // Codifica espaços
  mensagem.replace(" ", "%20");

  String url = "https://api.telegram.org/bot" + BOT_TOKEN +
               "/sendMessage?chat_id=" + CHAT_ID +
               "&text=" + mensagem;

  Serial.println();
  Serial.println("Enviando mensagem ao Telegram...");
  Serial.println(url);

  http.begin(client, url);

  int httpResponseCode = http.GET();

  Serial.print("Codigo HTTP: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {

    String resposta = http.getString();

    Serial.println("Resposta do Telegram:");
    Serial.println(resposta);

  } else {

    Serial.print("Erro HTTP: ");
    Serial.println(http.errorToString(httpResponseCode));

  }

  http.end();

}

// cria um servidor web na porta 80
WebServer server(80);

void paginaInicial() {

  server.send(
    200,
    "text/html",
    "<h1>Projeto Portao Inteligente</h1>"
    "<p>ESP32 conectado com sucesso!</p>"
  );

}

void setup() {

  Serial.begin(115200);

  Serial.println("==========================================");
  Serial.println(" SISTEMA DE CONTROLE DE PORTAO");
  Serial.println(" Inicializando ESP32...");
  Serial.println("==========================================");

  // LEDs
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  // Sensores
  pinMode(SENSOR_ABERTO, INPUT_PULLUP);
  pinMode(SENSOR_FECHADO, INPUT_PULLUP);

  // Relé
  pinMode(RELE, OUTPUT);

  // Estado inicial
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(RELE, LOW);

  Serial.println("Componentes inicializados com sucesso.");
  Serial.println("Relé desligado.");
  Serial.println("LEDs desligados.");
  Serial.println();

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado com sucesso!");
  Serial.print("Endereco IP: ");
  Serial.println(WiFi.localIP());

  Serial.println();
  Serial.println("Sistema pronto para monitoramento.");
  Serial.println("------------------------------------------");
  enviarTelegram("Sistema iniciado com sucesso!");

 // Página inicial
 server.on("/", paginaInicial);

 // Inicia o servidor
 server.begin();

 Serial.println("Servidor Web iniciado.");
}

void loop() {

  server.handleClient();

  // Sensor de portão aberto
  if (digitalRead(SENSOR_ABERTO) == LOW) {

    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);

    // Motor parado
    digitalWrite(RELE, LOW);

    Serial.println();
    Serial.println(">>> EVENTO DETECTADO <<<");
    Serial.println("Sensor de fim de curso ABERTO acionado.");
    Serial.println("Estado do portao: ABERTO");
    Serial.println("LED Verde: LIGADO");
    Serial.println("LED Vermelho: DESLIGADO");
    Serial.println("Rele: DESLIGADO (motor parado)");
    Serial.println("------------------------------------------");

    enviarTelegram("Portao aberto");

    delay(200);

  }

  // Sensor de portão fechado
  else if (digitalRead(SENSOR_FECHADO) == LOW) {

    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_VERDE, LOW);

    // Motor parado
    digitalWrite(RELE, LOW);

    Serial.println();
    Serial.println(">>> EVENTO DETECTADO <<<");
    Serial.println("Sensor de fim de curso FECHADO acionado.");
    Serial.println("Estado do portao: FECHADO");
    Serial.println("LED Verde: DESLIGADO");
    Serial.println("LED Vermelho: LIGADO");
    Serial.println("Rele: DESLIGADO (motor parado)");
    Serial.println("------------------------------------------");

    enviarTelegram("Portao fechado");

    delay(200);

  }

}