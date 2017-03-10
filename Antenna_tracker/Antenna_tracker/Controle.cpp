#include "Arduino.h"
#include "Controle.h"
#include "Config.h"
#include "ObjData.h"
#include "ObjMedia.h"
#include <VarSpeedServo.h>

VarSpeedServo servoP;
//VarSpeedServo servoT;

ObjData rssi_array_dados[INICIO];

/**
 * Metodo inicialização do modulo
 */
void Controle::IinicializaModulo(){
  Serial.begin(115200);
  Serial.println("Iniciou Setup");
  servoP.attach(SERVO_PAN);
  //servoT.attach(SERVO_TILT);

  calibraInicio();

  //Zera arrays
  /*for (int i = 0; i < INICIO; i++) {
    rssi_esquerda_array[i] = 0;
    rssi_direita_array[i] = 0;
  }*/
  Serial.println("Fim Setup");
}

/*int Controle::getAngulo(){
  return angulo;
}

int Controle::getEsquerda(){
  return esquerda;
}

int Controle::getDireita(){
  return direita;
}*/

/*
 * Calibração inicial.
 */
 void Controle::calibraInicio(){
  //Move Servo para posição Minima
  Serial.print("1 - Angulo para = ");
  Serial.println(SERVO_MIN);
  mudaAngulo(SERVO_MIN,40, true);
  lePortaCalibra();
  delay(1000);

  //Move Servo para posição Maxima
  Serial.print("2 - Angulo para = ");
  Serial.println(SERVO_MAX);
  mudaAngulo(SERVO_MAX,40,true);
  lePortaCalibra();
  delay(1000);

  Serial.print("3 - Angulo para = ");
  Serial.println("90");
  //Move Servo para Centro.
  mudaAngulo(90,40, true);
  lePortaCalibra();
  delay(1000);
 }

/**
 * Metodo que efetua leitura de dados e calibração do RSSI.
 */
ObjData Controle::lePortaCalibra(){
  //esquerda = random(85, 244);
  //esquerda=0;
  //direita = random(85, 244);
  //direita=0;

  //Le dados da porta
  esquerda = analogRead(RSSI_esquerda);
  direita = analogRead(RSSI_direita);

  //Desloca Array de Objetos
  avancaArrayObj(rssi_array_dados, INICIO);

  //Cria Objeto
  ObjData obj;

  //Preenche objetos
  obj.setEsquerda(esquerda);
  obj.setDireita(direita);
  //Calcula Percentuais.
  obj.setPercentEsquerda(map(esquerda,RSSI_MIN_e,RSSI_MAX_e,0,100));
  obj.setPercentDireita(map(direita,RSSI_MIN_d,RSSI_MAX_d,0,100));

  //Atribui Objeto ao array
  rssi_array_dados[ULTIMO] = obj;

  //Pode calibrar pelo array
  calibraRSSI(obj);
  return obj;
}

/**
 * Metodo que verifica entrada e controla rotação esquerda ou direita,
 * conforme leitura de RSSI
 * #featura: também deve calcular velocidade de deslocamento.
 */
void Controle::verificaEntrada(){

  //Le dados da porta
  ObjData obj_lido = lePortaCalibra();

  ObjMedia objmed = media(rssi_array_dados, INICIO);
  uint16_t mediaEsquerda = max(objmed.getEsquerdaMedia(), RSSI_MIN_e);
  uint16_t mediaDireita = max(objmed.getDireitaMedia(), RSSI_MIN_d);

  //Experimental
  float ang = 0;
  //Verifica se diferença da media maior que DEADBAND
  if(abs(mediaEsquerda - mediaDireita) > DEADBAND){
    if(mediaEsquerda > mediaDireita){
      //OldCode
      //angulo = (angulo+1)*SERVO_DIRECTION;

      float x = float(mediaEsquerda - mediaDireita) / 10;
      x = (1+ exp(-SIGMOID_SLOPE * x + SIGMOID_OFFSET));
      ang = x * SERVO_DIRECTION;


    }else if(mediaDireita > mediaEsquerda){
      //OldCode
      //angulo = (angulo-1)*SERVO_DIRECTION;

      float x = float(mediaDireita - mediaEsquerda) / 10;
      x = (1+ exp(-SIGMOID_SLOPE * x + SIGMOID_OFFSET));
      ang = x * SERVO_DIRECTION * -1;
    }
  }

  Serial.print("RSSI esquerda=");
  Serial.print(obj_lido.getEsquerda());
  Serial.print("/");
  Serial.print(obj_lido.getPercentEsquerda());
  Serial.print("% ,direita=");
  Serial.print(obj_lido.getDireita());
  Serial.print("/");
  Serial.print(obj_lido.getPercentDireita());
  Serial.print("%, angulo calc = ");
  Serial.print(angulo);
  Serial.print(", RSSI_MIN_e = ");
  Serial.print(RSSI_MIN_e);
  Serial.print(" / ");
  Serial.print(RSSI_MAX_e);
  Serial.print(", RSSI_MIN_d = ");
  Serial.print(RSSI_MIN_d);
  Serial.print(", / ");
  Serial.println(RSSI_MAX_d);
  mudaAngulo(ang,0, false);
}

/**
 * Metodo que avança posição no array
 */
void Controle::avancaArrayObj(ObjData *objetos, uint8_t n) {
  for (uint8_t i = 0; i < n - 1; i++) {
    objetos[i] = objetos[i + 1];
  }
}


/**
 * Metodo que muda o angulo do motor, conforme processamento.
 */
void Controle::mudaAngulo(int ang, int velocidade, boolean calibrate){
  //Segurança Servo
  if(calibrate == false){
    angulo += ang;
  }else{
      angulo = ang;
  }
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
 void Controle::calibraRSSI(ObjData obj){
    //Verifica Direita.
    if(obj.getDireita() <= RSSI_MIN_d || RSSI_MIN_d == 0){
        RSSI_MIN_d = obj.getDireita();
    }
    if(obj.getEsquerda() <= RSSI_MIN_e || RSSI_MIN_e == 0){
        RSSI_MIN_e = obj.getEsquerda();
    }
    //Verifica Direita.
    if(obj.getDireita() >= RSSI_MAX_d || RSSI_MAX_d ==0){
      RSSI_MAX_d = obj.getDireita();
    }
    if(obj.getEsquerda() >= RSSI_MAX_e || RSSI_MAX_e ==0){
      RSSI_MAX_e =obj.getEsquerda();
    }
    //Verifica Superior.
    //#Future
 }



/**
 * Metodo de calculo de media das medições rssi (ultimas 10).
 * OO
 */
ObjMedia Controle::media(ObjData lista[], uint8_t n) {
  ObjMedia obj_med;
  for (uint8_t i = 0; i < n; i++) {    
    obj_med.setDireitaMedia(obj_med.getDireitaMedia()+lista[i].getDireita());
    obj_med.setEsquerdaMedia(obj_med.getEsquerdaMedia()+lista[i].getEsquerda());
  }
  obj_med.setDireitaMedia(obj_med.getDireitaMedia()/n);
  obj_med.setEsquerdaMedia(obj_med.getEsquerdaMedia()/n);
  return obj_med;
}
