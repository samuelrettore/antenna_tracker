#include "Arduino.h"
#include "ObjData.h"

int ObjData::getEsquerda(){
  return esquerda;
}

int ObjData::getDireita(){
  return direita;
}

int ObjData::getPercentEsquerda(){
  return percent_esquerda;  
}

int ObjData::getPercentDireita(){
  return percent_direita;
}

void ObjData::setEsquerda(int esquerda){
  esquerda = esquerda;
}

void ObjData::setDireita(int direita){
  direita = direita;
}

void ObjData::setPercentEsquerda(int percent){
    percent_esquerda = percent;
}


void ObjData::setPercentDireita(int percent){
  percent_direita = percent;
}

