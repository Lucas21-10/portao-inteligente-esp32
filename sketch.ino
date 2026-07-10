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


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "time.h" // Biblioteca nativa para hora real (NTP)

// Configuração de Pinos 
#define LED_VERDE 2
#define LED_VERMELHO 16
#define SENSOR_ABERTO 4
#define SENSOR_FECHADO 5
#define RELE 18

// Redes e Credenciais 
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Telegram
const String BOT_TOKEN = "8849483743:AAEMKqqXxfATXzFhPV0Z9DxDdkHsXBZkkZA";
const String CHAT_ID = "8842562245";

// Configuração NTP (Hora Real)
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -10800; // Fuso horário UTC-3 (Brasília)
const int daylightOffset_sec = 0;   // Sem horário de verão

// --- Estados e contadores
enum EstadoPortao { FECHADO, ABERTO, EM_MOVIMENTO };
EstadoPortao estadoAtual = FECHADO;
EstadoPortao ultimoEstado = FECHADO;

int contadorAberturas = 0;
int contadorFechamentos = 0;

unsigned long tempoAberturaMillis = 0;
unsigned long tempoTotalAbertoSegundos = 0;

// Protótipos das Funções
void conectarWiFi();
void configurarNTP();
String obterHoraAtual();
void enviarTelegram(String mensagem);
void gerenciarEstadoPortao();
void atualizarPainelSerial();

// Setup
void setup() {
  Serial.begin(115200);
  
  // Inicialização de Pinos
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(SENSOR_ABERTO, INPUT_PULLUP);
  pinMode(SENSOR_FECHADO, INPUT_PULLUP);
  pinMode(RELE, OUTPUT);

  // Estado Inicial dos Pinos
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(RELE, LOW);

  conectarWiFi();
  configurarNTP();

  // Força o início do sistema como FECHADO para sincronia estável com o Wokwi
  if (digitalRead(SENSOR_ABERTO) == LOW) {
    estadoAtual = ABERTO;
    ultimoEstado = ABERTO;
    digitalWrite(LED_VERDE, HIGH);
  } else {
    estadoAtual = FECHADO;
    ultimoEstado = FECHADO;
    digitalWrite(LED_VERMELHO, HIGH); // Corrigido aqui!
  }

  enviarTelegram(" *Sistema de Portão Iniciado!* \nMonitoramento ativo via Serial.");
  
  // Imprime o painel inicial assim que o sistema liga
  atualizarPainelSerial();
}

// Loop 
void loop() {
  gerenciarEstadoPortao(); 
}

//  Implementação das Funções

// Conecta o ESP32 à rede Wi-Fi do simulador Wokwi.
void conectarWiFi() {
  Serial.println("\n==========================================");
  Serial.print("[WIFI] Conectando a rede: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[WIFI] Conectado com sucesso!");
  Serial.print("[WIFI] Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("==========================================");
}

// Sincroniza o relógio interno do ESP32 com servidores globais de internet.
void configurarNTP() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("[NTP] Sincronizando horário");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[NTP] Horário sincronizado com sucesso.");
}

// Retorna texto formatado com data e hora local de Brasília.
String obterHoraAtual() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "00/00/0000 00:00:00";
  }
  char buffer[25];
  strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
  return String(buffer);
}

// Dispara requisição HTTP segura para enviar mensagens ao bot do Telegram.
void enviarTelegram(String mensagem) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[ERRO] Falha no envio do Telegram: WiFi Desconectado!");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;

  mensagem.replace(" ", "%20");
  mensagem.replace("\n", "%0A");

  String url = "https://api.telegram.org/bot" + BOT_TOKEN + "/sendMessage?chat_id=" + CHAT_ID + "&text=" + mensagem + "&parse_mode=Markdown";

  http.begin(client, url);
  int httpResponseCode = http.GET();
  http.end();
}

/**
 * Lógica sequencial adaptada para botões de pulso rápidos no Wokwi.
 * Alterna o estado para EM_MOVIMENTO no primeiro clique e confirma o fim de curso no segundo.
 */
