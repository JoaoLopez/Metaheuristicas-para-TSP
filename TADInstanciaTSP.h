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
#include "BuscaLocal.h"
#include "GRASP.h"

InstanciaTSP* criarInstanciaTSP(char* pathArquivoTSP, int* statusOperacao);

int carregarSolucaoInstanciaTSP(char* pathArquivoSolucao, InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximo(InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLados(InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLadosRandomizada(InstanciaTSP* instanciaTSP, double alpha);

int melhorarSolucaoInstanciaTSPBuscaLocalPrimeiroAprimorante(InstanciaTSP* instanciaTSP);

int melhorarSolucaoInstanciaTSPBuscaLocalMelhorAprimorante(InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPGRASP(InstanciaTSP* instanciaTSP, int numeroRepeticoes, double alpha);

void deletarInstanciaTSP(InstanciaTSP* instanciaTSP);

int getDimensaoInstanciaTSP(InstanciaTSP* instanciaTSP);

VerticeGrafo* getGrafoInstanciaTSP(InstanciaTSP* instanciaTSP);

void atualizarCustoMelhorSolucaoInstanciaTSPMovimentoBuscaLocal(InstanciaTSP* instanciaTSP, double saldoNovoCusto);

void apagarSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP);

void setMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP, NoTour* tour);

double calcularCustoSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP, NoTour* tour);

void salvarInstanciaTSP(InstanciaTSP* instanciaTSP, FILE* arquivo);

char* getNomeInstanciaTSP(InstanciaTSP* instanciaTSP);

double getCustoMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP);

NoTour* getMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP);

///////////////////////DEBUG///////////////////////////////
void imprimirInstanciaTSP(InstanciaTSP* instanciaTSP);

#endif