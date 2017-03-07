
/**
 * Projeto desenvolvido por Samuel Rettore e Hugo Almeida
 * Baseado no projeto https://youtu.be/aACTKziuDVE
 * Antenna Tracker baseada em Arduino e RSSI 
 */
#include "Controle.h"

Controle ctrl;

void setup() {
  ctrl.IinicializaModulo();
}

/**
 * Loop principal
 */
void loop() {
  ctrl.verificaEntrada();  
}

