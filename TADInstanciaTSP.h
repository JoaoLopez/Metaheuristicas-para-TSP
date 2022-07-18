#ifndef TADInstanciaTSP
#define TADInstanciaTSP

#include "TADGrafo.h"
#include "TADPonto.h"
#include "TADTour.h"
#include "TADMetricas.h"
#include "TADErro.h"
#include <stdlib.h>
#include <string.h>

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
#include "ILS.h"

InstanciaTSP* criarInstanciaTSP(char* pathArquivoTSP, int* statusOperacao);

int carregarSolucaoInstanciaTSP(char* pathArquivoSolucao, InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximo(InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLados(InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLadosRandomizada(InstanciaTSP* instanciaTSP, double alpha);

int melhorarSolucaoInstanciaTSPBuscaLocalPrimeiroAprimorante(InstanciaTSP* instanciaTSP);

int melhorarSolucaoInstanciaTSPBuscaLocalMelhorAprimorante(InstanciaTSP* instanciaTSP);

int solucionarInstanciaTSPGRASP(InstanciaTSP* instanciaTSP, Metricas* metricas, int numeroRepeticoes, double alpha);

int solucionarInstanciaTSPILS(InstanciaTSP* instanciaTSP, Metricas* metricas, int numeroRepeticoes, double alpha);

int solucionarInstanciaTSPILSv2(InstanciaTSP* instanciaTSP, Metricas* metricas, int numeroRepeticoes, double alpha);

double calcularCustoSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP, NoTour* tour);

void atualizarCustoMelhorSolucaoInstanciaTSPMovimentoBuscaLocal(InstanciaTSP* instanciaTSP, double saldoNovoCusto);

void salvarInstanciaTSP(InstanciaTSP* instanciaTSP, FILE* arquivo);

void removerSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP);

void setMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP, NoTour* tour);

char* getNomeInstanciaTSP(InstanciaTSP* instanciaTSP);

double getCustoMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP);

VerticeGrafo* getGrafoInstanciaTSP(InstanciaTSP* instanciaTSP);

int getDimensaoInstanciaTSP(InstanciaTSP* instanciaTSP);

NoTour* getMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP);

void deletarInstanciaTSP(InstanciaTSP* instanciaTSP);

//////////////////DEBUG///////////////////////////////
void imprimirInstanciaTSP(InstanciaTSP* instanciaTSP);

#endif