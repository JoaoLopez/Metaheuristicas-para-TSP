#include "TADHeuristicas.h"

int sortearNumeroAleatorio(int valorMinimo, int valorMaximo){
    return (rand() % (valorMaximo - valorMinimo + 1)) + valorMinimo;
}

int solucionarHeuristicaVizinhoMaisProximo(InstanciaTSP* instanciaTSP){
    int* statusOperacao = (int*) malloc(sizeof(int));
    if(statusOperacao == NULL){
        return ERRO_MEMORIA_INSUFICIENTE;
    }

    //Sorteando vértice do grafo pelo qual o algoritmo começará
    int numeroCidades = getDimensaoInstanciaTSP(instanciaTSP);
    int idCidadeInicio = sortearNumeroAleatorio(1, numeroCidades);
    
    //////DEBUG////////
    printf("CIDADE INICIAL SORTEADA: %d\n", idCidadeInicio);
    //////////////////


    //Adicionando vértice à solução
    NoTour* solucaoInstanciaTSP = criarTour();
    solucaoInstanciaTSP = inserirCidadeFimTour(idCidadeInicio, solucaoInstanciaTSP, statusOperacao);
    if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE ||
    *statusOperacao == ERRO_INSERCAO_CIDADE_TOUR_INVALIDA){
        int codigoErro = *statusOperacao;
        free(statusOperacao);
        deletarTour(solucaoInstanciaTSP);
        return codigoErro;
    }


    //////DEBUG////////
    printf("Imprimindo tour:\n");
    imprimirTour(solucaoInstanciaTSP);
    printf("\n");
    //////////////////


    //Recuperando grafo da instância de TSP e ordenando todas as suas
    //arestas em ordem crescente de peso
    VerticeGrafo* grafo = getGrafoInstanciaTSP(instanciaTSP);
    if(grafo == NULL){
        free(statusOperacao);
        deletarTour(solucaoInstanciaTSP);
        return ERRO_GRAFO_INSTANCIA_TSP_VAZIO;
    }

    /////////DEBUG///////////////
    printf("ORDENANDO GRAFO...\n");
    /////////////////////////////

    ordenarPesosArestasGrafo(grafo);

    /////////DEBUG///////////////
    printf("IMPRIMINDO GRAFO ORDENADO:\n");
    imprimirGrafo(grafo);
    printf("\n");
    /////////////////////////////

    VerticeGrafo* verticeAtual = getVerticeGrafo(grafo, idCidadeInicio);
    VerticeVizinho* vizinhoAtual = verticeAtual->verticeVizinho;
    int cidadeInseridaSucesso = 0;
    for(int i = 0; i < numeroCidades-1; i++){
        cidadeInseridaSucesso = 0;

        printf("VerticeAtual: %d\n", verticeAtual->id);

        while(vizinhoAtual != NULL){

            if(vizinhoAtual->id == idCidadeInicio){
                vizinhoAtual = vizinhoAtual->proximoVizinho;
                continue;
            }

            printf("VizinhoAtual: %d\n", vizinhoAtual->id);

            solucaoInstanciaTSP = inserirCidadeFimTour(vizinhoAtual->id, solucaoInstanciaTSP, statusOperacao);
            if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE){
                free(statusOperacao);
                deletarTour(solucaoInstanciaTSP);
                return ERRO_MEMORIA_INSUFICIENTE;
            }
            if(*statusOperacao == OK){
                cidadeInseridaSucesso = 1;

                printf("CIDADE INSERIDA COM SUCESSO!\n");
                printf("Imprimindo Tour:\n");
                imprimirTour(solucaoInstanciaTSP);
                printf("\n");

                break;
            }

            vizinhoAtual = vizinhoAtual->proximoVizinho;
        }

        if(cidadeInseridaSucesso){
            //Não há risco de "getVerticeGrafo" retornar NULL pois
            //estamos buscando o vértice de destino de uma aresta que
            //se encontra no grafo
            verticeAtual = getVerticeGrafo(grafo, vizinhoAtual->id);
            vizinhoAtual = verticeAtual->verticeVizinho;
        }
        else{
            free(statusOperacao);
            deletarTour(solucaoInstanciaTSP);
            return ERRO_IMPOSSIVEL_GERAR_TOUR_GRAFO_INSTANCIA_TSP;
        }
    }

    //Inserindo última cidade ao tour (a cidade de início)
    solucaoInstanciaTSP = inserirCidadeFimTour(idCidadeInicio, solucaoInstanciaTSP, statusOperacao);
    if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE ||
    *statusOperacao == ERRO_INSERCAO_CIDADE_TOUR_INVALIDA){
        int codigoErro = *statusOperacao;
        free(statusOperacao);
        deletarTour(solucaoInstanciaTSP);
        return codigoErro;
    }


    printf("##### TOUR FINAL #####\n");
    imprimirTour(solucaoInstanciaTSP);
    printf("\n");


    free(statusOperacao);
    setMelhorSolucaoInstanciaTSP(instanciaTSP, solucaoInstanciaTSP);
    return OK;
}
