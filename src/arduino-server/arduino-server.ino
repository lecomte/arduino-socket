/*
  Project Name: Simple Arduino Communication Project
  Authors:
        Gabriel Lecomte Pinho e Souza
        Pedro Vidal Sales
  Version: 1.0.0
  Boards Supported:
      Arduino
      NodeMCU
  Dependencies:
      Ethernet, 1.1.2
      SPI, 1.0.0
  Description:
    Um simples robô que recebe solicitações dos tipos Get e Set para modificar ou consultar valores de pinos do Arduino ou de um vetor de inteiros.
*/
#include <SPI.h>
#include <Ethernet.h>

#define ENCODER_DIREITA 2
#define ENCODER_ESQUERDA 3

#define ACELERADOR_DIREITA 5
#define ACELERADOR_ESQUERDA 6

//  Definição das entradas para os motores de cada lado, observem que alguns robôs precisaram ter os pinos trocados devido aos módulos de comunicação que utilizam.
//  Vejam os 4 fios unidos da ponte H que são conectados no Arduino para saberem o padrão do robô que vocês tem em mãos.
#define DIRECAO_DIREITA_1 4   //4
#define DIRECAO_DIREITA_2 7   //7

#define DIRECAO_ESQUERDA_1 8  //8
#define DIRECAO_ESQUERDA_2 9 //9

#define IR_PARA_FRENTE_DIREITA() do { digitalWrite(DIRECAO_DIREITA_1, HIGH); digitalWrite(DIRECAO_DIREITA_2, LOW); } while(false)
#define IR_PARA_FRENTE_ESQUERDA() do { digitalWrite(DIRECAO_ESQUERDA_1, HIGH); digitalWrite(DIRECAO_ESQUERDA_2, LOW); } while(false)
#define IR_PARA_FRENTE() do { IR_PARA_FRENTE_DIREITA(); IR_PARA_FRENTE_ESQUERDA(); } while(false)

#define IR_PARA_TRAS_DIREITA() do { digitalWrite(DIRECAO_DIREITA_1, LOW); digitalWrite(DIRECAO_DIREITA_2, HIGH); } while(false)
#define IR_PARA_TRAS_ESQUERDA() do { digitalWrite(DIRECAO_ESQUERDA_1, LOW); digitalWrite(DIRECAO_ESQUERDA_2, HIGH); } while(false)
#define IR_PARA_TRAS() do { IR_PARA_TRAS_DIREITA(); IR_PARA_TRAS_ESQUERDA(); } while(false)

#define ACELERA_DIREITA(VELOCIDADE) do { pwmDireita = VELOCIDADE; analogWrite(ACELERADOR_DIREITA, VELOCIDADE); } while(false)
#define ACELERA_ESQUERDA(VELOCIDADE) do { pwmEsquerda = VELOCIDADE; analogWrite(ACELERADOR_ESQUERDA, VELOCIDADE); } while(false)
#define ACELERA(VELOCIDADE) do { ACELERA_DIREITA(VELOCIDADE); ACELERA_ESQUERDA(VELOCIDADE); } while(false)

#define FREIO_DIREITA() { ACELERA_DIREITA(0); digitalWrite(DIRECAO_DIREITA_1, LOW); digitalWrite(DIRECAO_DIREITA_2, LOW); } while(false)
#define FREIO_ESQUERDA() { ACELERA_ESQUERDA(0); digitalWrite(DIRECAO_ESQUERDA_1, LOW); digitalWrite(DIRECAO_ESQUERDA_2, LOW); } while(false)
#define FREIO() do { FREIO_DIREITA(); FREIO_ESQUERDA(); } while(false)

// Velocidade armazenada PWM
volatile int pwmDireita = 0;
volatile int pwmEsquerda = 0;

// Numero de passos do Encoder Ótico
volatile int contador_direita = 0;
volatile int contador_esquerda = 0;

// Definir velocidades
int velocidadeDireita  = 230;
int velocidadeEsquerda = 230;


byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 141, 30);
int values[27];

EthernetServer server(8080);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Servidor iniciado em: ");
  Serial.println(Ethernet.localIP());

  // Configuração dos pinos da Ponte H
  pinMode(DIRECAO_DIREITA_1, OUTPUT);
  pinMode(DIRECAO_DIREITA_2, OUTPUT);
  pinMode(DIRECAO_ESQUERDA_1, OUTPUT);
  pinMode(DIRECAO_ESQUERDA_2, OUTPUT);

  // Configuração dos pinos do Encoder Ótico
  pinMode(ENCODER_DIREITA, INPUT_PULLUP);
  pinMode(ENCODER_ESQUERDA, INPUT_PULLUP);

  // Funções de Interrupção de cada um dos Encoders
  attachInterrupt(digitalPinToInterrupt(ENCODER_DIREITA), contadorDireita, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_ESQUERDA), contadorEsquerda, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Conexao recebida!");
    String message = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c != '\n') {
          message += c;
        }
        else {
          if (message[0] == 'G') {
            Serial.println("Operacao GET recebida!");
            int result, pos = message.substring(1, message.length()).toInt();
            if (pos <= 13) {
              result = digitalRead(pos);
            }
            else if (pos == 40) {
              result = contador_direita;  
            }
            else {
              result = values[pos - 13];
            }
            Serial.print("Valor solicitado ");
            Serial.print(pos);
            Serial.print(": ");
            Serial.println(result);
            String sres(result);
            client.print("R" + sres);
          }
          else if (message[0] == 'S') {
            Serial.println("Operacao SET recebida!");
            message = message.substring(1, message.length());
            int eqPos = message.indexOf('=');
            int dest = message.substring(0, eqPos).toInt(), num = message.substring(eqPos + 1, message.length()).toInt();
            Serial.print("Valor solicitado ");
            Serial.print(dest);
            Serial.print(": ");
            Serial.println(num);
            if (dest <= 13) {
              if (num > 1 || num < 0) {
                client.println("R0");
              }
              else {
                digitalWrite(dest, num);
                client.println("R1");
              }
            }
            else {
              values[dest - 13] = num;
              client.println("R1");
            }
          }
          message = "";
          break;
        }
      }
    }
    client.stop();
    Serial.println("Conexao encerrada!");
  }
  else {
    if (values[1] >= 1) {
      ACELERA_DIREITA(velocidadeDireita);
      ACELERA_ESQUERDA(velocidadeEsquerda);
      IR_PARA_FRENTE_DIREITA();
      IR_PARA_FRENTE_ESQUERDA();
      delay(1000 * values[1]);
      FREIO();
      delay(1500);
      values[1] = 0;
    }
  }
}

void contadorDireita(){
  contador_direita++;
}

void contadorEsquerda(){
  contador_esquerda++;
}
