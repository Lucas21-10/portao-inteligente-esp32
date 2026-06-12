# Sistema Inteligente de Controle de Portão Automatizado com ESP32

## Descrição

Projeto desenvolvido para a disciplina de Sistemas Embarcados.

O sistema utiliza um ESP32 para monitorar sensores de fim de curso e controlar um motor de portão através de um módulo relé.

## Componentes Utilizados

* ESP32
* Módulo Relé
* LED Verde
* LED Vermelho
* Sensor de fim de curso aberto (simulado por botão)
* Sensor de fim de curso fechado (simulado por botão)

## Funcionamento

O ESP32 monitora dois sensores responsáveis por indicar se o portão está totalmente aberto ou totalmente fechado.

Quando o sensor aberto é acionado:

* LED verde acende.
* LED vermelho apaga.

Quando o sensor fechado é acionado:

* LED vermelho acende.
* LED verde apaga.

O ESP32 também realiza conexão à rede Wi-Fi para futuras integrações com interface web.

## Estrutura do Código

### setup()

Responsável por:

* Inicialização dos pinos.
* Inicialização da comunicação serial.
* Conexão Wi-Fi.

### loop()

Responsável por:

* Leitura dos sensores.
* Atualização dos LEDs.
* Controle do estado do sistema.

## Futuras Implementações

* Interface web para abertura e fechamento do portão.
* Comunicação com backend.
* Uso de interrupções para sensores.
* Atualização de status em tempo real.

## Protótipo

O protótipo foi desenvolvido utilizando a plataforma Wokwi para simulação do circuito.
