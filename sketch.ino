/*
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

void setup() {

  Serial.begin(115200);

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

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  Serial.println("");
  Serial.println("WiFi conectado!");

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  // Sensor de portão aberto
  if (digitalRead(SENSOR_ABERTO) == LOW) {

    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);

    // Motor parado
    digitalWrite(RELE, LOW);

    Serial.println("PORTAO ABERTO");

    delay(200);

  }

  // Sensor de portão fechado
  else if (digitalRead(SENSOR_FECHADO) == LOW) {

    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_VERDE, LOW);

    // Motor parado
    digitalWrite(RELE, LOW);

    Serial.println("PORTAO FECHADO");

    delay(200);

  }

}