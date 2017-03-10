#ifndef ObjMedia_h
#define ObjMedia_h

#include "Arduino.h"

class ObjMedia {
  public:
    uint32_t getEsquerdaMedia();
    uint32_t getDireitaMedia();
    void setEsquerdaMedia(uint32_t esquerda);
    void setDireitaMedia(uint32_t direita);

  private:
    uint32_t _esquerda_media = 0 ;
    uint32_t _direita_media = 0;
};

#endif
