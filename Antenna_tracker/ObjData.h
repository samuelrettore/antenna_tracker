#ifndef ObjData_h
#define ObjData_h

#include "Arduino.h"

class ObjData {
  public:
    int getEsquerda();
    int getDireita();
    int getPercentEsquerda();
    int getPercentDireita();
    void setEsquerda(int esquerda);
    void setDireita(int direita);
    void setPercentEsquerda(int percent);
    void setPercentDireita(int percent);
    
  private:
   int esquerda = 0;
   int direita = 0;
   int percent_esquerda = 0;
   int percent_direita = 0;
};

#endif
