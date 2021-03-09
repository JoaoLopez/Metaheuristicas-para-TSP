#ifndef TADHEURISTICAVIZINHOMAISPROXIMO_H
#define TADHEURISTICAVIZINHOMAISPROXIMO_H

/////////////DEBUG/////////
#include <stdio.h>
///////////////////////////
#include <stdlib.h>
#include "TADErro.h"
#include "TADInstanciaTSP.h"
#include "TADGrafo.h"
#include "TADTour.h"

int sortearNumeroAleatorio(int valorMinimo, int valorMaximo);

int solucionarHeuristicaVizinhoMaisProximo(InstanciaTSP* instanciaTSP);

#endif