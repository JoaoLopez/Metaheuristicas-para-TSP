#ifndef TADHEURISTICAVIZINHOMAISPROXIMO_H
#define TADHEURISTICAVIZINHOMAISPROXIMO_H

#define OK 0
#define VERTICE_VIZINHO_E_NULL 1
#define VERTICE_VIZINHO_DEVE_SER_ATUALIZADO 2

/////////////DEBUG/////////
#include <stdio.h>
///////////////////////////
#include <stdlib.h>
#include "TADErro.h"
#include "TADInstanciaTSP.h"
#include "TADGrafo.h"
#include "TADTour.h"
#include "util.h"

int solucionarHeuristicaVizinhoMaisProximo(InstanciaTSP* instanciaTSP);

int solucionarHeuristicaVizinhoMaisProximoDoisLados(InstanciaTSP* instanciaTSP);

int verticeVizinhoPrecisaSerAtualizadoHeuristicaVizinhoMaisProximoDoisLados(VerticeVizinho* verticeVizinho, int idCidade);

#endif