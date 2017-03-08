#include "Arduino.h"
#include "ObjData.h"

int ObjData::getEsquerda(){
  return _esquerda;
}

int ObjData::getDireita(){
  return _direita;
}

int ObjData::getPercentEsquerda(){
  return _percent_esquerda;
}

int ObjData::getPercentDireita(){
  return _percent_direita;
}

void ObjData::setEsquerda(int esquerda){
  _esquerda = esquerda;
}

void ObjData::setDireita(int direita){
  _direita = direita;
}

void ObjData::setPercentEsquerda(int percent){
  _percent_esquerda = percent;
}


void ObjData::setPercentDireita(int percent){
  _percent_direita = percent;
}