void gerenciarEstadoPortao() {
  bool sensorAberto = (digitalRead(SENSOR_ABERTO) == LOW);
  bool sensorFechado = (digitalRead(SENSOR_FECHADO) == LOW);

  // Transições inteligentes baseadas no clique do botão e no estado de memória anterior
  if (sensorAberto) {
    if (estadoAtual == FECHADO) {
      estadoAtual = EM_MOVIMENTO;
      delay(300); // Para evitar leituras duplicadas no mesmo clique
    } else if (estadoAtual == EM_MOVIMENTO) {
      estadoAtual = ABERTO;
      delay(300);
    }
  } 
  else if (sensorFechado) {
    if (estadoAtual == ABERTO) {
      estadoAtual = EM_MOVIMENTO;
      delay(300);
    } else if (estadoAtual == EM_MOVIMENTO) {
      estadoAtual = FECHADO;
      delay(300);
    }
  }

  // Se o estado mudou, processa as saídas, contadores e alertas
  if (estadoAtual != ultimoEstado) {
    String horario = obterHoraAtual();
    String msgTelegram = "";

    if (estadoAtual == ABERTO) {
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_VERMELHO, LOW);
      digitalWrite(RELE, LOW);

      contadorAberturas++;
      tempoAberturaMillis = millis(); // Inicia contagem do tempo aberto

      msgTelegram = " *Portão ABERTO*\n"
                    " Hora: " + horario + "\n"
                    " Total de Aberturas: " + String(contadorAberturas);
    } 
    else if (estadoAtual == FECHADO) {
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_VERMELHO, HIGH);
      digitalWrite(RELE, LOW);

      contadorFechamentos++;
      
      if (tempoAberturaMillis > 0) {
        tempoTotalAbertoSegundos = (millis() - tempoAberturaMillis) / 1000;
        tempoAberturaMillis = 0; // Reseta cronômetro
      }

      msgTelegram = " *Portão FECHADO*\n"
                    " Hora: " + horario + "\n"
                    " Ficou aberto por: " + String(tempoTotalAbertoSegundos) + "s\n"
                    " Total de Fechamentos: " + String(contadorFechamentos);
    } 
    else if (estadoAtual == EM_MOVIMENTO) {
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_VERMELHO, LOW);
      digitalWrite(RELE, LOW); 

      // Se estava aberto e começou a fechar, calcula o tempo de permanência imediatamente
      if (ultimoEstado == ABERTO && tempoAberturaMillis > 0) {
        tempoTotalAbertoSegundos = (millis() - tempoAberturaMillis) / 1000;
      }

      msgTelegram = " *Portão em Movimento...*\n"
                    " Hora: " + horario;
    }

    enviarTelegram(msgTelegram);
    ultimoEstado = estadoAtual; 
    atualizarPainelSerial(); 
  }
}

// Desenha a interface no Monitor Serial.
void atualizarPainelSerial() {
  String statusStr = "";
  if (estadoAtual == ABERTO) statusStr = "ABERTO";
  else if (estadoAtual == FECHADO) statusStr = "FECHADO";
  else statusStr = "EM MOVIMENTO";

  unsigned long tempoExibicaoAberto = tempoTotalAbertoSegundos;
  if (estadoAtual == ABERTO && tempoAberturaMillis > 0) {
    tempoExibicaoAberto = (millis() - tempoAberturaMillis) / 1000;
  }

  Serial.println("\n=============================================");
  Serial.println("         PAINEL DE CONTROLE DE PORTÃO        ");
  Serial.println("=============================================");
  Serial.print("  Hora Atual:     "); Serial.println(obterHoraAtual());
  Serial.print("  Estado Atual:   "); Serial.println(statusStr);
  Serial.println("---------------------------------------------");
  Serial.print("  Contador Aberturas:   "); Serial.println(contadorAberturas);
  Serial.print("  Contador Fechamentos: "); Serial.println(contadorFechamentos);
  Serial.print("  Último Tempo Aberto:  "); Serial.print(tempoExibicaoAberto); Serial.println(" segundos");
  Serial.println("=============================================");
}