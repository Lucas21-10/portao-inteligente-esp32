
Monitoramento Inteligente de Portão de Garagem com ESP32

Este projeto simula um sistema de portão eletrônico inteligente de garagem. Ele usa uma placa chamada ESP32 para monitorar a posição do portão (se está aberto ou fechado) através de sensores, avisa o dono da casa em tempo real pelo Telegram e mostra um relatório completo na tela do computador.

Peças Usadas no Projeto
Placa ESP32: O "cérebro" do projeto, que tem Wi-Fi integrado para se conectar à internet.

Dois Botões: Que fingem ser os sensores de fim de curso (aqueles que o portão encosta quando termina de abrir ou fechar).

LED Vermelho: Acende para mostrar que o portão está fechado.

LED Verde: Acende para mostrar que o portão está aberto.

Módulo Relé: A peça que na vida real ligaria e desligaria o motor do portão.


O sistema foi programado para ser inteligente e não ficar mandando mensagens repetidas no seu celular. Ele funciona assim:

Portão Fechado: Quando você liga o sistema, o LED vermelho acende. O portão está seguro e fechado.

Portão em Movimento: Quando você clica no botão para abrir, os LEDs se apagam. O sistema entende que o portão está no meio do caminho e te manda uma mensagem no Telegram: "Portão em Movimento...".

Portão Aberto: Ao clicar no botão de abertura total, o LED verde acende. O ESP32 começa a contar o tempo no relógio. Você recebe um aviso: "Portão ABERTO", junto com o número de vezes que ele já foi aberto no dia.

Calculando o Tempo Exposto: Quando o portão começa a fechar de novo, o sistema faz uma conta matemática rápida e descobre exatamente quantos segundos a sua garagem ficou aberta e desprotegida. Ele te manda essa informação no Telegram: "Portão FECHADO. Ficou aberto por: 15 segundos".

Hora Certa: Toda vez que o portão se move, o ESP32 vai na internet e puxa o horário oficial de Brasília. Assim, todas as mensagens chegam no seu Telegram com a data e a hora exatas do acontecimento.

Como o Código Foi Organizado?
Para o código não virar uma bagunça, ele foi dividido em tarefas bem definidas:

Organização Inicial (Protótipos): No começo do código, avisamos ao sistema tudo o que ele vai precisar fazer. Isso deixa o código limpo e elegante.

A Inicialização (Setup): Quando a placa liga, ela ativa as luzes, os botões, conecta no Wi-Fi da sua casa e arruma o relógio com a hora certa da internet. Ela também manda uma mensagem de boas-vindas: "Sistema Iniciado!".

O Monitoramento Contínuo (Loop): A placa fica lendo os botões milhares de vezes por segundo de forma super leve, esperando você interagir.

O Painel de Controle (Serial): Toda vez que você mexe no portão, uma tabela bonita e organizada aparece na tela do computador mostrando a hora atual, o estado do portão, quantas vezes ele abriu/fechou e o tempo que passou aberto.

Conclusão e Testes
Todo o projeto — desde a fiação dos componentes até a programação dos alertas de internet — foi montado e testado com sucesso dentro do simulador Wokwi, funcionando exatamente como um portão eletrônico real funcionaria na sua casa.

🔗 Link da simulação:

https://wokwi.com/projects/464730610212755457