#ifndef MAIN_ILS_C
#define MAIN_ILS_C

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "TADErro.h"
#include "TADInstanciaTSP.h"
#include "util.h"
#include "TADMetricas.h"

void getNomeArqEntradaInst(char *nomeInstancia, char *arqEntrada){
    strcpy(arqEntrada, nomeInstancia);
    strcat(arqEntrada, ".tsp");
}

void getNomeArqSaidaInst(char *nomeInstancia, char *arqSaida){
    strcpy(arqSaida, nomeInstancia);
    strcat(arqSaida, "_Result_ILS.sol");
}

void getNomeArqMetricasInst(char *nomeInstancia, char *arqMetricas){
    strcpy(arqMetricas, nomeInstancia);
    strcat(arqMetricas, "_Metricas_ILS.sol");
}

InstanciaTSP* getInstanciaTSP(char* nomeInst, int carregarSolucaoSalva, int* statusOperacao){
    char arqEntrada[100], arqSaida[100];

    //Criando instância de TSP
    printf("Criando instância TSP...\n");
    getNomeArqEntradaInst(nomeInst, arqEntrada);
    InstanciaTSP* instanciaTSP = criarInstanciaTSP(arqEntrada, statusOperacao);
    if(*statusOperacao != OK)
        return NULL;

    //Carregando solução prévia, se o usuário requisitar
    if(carregarSolucaoSalva){
        printf("Carregando solução da instância TSP...\n");
        getNomeArqSaidaInst(nomeInst, arqSaida);
        *statusOperacao = carregarSolucaoInstanciaTSP(arqSaida, instanciaTSP);
        if(*statusOperacao != OK){
            deletarInstanciaTSP(instanciaTSP);
            return NULL;
        }
    }
    return instanciaTSP;
}

int main(int argc, char *argv[]){
    srand(time(NULL));

    InstanciaTSP* instanciaTSP = NULL;
    double segCPUInicial, segSistemaInicial, segCPUFinal, segSistemaFinal;
    char arqSaida[100], arqMetricas[100];
    FILE* arq;
    int statusOperacao;

    int carregarSolucaoSalva = atoi(argv[1]);
    int numeroRepeticoes = atoi(argv[2]);
    double alpha = atof(argv[3]);
    for(int i = 4; i < argc; i++){
        printf("Experimento sendo processado: %s\n", argv[i]);
        instanciaTSP = getInstanciaTSP(argv[i], carregarSolucaoSalva, &statusOperacao);
        if(statusOperacao == ERRO_ABRIR_ARQUIVO)    printf("ERRO: ERRO AO ABRIR ARQUIVO!\n");
        if(statusOperacao == ERRO_MEMORIA_INSUFICIENTE)    printf("ERRO: MEMÓRIA INSUFICIENTE!\n");
        if(statusOperacao == ERRO_CRIAR_GRAFO)   printf("ERRO: ERRO AO CRIAR O GRAFO DA INSTÂNCIA DE TSP!\n");
        if(statusOperacao != OK)
            return statusOperacao;

        //ILS
        printf("Executando ILS...\n");
        Metricas* metricas = criarMetricas();
        metricas->numSolucoesAntigas = numeroRepeticoes;
        metricas->custosSolucoesAntigas = (double*) malloc(numeroRepeticoes*sizeof(double));

        Tempo_CPU_Sistema(&segCPUInicial, &segSistemaInicial);
        statusOperacao = solucionarInstanciaTSPILS(instanciaTSP, metricas, numeroRepeticoes, alpha);
        Tempo_CPU_Sistema(&segCPUFinal, &segSistemaFinal);

        metricas->tempoExecucao = segCPUFinal - segCPUInicial;

        if(statusOperacao == ERRO_GERAR_SOLUCAO_INSTANCIA_TSP)    printf("ERRO: ERRO AO TENTAR GERAR UMA SOLUÇÃO DA INSTÂNCIA DE TSP!\n");
        if(statusOperacao == ERRO_EXECUTAR_BUSCA_LOCAL_INSTANCIA_TSP)   printf("ERRO: ERRO AO EXECUTAR BUSCA LOCAL!\n");
        if(statusOperacao != OK){
            deletarInstanciaTSP(instanciaTSP);
            free(metricas->custosSolucoesAntigas);
            return statusOperacao;
        }

        //Salvando dados e métricas em arquivos
        getNomeArqSaidaInst(argv[i], arqSaida);
        arq = fopen(arqSaida, "wt");
        salvarInstanciaTSP(instanciaTSP, arq);
        fclose(arq);
        getNomeArqMetricasInst(argv[i], arqMetricas);
        salvarMetricas(metricas, arqMetricas);

        //Deletando instância de TSP
        deletarInstanciaTSP(instanciaTSP);
        instanciaTSP = NULL;
    }

    return 0;
}

#endif