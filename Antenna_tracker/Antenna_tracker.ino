/**
 * Projeto desenvolvido por Samuel Rettore e Hugo Almeida
 * Baseado no projeto https://youtu.be/aACTKziuDVE
 * Antenna Tracker baseada em Arduino e RSSI 
 */
#include <Servo.h>

/*
 * Configuracao RSSI
 * Portas de leitura das anteas RSSI
 * Niveis de RSSI para Calibração
 */
#define RSSI_direita 0
#define RSSI_esquerda 1
#define RSSI_vertical 2

#define RSSI_MIN 85
#define RSSI_MAX 244

/**
 * Valor usado para nao varir todo montendo somente 
 * quando firerenca de leitura maior que 5.
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
#define SERVO_SPEED 5

Servo servoP;
Servo servoT;

int angulo = (SERVO_MAX/2);
//Variaveis
int esquerda = 0, direita = 0;
int esquerda_old = 0, direita_old = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciou Setup");
  servoP.attach(SERVO_PAN);
  servoT.attach(SERVO_TILT);
  
  servoP.write(SERVO_MIN);
  servoT.write(SERVO_MIN);
  delay(1000);
  servoP.write(SERVO_MAX);
  servoT.write(SERVO_MAX);
  delay(1000);
  //Midle
  servoP.write((SERVO_MAX/2));
  servoT.write((SERVO_MAX/2));
  Serial.println("Fim Setup");
}

void loop() {
  verificaEntrada();
  Serial.print("RSSI // esquerda = ");
  Serial.print(esquerda);
  Serial.print(", direita = ");
  Serial.print(direita); 
  
  Serial.print(", angulo calculado = ");
  Serial.println(angulo);  
  delay(80);   
}

void verificaEntrada(){
  esquerda = analogRead(RSSI_esquerda);
  direita = analogRead(RSSI_direita);
  //esquerda = random(RSSI_MIN, RSSI_MAX);
  //direita = random(RSSI_MIN, RSSI_MAX);  
  if(esquerda > direita && abs(esquerda_old-esquerda) > DEADBAND){
    angulo = angulo+SERVO_SPEED;
    esquerda_old = esquerda;
  }else if(direita > esquerda && abs(direita_old-direita) > DEADBAND){
    angulo = angulo-SERVO_SPEED;
    direita_old = direita;
  }
  //Segurança Servo  
  angulo = constrain(angulo,SERVO_MIN,SERVO_MAX);
  servoP.write(angulo);
}

