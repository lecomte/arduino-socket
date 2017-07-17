#include <wiringPi.h>
#include <softPwm.h>
using namespace std;

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

#define ACELERA_DIREITA(VELOCIDADE) do { pwmDireita = VELOCIDADE; softPwmWrite(ACELERADOR_DIREITA, VELOCIDADE); } while(false)
#define ACELERA_ESQUERDA(VELOCIDADE) do { pwmEsquerda = VELOCIDADE; softPwmWrite(ACELERADOR_ESQUERDA, VELOCIDADE); } while(false)
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

void contadorDireita(){
	contador_direita++;
}

void contadorEsquerda(){
	contador_esquerda++;
}

int main() {
	wiringPiSetup();
	pinMode(DIRECAO_DIREITA_1, OUTPUT);
	pinMode(DIRECAO_DIREITA_2, OUTPUT);
	pinMode(DIRECAO_ESQUERDA_1, OUTPUT);
	pinMode(DIRECAO_ESQUERDA_2, OUTPUT);
	pinMode(ENCODER_DIREITA, INPUT);
	pinMode(ENCODER_ESQUERDA, INPUT);
	pullUpDnControl(ENCODER_DIREITA, PUD_UP);
	pullUpDnControl(ENCODER_ESQUERDA, PUD_UP);
	wiringPiISR(ENCODER_DIREITA, INT_EDGE_SETUP, contadorDireita);
	wiringPiISR(ENCODER_ESQUERDA, INT_EDGE_SETUP, contadorEsquerda);
	while(1) {
		
	}
}

