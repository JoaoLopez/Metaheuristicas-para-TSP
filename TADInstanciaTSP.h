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
    double custoMelhorSolucao;
}InstanciaTSP;

#include "TADHeuristicas.h"

InstanciaTSP* criarInstanciaTSP(char* pathArquivoTSP, int* statusOperacao);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximo(InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLados(InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLadosRandomizada(InstanciaTSP* instanciaTSP, double alpha);

void deletarInstanciaTSP(InstanciaTSP* instanciaTSP);

int getDimensaoInstanciaTSP(InstanciaTSP* instanciaTSP);

VerticeGrafo* getGrafoInstanciaTSP(InstanciaTSP* instanciaTSP);

void setMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP, NoTour* tour);

void calcularCustoMelhorSolucaoInstanciaTSP(InstanciaTSP* InstanciaTSP);

void salvarInstanciaTSP(InstanciaTSP* instanciaTSP, FILE* arquivo);

char* getNomeInstanciaTSP(InstanciaTSP* instanciaTSP);

///////////////////////DEBUG///////////////////////////////
void imprimirInstanciaTSP(InstanciaTSP* instanciaTSP);

#endif