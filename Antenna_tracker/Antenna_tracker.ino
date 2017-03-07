
/**
 * Projeto desenvolvido por Samuel Rettore e Hugo Almeida
 * Baseado no projeto https://youtu.be/aACTKziuDVE
 * Antenna Tracker baseada em Arduino e RSSI 
 */
#include "Config.h"
#include <VarSpeedServo.h>
#include "Controle.h"

/**
 * Alterado para variaveis
 * Calibracao automatica
 */
//#define RSSI_MIN 85
//#define RSSI_MAX 244
//Rssi Direita
int RSSI_MIN_d = 0;
int RSSI_MAX_d = 0;
//Rssi Esquerda
int RSSI_MIN_e = 0;
int RSSI_MAX_e = 0;
//Rssi Suprerior
int RSSI_MIN_s = 0;
int RSSI_MAX_s = 0;


VarSpeedServo servoP;
//VarSpeedServo servoT;

int angulo = 90;

//Variaveis
int esquerda = 0, direita = 0;

#define INICIO  10
#define ULTIMO  INICIO - 1

uint16_t rssi_esquerda_array[INICIO];
uint16_t rssi_direita_array[INICIO];


#define SIGMOID_SLOPE       1
#define SIGMOID_OFFSET 4

Controle ctrl;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciou Setup");
  servoP.attach(SERVO_PAN);
  //servoT.attach(SERVO_TILT);

  mudaAngulo(SERVO_MIN,20);
  delay(100);
  mudaAngulo(SERVO_MAX,20);
  delay(100);
  mudaAngulo(90,20);
  delay(100);
  
  //Zera arrays  
  for (int i = 0; i < INICIO; i++) {
    rssi_esquerda_array[i] = 0;
    rssi_direita_array[i] = 0;
  }  
  Serial.println("Fim Setup");
  ctrl.Teste(3);
}

/**
 * Loop principal
 */
void loop() {
  verificaEntrada();
  Serial.print("RSSI-esquerda = ");
  Serial.print(esquerda);
  Serial.print(",direita = ");
  Serial.print(direita);   
  Serial.print(", angulo calc = ");
  Serial.print(angulo);  
  Serial.print(", RSSI_MIN_e = ");
  Serial.print(RSSI_MIN_e);
  Serial.print(" / ");
  Serial.print(RSSI_MAX_e);
  Serial.print(", RSSI_MIN_D = ");
  Serial.print(RSSI_MIN_d);
  Serial.print(", / ");
  Serial.println(RSSI_MAX_d);
}

/**
 * Metodo que verifica entrada e controla rotação esquerda ou direita, 
 * conforme leitura de RSSI
 * #featura: também deve calcular velocidade de deslocamento.
 */
void verificaEntrada(){
  
  //esquerda = random(85, 244);
  //esquerda=0;
  //direita = random(85, 244); 
  //direita=0;  
  
  //Le dados da porta
  esquerda = analogRead(RSSI_esquerda);
  direita = analogRead(RSSI_direita);
  
  //Desloca Array
  avancaArray(rssi_esquerda_array, INICIO);
  avancaArray(rssi_direita_array, INICIO);

  //Copia Para array
  rssi_esquerda_array[ULTIMO] = esquerda;
  rssi_direita_array[ULTIMO] = direita;
  
  //Pode calibrar pelo array
  calibraRSSI(esquerda,direita);

  uint16_t mediaEsquerda = max(media(rssi_esquerda_array, INICIO), RSSI_MIN_e);
  uint16_t mediaDireita = max(media(rssi_direita_array, INICIO), RSSI_MIN_d);

  
  //Experimental
  float ang = 0;
  
  if(esquerda > direita && abs(rssi_esquerda_array[ULTIMO-1]-esquerda) > DEADBAND){
    //angulo = (angulo+1)*SERVO_DIRECTION;

    float x = float(mediaEsquerda - mediaDireita) / 10;
    x = (1+ exp(-SIGMOID_SLOPE * x + SIGMOID_OFFSET));
    ang = x * SERVO_DIRECTION;
        
    
  }else if(direita > esquerda && abs(rssi_direita_array[ULTIMO-1]-direita) > DEADBAND){
    //angulo = (angulo-1)*SERVO_DIRECTION;

    float x = float(mediaDireita - mediaEsquerda) / 10;
    x = (1+ exp(-SIGMOID_SLOPE * x + SIGMOID_OFFSET));
    ang = x * SERVO_DIRECTION * -1;      
    
  }  
  
  Serial.print("Valor esquerda=");
  Serial.print(mediaEsquerda);
  Serial.print(", valor direita=");
  Serial.print(mediaDireita);
  Serial.print(", ang = ");
  Serial.print(ang);
  Serial.print(", angulo = ");
  Serial.println(angulo);
  
  mudaAngulo(ang,0);
}

/**
 * Metodo que muda o angulo do motor, conforme processamento.
 */
void mudaAngulo(int ang, int velocidade){
  //Segurança Servo  
  angulo += ang;
  angulo = constrain(angulo,SERVO_MIN,SERVO_MAX);  
  if(velocidade <= 0){
    velocidade = SERVO_SPEED;
  }
  //Velocidade Fixa
  servoP.write(angulo, velocidade, true);
  //delay(20);
}


/**
 * Metodo de calibração de RSSI
 * #Experimental
 */
 void calibraRSSI(int esquerda, int direita){
    //Verifica Direita.
    if(direita <= RSSI_MIN_d || RSSI_MIN_d == 0){
        RSSI_MIN_d = direita;
    }
    if(esquerda <= RSSI_MIN_e || RSSI_MIN_e == 0){
        RSSI_MIN_e = esquerda;
    }
    //Verifica Esquerda.
    if(direita >= RSSI_MAX_d || RSSI_MAX_d ==0){
      RSSI_MAX_d = direita;
    }
    if(esquerda >= RSSI_MAX_e || RSSI_MAX_e ==0){
      RSSI_MAX_e = esquerda;
    }
    //Verifica Superior.
    //#Future
 }

/**
 * Metodo que avança posição no array
 */
void avancaArray(uint16_t *posicoes, uint8_t n) {
  for (uint8_t i = 0; i < n - 1; i++) {
    posicoes[i] = posicoes[i + 1];
  }
}

/**
 * Metodo de calculo de media das medições rssi (ultimas 10).
 */
uint16_t media(uint16_t lista[], uint8_t n) {
  uint32_t soma = 0;
  for (uint8_t i = 0; i < n; i++) {
    soma += lista[i];
  }
  return uint16_t(soma / n);
}


