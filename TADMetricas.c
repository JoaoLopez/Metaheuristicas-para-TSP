#include "TADMetricas.h"

Metricas* criarMetricas(){
    Metricas* metricas = (Metricas*) malloc(sizeof(Metricas));
    metricas->tempoExecucao = 0;
    metricas->numSolucoesAntigas = 0;
    metricas->custosSolucoesAntigas = NULL;
    return metricas;
}

void salvarMetricas(Metricas* metricas, char* nomeArq){
    FILE* arq = fopen(nomeArq, "wt");
    fprintf(arq, "Tempo de Execução: %lf\n", metricas->tempoExecucao);
    fprintf(arq, "Número de Soluções Antigas: %d\n", metricas->numSolucoesAntigas);
    fprintf(arq, "Custo das Soluções Antigas:\n");
    for(int i=0; i < metricas->numSolucoesAntigas; i++)
        fprintf(arq, "%lf\n", metricas->custosSolucoesAntigas[i]);
    fclose(arq);
}

void carregarMetricas(Metricas* metricas, char* nomeArq){
    FILE* arq = fopen(nomeArq, "rt");
    fscanf(arq, "Tempo de Execução: %lf\n", &metricas->tempoExecucao);
    fscanf(arq, "Número de Soluções Antigas: %d\n", &metricas->numSolucoesAntigas);

    metricas->custosSolucoesAntigas = (double*) malloc(metricas->numSolucoesAntigas*sizeof(double));
    fscanf(arq, "Custo das Soluções Antigas:\n");
    for(int i=0; i < metricas->numSolucoesAntigas; i++)
        fscanf(arq, "%lf\n", &(metricas->custosSolucoesAntigas[i]));
    fclose(arq);
}

int getNumSolucoesAntigas(Metricas* metricas){
    return metricas->numSolucoesAntigas;
}

double* getCustosSolucoesAntigas(Metricas* metricas){
    return metricas->custosSolucoesAntigas;
}

double getTempoExecucao(Metricas* metricas){
    return metricas->tempoExecucao;
}

void setNumSolucoesAntigas(Metricas* metricas, int numSolucoes){
    metricas->numSolucoesAntigas = numSolucoes;
}

void setCustosSolucoesAntigas(Metricas* metricas, int pos, double custo){
    metricas->custosSolucoesAntigas[pos] = custo;
}

void setTempoExecucao(Metricas* metricas, double TempoExecucao){
    metricas->tempoExecucao = TempoExecucao;
}

void imprimirMetricas(Metricas* metricas){
    printf("Métricas\n");
    printf("Tempo de Execução: %lf\n", metricas->tempoExecucao);
    printf("Número de Soluções Antigas: %d\n", metricas->numSolucoesAntigas);
    printf("Custos das Soluções Antigas:\n");
    for(int i=0; i < metricas->numSolucoesAntigas; i++)
        printf("    %lf\n", metricas->custosSolucoesAntigas[i]);
}