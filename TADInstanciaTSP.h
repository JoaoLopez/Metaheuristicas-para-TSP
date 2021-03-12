#ifndef TADInstanciaTSP
#define TADInstanciaTSP

#include "TADGrafo.h"
#include "TADPonto.h"
#include "TADTour.h"
#include "TADErro.h"
#include <stdlib.h>

typedef struct instanciaTSP{
    char* nome;
    char* comentario;
    char* tipo;
    int dimensao;
    char* tipoPesoAresta;
    VerticeGrafo* grafo;
    NoTour* melhorSolucao;
}InstanciaTSP;

#include "TADHeuristicas.h"

InstanciaTSP* criarInstanciaTSP(char* pathArquivoTSP, int* statusOperacao);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximo(InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLados(InstanciaTSP* instanciaTSP);

void deletarInstanciaTSP(InstanciaTSP* instanciaTSP);

int getDimensaoInstanciaTSP(InstanciaTSP* instanciaTSP);

VerticeGrafo* getGrafoInstanciaTSP(InstanciaTSP* instanciaTSP);

void setMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP, NoTour* tour);

#endif