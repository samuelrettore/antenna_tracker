#include "Arduino.h"
#include "Controle.h"
#include "Config.h"
#include <VarSpeedServo.h>

VarSpeedServo servoP;
//VarSpeedServo servoT;

/**
 * Metodo inicialização do modulo
 */
void Controle::IinicializaModulo(){
  Serial.begin(115200);
  Serial.println("Iniciou Setup");
  servoP.attach(SERVO_PAN);
  //servoT.attach(SERVO_TILT);
  mudaAngulo(SERVO_MIN,20);
  delay(100);
  mudaAngulo(SERVO_MAX,20);
  delay(100);
  mudaAngulo(angulo,20);
  delay(100);
  //Zera arrays  
  for (int i = 0; i < INICIO; i++) {
    rssi_esquerda_array[i] = 0;
    rssi_direita_array[i] = 0;
  }  
  Serial.println("Fim Setup");  
}

int Controle::getAngulo(){
  return angulo;
}

int Controle::getEsquerda(){
  return esquerda;
}

int Controle::getDireita(){
  return direita;
}

/**
 * Metodo que verifica entrada e controla rotação esquerda ou direita, 
 * conforme leitura de RSSI
 * #featura: também deve calcular velocidade de deslocamento.
 */
void Controle::verificaEntrada(){
  
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

  Serial.print("RSSI-esquerda = ");
  Serial.print(getEsquerda());
  Serial.print(",direita = ");
  Serial.print(getDireita());   
  Serial.print(", angulo calc = ");
  Serial.print(getAngulo());  
  Serial.print(", RSSI_MIN_e = ");
  Serial.print(RSSI_MIN_e);
  Serial.print(" / ");
  Serial.print(RSSI_MAX_e);
  Serial.print(", RSSI_MIN_D = ");
  Serial.print(RSSI_MIN_d);
  Serial.print(", / ");
  Serial.println(RSSI_MAX_d);

  
  Serial.print("Valor esquerda=");
  Serial.print(mediaEsquerda);
  Serial.print(", valor direita=");
  Serial.print(mediaDireita);
  Serial.print(", ang = ");
  Serial.print(ang);
  Serial.print(", angulo = ");
  Serial.println(getAngulo());
  
  mudaAngulo(ang,0);
}

/**
 * Metodo que avança posição no array
 */
void Controle::avancaArray(uint16_t *posicoes, uint8_t n) {
  for (uint8_t i = 0; i < n - 1; i++) {
    posicoes[i] = posicoes[i + 1];
  }
}


/**
 * Metodo que muda o angulo do motor, conforme processamento.
 */
void Controle::mudaAngulo(int ang, int velocidade){
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
 void Controle::calibraRSSI(int esquerda, int direita){
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
 * Metodo de calculo de media das medições rssi (ultimas 10).
 */
uint16_t Controle::media(uint16_t lista[], uint8_t n) {
  uint32_t soma = 0;
  for (uint8_t i = 0; i < n; i++) {
    soma += lista[i];
  }
  return uint16_t(soma / n);
}
