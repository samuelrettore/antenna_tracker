/**
 * Projeto desenvolvido por Samuel Rettore e Hugo Almeida
 * Baseado no projeto https://youtu.be/aACTKziuDVE
 * Antenna Tracker baseada em Arduino e RSSI
 *
 * ==== Configuração ===========
 *
 */

/*
 * Configuracao RSSI
 * Portas de leitura das anteas RSSI
 * Niveis de cd ..RSSI para Calibração
 */
#define RSSI_direita 0
#define RSSI_esquerda 1
//#Future
#define RSSI_vertical 2

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

/**
 * Velocidade do servo
 * Valores de 1 a 255
 */
#define SERVO_SPEED 60

//Direção Normal 0, invertido -1.
#define SERVO_DIRECTION     1

/*
 * Define Minimo e maximo do servo
 */
#define SERVO_MAX 180
#define SERVO_MIN 13

/**
 * Alterado para variaveis
 * Calibracao automatica
 */
//#define RSSI_MIN 85
//#define RSSI_MAX 244

//Tamanho Array maximo
#define INICIO  10
//Inicio Array De dados
#define ULTIMO  INICIO - 1

/**
 * Variaveis usadas na biblioteca padrão.
 */
#define SIGMOID_SLOPE       1
#define SIGMOID_OFFSET 4
