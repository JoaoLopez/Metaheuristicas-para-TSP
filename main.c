#ifndef MAIN_C
#define MAIN_C

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
    double segCPUInicial, segSistemaInicial, segCPUFinal, segSistemaFinal, tempoExecucao;
    char nomeArquivoSaida[100];
    FILE* arquivoSaida;
    int* statusOperacao = (int*) malloc(sizeof(int));
    if(statusOperacao == NULL){
        printf("ERRO: MEMÓRIA INSUFICIENTE!\n");
        return ERRO_MEMORIA_INSUFICIENTE;
    }

    for(int i = 1; i < argc; i++){
        //Criando instância de TSP
        printf("Arquivo sendo processado: %s\n", argv[i]);
        instanciaTSP = criarInstanciaTSP(argv[i], statusOperacao);
        if(*statusOperacao == ERRO_ABRIR_ARQUIVO)    printf("ERRO: ERRO AO ABRIR ARQUIVO!\n");
        if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE)    printf("ERRO: MEMÓRIA INSUFICIENTE!\n");
        if(*statusOperacao == ERRO_CRIAR_GRAFO)   printf("ERRO: ERRO AO CRIAR O GRAFO DA INSTÂNCIA DE TSP!\n");
        if(*statusOperacao != OK){
            int codigoErro = *statusOperacao;
            free(statusOperacao);
            return codigoErro;
        }


        //Heurística do Vizinho Mais Próximo
        printf("Solucionando instância pela heurística do vizinho mais próximo...\n");
        Tempo_CPU_Sistema(&segCPUInicial, &segSistemaInicial);
        *statusOperacao = solucionarInstanciaTSPHeuristicaVizinhoMaisProximo(instanciaTSP);
        Tempo_CPU_Sistema(&segCPUFinal, &segSistemaFinal);
        tempoExecucao = segCPUFinal - segCPUInicial;

        if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE)    printf("ERRO: MEMÓRIA INSUFICIENTE!\n");
        if(*statusOperacao == ERRO_INSERCAO_CIDADE_TOUR_INVALIDA)   printf("ERRO: ERRO AO INSERIR CIDADE NO TOUR!\n");
        if(*statusOperacao == ERRO_GRAFO_INSTANCIA_TSP_VAZIO)   printf("ERRO: NÃO HÁ GRAFO DEFINIDO PARA A INSTÂNCIA DE TSP!\n");
        if(*statusOperacao == ERRO_IMPOSSIVEL_GERAR_TOUR_GRAFO_INSTANCIA_TSP)   printf("ERRO: IMPOSSÍVEL GERAR UM TOUR NO GRAFO DA INSTÂNCIA DE TSP!\n");
        if(*statusOperacao != OK){
            deletarInstanciaTSP(instanciaTSP);
            int codigoErro = *statusOperacao;
            free(statusOperacao);
            return codigoErro;
        }

        //Salvando dados no arquivo de saída
        strcpy(nomeArquivoSaida, getNomeInstanciaTSP(instanciaTSP));
        strcat(nomeArquivoSaida, "HVMP.txt");
        arquivoSaida = fopen(nomeArquivoSaida, "wt");
        salvarInstanciaTSP(instanciaTSP, arquivoSaida);
        fprintf(arquivoSaida, "Tempo de execução: %lf", tempoExecucao);
        fclose(arquivoSaida);


        //Heurística do Vizinho Mais Próximo Pelos Dois Lados
        printf("Solucionando instância pela heurística do vizinho mais próximo pelos dois lados...\n");
        Tempo_CPU_Sistema(&segCPUInicial, &segSistemaInicial);
        *statusOperacao = solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLados(instanciaTSP);
        Tempo_CPU_Sistema(&segCPUFinal, &segSistemaFinal);
        tempoExecucao = segCPUFinal - segCPUInicial;

        if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE)    printf("ERRO: MEMÓRIA INSUFICIENTE!\n");
        if(*statusOperacao == ERRO_INSERCAO_CIDADE_TOUR_INVALIDA)   printf("ERRO: ERRO AO INSERIR CIDADE NO TOUR!\n");
        if(*statusOperacao == ERRO_GRAFO_INSTANCIA_TSP_VAZIO)   printf("ERRO: NÃO HÁ GRAFO DEFINIDO PARA A INSTÂNCIA DE TSP!\n");
        if(*statusOperacao == ERRO_IMPOSSIVEL_GERAR_TOUR_GRAFO_INSTANCIA_TSP)   printf("ERRO: IMPOSSÍVEL GERAR UM TOUR NO GRAFO DA INSTÂNCIA DE TSP!\n");
        if(*statusOperacao != OK){
            deletarInstanciaTSP(instanciaTSP);
            int codigoErro = *statusOperacao;
            free(statusOperacao);
            return codigoErro;
        }

        //Salvando dados no arquivo de saída
        strcpy(nomeArquivoSaida, getNomeInstanciaTSP(instanciaTSP));
        strcat(nomeArquivoSaida, "HVMPDL.txt");
        arquivoSaida = fopen(nomeArquivoSaida, "wt");
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