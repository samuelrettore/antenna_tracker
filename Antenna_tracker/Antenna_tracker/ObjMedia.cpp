#include "Arduino.h"
#include "ObjMedia.h"

uint32_t ObjMedia::getEsquerdaMedia(){
  return _esquerda_media;
}
uint32_t ObjMedia::getDireitaMedia(){
  return _direita_media;
}
void ObjMedia::setEsquerdaMedia(uint32_t esquerda){
  _esquerda_media = esquerda;
}
void ObjMedia::setDireitaMedia(uint32_t direita){
  _direita_media = direita;
}
