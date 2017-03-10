#ifndef Controle_h
#define Controle_h

#include "Arduino.h"
#include "ObjData.h"
#include "ObjMedia.h"

class Controle {
  public:

   void IinicializaModulo();
   void mudaAngulo(int ang, int velocidade, boolean calibrate);
   void verificaEntrada();

  private:
    void avancaArray(uint16_t *posicoes, uint8_t n);
    void avancaArrayObj(ObjData *objetos, uint8_t n);

    void calibraRSSI(ObjData obj);

    ObjMedia media(ObjData lista[], uint8_t n);
    int getAngulo();
    int getEsquerda();
    int getDireita();
    ObjData lePortaCalibra();
    void calibraInicio();

    int angulo = 90;
    int esquerda = 0, direita = 0;
    //Rssi Direita
    int RSSI_MIN_d = 0;
    int RSSI_MAX_d = 0;
    //Rssi Esquerda
    int RSSI_MIN_e = 0;
    int RSSI_MAX_e = 0;
    //Rssi Suprerior
    int RSSI_MIN_s = 0;
    int RSSI_MAX_s = 0;
};

#endif
