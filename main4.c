#ifndef MAIN4_C
#define MAIN4_C

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "TADErro.h"
#include "TADInstanciaTSP.h"
#include "util.h"

int main(int argc, char *argv[]){
    srand(time(NULL));

    InstanciaTSP* instanciaTSP = NULL;
    double segCPUInicial, segSistemaInicial, segCPUFinal, segSistemaFinal, tempoExecucao, tempoExecucaoPrevio;
    char nomeArquivo[100];
    FILE* arquivoSaida;
    int* statusOperacao = (int*) malloc(sizeof(int));
    if(statusOperacao == NULL){
        printf("ERRO: MEMÓRIA INSUFICIENTE!\n");
        return ERRO_MEMORIA_INSUFICIENTE;
    }

    int carregarSolucaoSalva = atoi(argv[1]);
    int numeroRepeticoes = atoi(argv[2]);
    double alpha = atof(argv[3]);

    for(int i = 4; i < argc; i++){

        printf("Experimento sendo processado: %s\n", argv[i]);
        
        //Criando instância de TSP
        strcpy(nomeArquivo, argv[i]);
        strcat(nomeArquivo, ".tsp");
        printf("Criando instância TSP armazenada em %s\n", nomeArquivo);
        instanciaTSP = criarInstanciaTSP(nomeArquivo, statusOperacao);
        if(*statusOperacao == ERRO_ABRIR_ARQUIVO)    printf("ERRO: ERRO AO ABRIR ARQUIVO!\n");
        if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE)    printf("ERRO: MEMÓRIA INSUFICIENTE!\n");
        if(*statusOperacao == ERRO_CRIAR_GRAFO)   printf("ERRO: ERRO AO CRIAR O GRAFO DA INSTÂNCIA DE TSP!\n");
        if(*statusOperacao != OK){
            int codigoErro = *statusOperacao;
            free(statusOperacao);
            return codigoErro;
        }

        //Carregando solução prévia, se o usuário requisitar
        if(carregarSolucaoSalva){
            strcpy(nomeArquivo, argv[i]);
            strcat(nomeArquivo, "GRASP");
            strcat(nomeArquivo, "_");
            strcat(nomeArquivo, argv[1]);
            strcat(nomeArquivo, "_");
            strcat(nomeArquivo, argv[3]);
            strcat(nomeArquivo, ".sol");
            printf("Carregando solução da instância TSP armazenada em %s\n", nomeArquivo);
            *statusOperacao = carregarSolucaoInstanciaTSP(nomeArquivo, instanciaTSP);
            if(*statusOperacao == ERRO_ABRIR_ARQUIVO)    printf("ERRO: ERRO AO ABRIR ARQUIVO!\n");
            if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE)    printf("ERRO: MEMÓRIA INSUFICIENTE!\n");
            if(*statusOperacao == ERRO_CARREGAR_SOLUCAO)    printf("ERRO: ERRO AO CARREGAR SOLUÇÃO!\n");
            if(*statusOperacao != OK){
                int codigoErro = *statusOperacao;
                free(statusOperacao);
                deletarInstanciaTSP(instanciaTSP);
                return codigoErro;
            }

            strcpy(nomeArquivo, argv[i]);
            strcat(nomeArquivo, "GRASP");
            strcat(nomeArquivo, "_");
            strcat(nomeArquivo, argv[1]);
            strcat(nomeArquivo, "_");
            strcat(nomeArquivo, argv[3]);
            strcat(nomeArquivo, ".tempo");
            FILE* arq = fopen(nomeArquivo, "rt");
            if(arq != NULL){
                fscanf(arq, "Tempo de execução: %lf", &tempoExecucaoPrevio);
            }
            else{
                printf("ERRO AO ABRIR ARQUIVO COM TEMPO DE EXECUÇÃO PRÉVIO!");
                free(statusOperacao);
                deletarInstanciaTSP(instanciaTSP);
                return ERRO_ABRIR_ARQUIVO;
            }
            fclose(arq);
        }

        //GRASP
        printf("Executando GRASP...\n");
        Tempo_CPU_Sistema(&segCPUInicial, &segSistemaInicial);
        *statusOperacao = solucionarInstanciaTSPGRASP(instanciaTSP, numeroRepeticoes - carregarSolucaoSalva, alpha);
        Tempo_CPU_Sistema(&segCPUFinal, &segSistemaFinal);
        tempoExecucao = segCPUFinal - segCPUInicial;

        if(carregarSolucaoSalva){
            tempoExecucao = tempoExecucao + tempoExecucaoPrevio;
        }

        if(*statusOperacao == ERRO_GERAR_SOLUCAO_INSTANCIA_TSP)    printf("ERRO: ERRO AO TENTAR GERAR UMA SOLUÇÃO DA INSTÂNCIA DE TSP!\n");
        if(*statusOperacao == ERRO_EXECUTAR_BUSCA_LOCAL_INSTANCIA_TSP)   printf("ERRO: ERRO AO EXECUTAR BUSCA LOCAL!\n");
        if(*statusOperacao != OK){
            int codigoErro = *statusOperacao;
            free(statusOperacao);
            deletarInstanciaTSP(instanciaTSP);
            return codigoErro;
        }

        //Salvando dados no arquivo de saída
        strcpy(nomeArquivo, argv[i]);
        strcat(nomeArquivo, "GRASP");
        strcat(nomeArquivo, "_");
        strcat(nomeArquivo, argv[2]);
        strcat(nomeArquivo, "_");
        strcat(nomeArquivo, argv[3]);
        strcat(nomeArquivo, ".txt");
        arquivoSaida = fopen(nomeArquivo, "wt");
        salvarInstanciaTSP(instanciaTSP, arquivoSaida);
        fprintf(arquivoSaida, "Tempo de execução: %lf", tempoExecucao);
        fclose(arquivoSaida);

        //Deletando instância de TSP
        deletarInstanciaTSP(instanciaTSP);
        instanciaTSP = NULL;
    }

    free(statusOperacao);
    return 0;
}

#endif