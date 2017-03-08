#ifndef ObjData_h
#define ObjData_h

#include "Arduino.h"

class ObjData {
  public:
    ObjData(){
      _esquerda = 0;      
    }
    int getEsquerda();
    int getDireita();
    int getPercentEsquerda();
    int getPercentDireita();
    void setEsquerda(int esquerda);
    void setDireita(int direita);
    void setPercentEsquerda(int percent);
    void setPercentDireita(int percent);
    
  private:
    int _esquerda;
    int _direita;
    int _percent_esquerda;
    int _percent_direita;
};

#endif
