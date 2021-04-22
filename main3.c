#ifndef MAIN3_C
#define MAIN3_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TADErro.h"
#include "TADInstanciaTSP.h"
#include "util.h"

int main(int argc, char *argv[]){
    InstanciaTSP* instanciaTSP = NULL;
    double segCPUInicial, segSistemaInicial, segCPUFinal, segSistemaFinal, tempoExecucao;
    char nomeArquivo[100], solucaoBase[100];
    FILE* arquivoSaida;
    int* statusOperacao = (int*) malloc(sizeof(int));
    if(statusOperacao == NULL){
        printf("ERRO: MEMÓRIA INSUFICIENTE!\n");
        return ERRO_MEMORIA_INSUFICIENTE;
    }

    for(int i = 1; i < argc; i++){
        
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

        for(int j = 0; j < 2; j++){
            if(j == 0)  strcpy(solucaoBase, "HVMP");
            if(j == 1)  strcpy(solucaoBase, "HVMPDL");
            //if(j == 2)  strcpy(solucaoBase, "HVMPDL_0.1_500");

            //Busca local primeiro aprimorante a partir da solução utilizando HVMP
            strcpy(nomeArquivo, argv[i]);
            strcat(nomeArquivo, solucaoBase);
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

            printf("Busca local primeiro aprimorante a partir da solução HVMP...\n");
            Tempo_CPU_Sistema(&segCPUInicial, &segSistemaInicial);
            *statusOperacao = melhorarSolucaoInstanciaTSPBuscaLocalPrimeiroAprimorante(instanciaTSP);
            Tempo_CPU_Sistema(&segCPUFinal, &segSistemaFinal);
            tempoExecucao = segCPUFinal - segCPUInicial;

            if(*statusOperacao == ERRO_RECUPERAR_NO_TOUR)    printf("ERRO: ERRO AO RECUPERAR NÓ DO TOUR!\n");
            if(*statusOperacao == ERRO_RECUPERAR_PESO_ARESTA_GRAFO)   printf("ERRO: ERRO AO RECUPERAR PESO DA ARESTA DO GRAFO!\n");
            if(*statusOperacao == ERRO_REALIZAR_MOVIMENTO_2_OTIMO)   printf("ERRO: ERRO REALIZAR MOVIMENTO 2-ÓTIMO!\n");
            if(*statusOperacao != OK){
                int codigoErro = *statusOperacao;
                free(statusOperacao);
                deletarInstanciaTSP(instanciaTSP);
                return codigoErro;
            }

            //Salvando dados no arquivo de saída
            strcpy(nomeArquivo, argv[i]);
            strcat(nomeArquivo, solucaoBase);
            strcat(nomeArquivo, "_PA.txt");
            arquivoSaida = fopen(nomeArquivo, "wt");
            salvarInstanciaTSP(instanciaTSP, arquivoSaida);
            fprintf(arquivoSaida, "Tempo de execução: %lf", tempoExecucao);
            fclose(arquivoSaida);

        
            //Busca local melhor aprimorante a partir da solução utilizando HVMP
            strcpy(nomeArquivo, argv[i]);
            strcat(nomeArquivo, solucaoBase);
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

            printf("Busca local melhor aprimorante a partir da solução HVMP...\n");
            Tempo_CPU_Sistema(&segCPUInicial, &segSistemaInicial);
            *statusOperacao = melhorarSolucaoInstanciaTSPBuscaLocalMelhorAprimorante(instanciaTSP);
            Tempo_CPU_Sistema(&segCPUFinal, &segSistemaFinal);
            tempoExecucao = segCPUFinal - segCPUInicial;

            if(*statusOperacao == ERRO_RECUPERAR_NO_TOUR)    printf("ERRO: ERRO AO RECUPERAR NÓ DO TOUR!\n");
            if(*statusOperacao == ERRO_RECUPERAR_PESO_ARESTA_GRAFO)   printf("ERRO: ERRO AO RECUPERAR PESO DA ARESTA DO GRAFO!\n");
            if(*statusOperacao == ERRO_REALIZAR_MOVIMENTO_2_OTIMO)   printf("ERRO: ERRO REALIZAR MOVIMENTO 2-ÓTIMO!\n");
            if(*statusOperacao != OK){
                int codigoErro = *statusOperacao;
                free(statusOperacao);
                deletarInstanciaTSP(instanciaTSP);
                return codigoErro;
            }

            //Salvando dados no arquivo de saída
            strcpy(nomeArquivo, argv[i]);
            strcat(nomeArquivo, solucaoBase);
            strcat(nomeArquivo, "_MA.txt");
            arquivoSaida = fopen(nomeArquivo, "wt");
            salvarInstanciaTSP(instanciaTSP, arquivoSaida);
            fprintf(arquivoSaida, "Tempo de execução: %lf", tempoExecucao);
            fclose(arquivoSaida);
        }


        //Deletando instância de TSP
        deletarInstanciaTSP(instanciaTSP);
        instanciaTSP = NULL;
    }

    free(statusOperacao);
    return 0;
}

#endif