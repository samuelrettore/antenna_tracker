/**
 * Projeto desenvolvido por Samuel Rettore e Hugo Almeida
 * Baseado no projeto https://youtu.be/aACTKziuDVE
 * Antenna Tracker baseada em Arduino e RSSI 
 */
#include <VarSpeedServo.h>

/*
 * Configuracao RSSI
 * Portas de leitura das anteas RSSI
 * Niveis de RSSI para Calibração
 */
#define RSSI_direita 0
#define RSSI_esquerda 1
#define RSSI_vertical 2


/**
 * Alterado para variaveis
 * Calibracao automatica
 */
//#define RSSI_MIN 85
//#define RSSI_MAX 244
int RSSI_MIN = 0;
int RSSI_MAX = 0;

/**
 * Valor usado para nao varrer todo montendo informações de leitura, somente
 * quando diferencia de leitura maior que o parametro.
 */
#define DEADBAND 5

/*
 * Configura motores Servos .
 * Portas Servo Motor
 */
#define SERVO_PAN 5
#define SERVO_TILT 6

/*
 * Define Minimo e maximo do servo
 */
#define SERVO_MAX 180
#define SERVO_MIN 13

VarSpeedServo servoP;
//VarSpeedServo servoT;

int angulo = (SERVO_MAX/2);
//Variaveis
int esquerda = 0, direita = 0;
int esquerda_old = 0, direita_old = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciou Setup");
  servoP.attach(SERVO_PAN);
  //servoT.attach(SERVO_TILT);
  //Gira esquerda  
  mudaAngulo(SERVO_MIN);
  //servoT.write(SERVO_MIN, 30, true);
  //Gira direita
  mudaAngulo(SERVO_MAX);
  //servoT.write(SERVO_MAX, 30, true);  
  //Seta Meio
  mudaAngulo((SERVO_MAX/2));
  //servoT.write((SERVO_MAX/2));
  Serial.println("Fim Setup");
}

/**
 * Loop principal
 */
void loop() {
  verificaEntrada();
  Serial.print("RSSI // esquerda = ");
  Serial.print(esquerda);
  Serial.print(", direita = ");
  Serial.print(direita);   
  Serial.print(", angulo calculado = ");
  Serial.print(angulo);  
  Serial.print(", RSSI_MIN = ");
  Serial.print(RSSI_MIN);
  Serial.print(", RSSI_MAX = ");
  Serial.println(RSSI_MAX);
  //delay(80);   
}

/**
 * Metodo que verifica entrada e controla rotação esquerda ou direita, 
 * conforme leitura de RSSI
 * #featura: também deve calcular velocidade de deslocamento.
 */
void verificaEntrada(){
  esquerda = analogRead(RSSI_esquerda);
  direita = analogRead(RSSI_direita);
  //esquerda = random(RSSI_MIN, RSSI_MAX);
  //direita = random(RSSI_MIN, RSSI_MAX);  
  calibraRSSI(esquerda,direita);
  
  if(esquerda > direita && abs(esquerda_old-esquerda) > DEADBAND){
    angulo = angulo+1;
    esquerda_old = esquerda;
  }else if(direita > esquerda && abs(direita_old-direita) > DEADBAND){
    angulo = angulo-1;
    direita_old = direita;
  }  
  mudaAngulo(angulo);
}

/**
 * Metodo que muda o angulo do motor, conforme processamento.
 */
void mudaAngulo(int ang){
  //Segurança Servo  
  ang = constrain(ang,SERVO_MIN,SERVO_MAX);
  angulo = ang;
  //Velocidade Fixa
  servoP.write(ang, 30, true);
}


/**
 * Metodo de calibração de RSSI
 * #Experimental
 */
 void calibraRSSI(int vl1, int vl2){
    if(vl1 <= RSSI_MIN || RSSI_MIN == 0){
        RSSI_MIN = vl1;
    }
    if(vl2 <= RSSI_MIN || RSSI_MIN == 0){
        RSSI_MIN = vl2;
    }
    if(vl1>= RSSI_MAX || RSSI_MAX ==0){
      RSSI_MAX = vl1;
    }
    if(vl2 >= RSSI_MAX || RSSI_MAX ==0){
      RSSI_MAX = vl2;
    }
 }

