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
//Rssi Direita
int RSSI_MIN_d = 0;
int RSSI_MAX_d = 0;
//Rssi Esquerda
int RSSI_MIN_e = 0;
int RSSI_MAX_e = 0;
//Rssi Suprerior
int RSSI_MIN_s = 0;
int RSSI_MAX_s = 0;
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

//Direção Normal 0, invertido -1.
#define SERVO_DIRECTION     1

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


#define INICIO  10
#define ULTIMO  INICIO - 1

uint16_t rssi_esquerda_array[INICIO];
uint16_t rssi_direita_array[INICIO];

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

  //Zera arrays  
  for (int i = 0; i < INICIO; i++) {
    rssi_esquerda_array[i] = 0;
    rssi_direita_array[i] = 0;
  }  
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
  Serial.print(", RSSI_MIN_e = ");
  Serial.print(RSSI_MIN_e);
  Serial.print(", RSSI_MAX_e = ");
  Serial.print(RSSI_MAX_e);
  Serial.print(", RSSI_MIN_D = ");
  Serial.print(RSSI_MIN_d);
  Serial.print(", RSSI_MAX_D = ");
  Serial.println(RSSI_MAX_d);
  
  //delay(80);   
}

/**
 * Metodo que verifica entrada e controla rotação esquerda ou direita, 
 * conforme leitura de RSSI
 * #featura: também deve calcular velocidade de deslocamento.
 */
void verificaEntrada(){
  //Le dados da porta
  esquerda = analogRead(RSSI_esquerda);
  direita = analogRead(RSSI_direita);
  
  //Desloca Array
  avancaArray(rssi_esquerda_array, INICIO);
  avancaArray(rssi_direita_array, INICIO);

  //Copia Para array
  rssi_esquerda_array[ULTIMO] = esquerda;
  rssi_direita_array[ULTIMO] = direita;
  
  //esquerda = random(RSSI_MIN, RSSI_MAX);
  //direita = random(RSSI_MIN, RSSI_MAX);  
  //Pode calibrar pelo array
  calibraRSSI(esquerda,direita);
  
  if(esquerda > direita && abs(esquerda_old-esquerda) > DEADBAND){
    angulo = (angulo+1)*SERVO_DIRECTION;
    esquerda_old = esquerda;
    
  }else if(direita > esquerda && abs(direita_old-direita) > DEADBAND){
    angulo = (angulo-1)*SERVO_DIRECTION * -1;
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

