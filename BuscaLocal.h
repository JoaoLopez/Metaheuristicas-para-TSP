#ifndef BUSCALOCAL_H
#define BUSCALOCAL_H

/////DEBUG////////
#include <stdio.h>
///////////////////
#include "TADInstanciaTSP.h"
#include "TADTour.h"
#include "TADGrafo.h"
#include "TADErro.h"

int executarBuscaLocalPrimeiroAprimorante(InstanciaTSP* instanciaTSP);

int executarBuscaLocalMelhorAprimorante(InstanciaTSP* instanciaTSP);

#endif