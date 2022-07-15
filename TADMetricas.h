#ifndef METRICAS_H
#define METRICAS_H

/////DEBUG////////
#include <stdio.h>
///////////////////
#include <stdlib.h>
#include "TADTour.h"
#include "TADGrafo.h"
#include "TADErro.h"

typedef struct metricas{
    int numSolucoesAntigas;
    double *custosSolucoesAntigas;
    double tempoExecucao;
}Metricas;

#include "TADInstanciaTSP.h"

Metricas* criarMetricas();
void salvarMetricas(Metricas* metricas, char* nomeArq);
void carregarMetricas(Metricas* metricas, char* nomeArq);
int getNumSolucoesAntigas(Metricas* metricas);
double* getCustosSolucoesAntigas(Metricas* metricas);
double getTempoExecucao(Metricas* metricas);
void setNumSolucoesAntigas(Metricas* metricas, int numSolucoes);
void setCustosSolucoesAntigas(Metricas* metricas, int pos, double custo);
void setTempoExecucao(Metricas* metricas, double TempoExecucao);
void imprimirMetricas(Metricas* metricas);

#endif