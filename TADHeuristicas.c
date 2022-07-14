#include "TADHeuristicas.h"

int solucionarHeuristicaVizinhoMaisProximo(InstanciaTSP* instanciaTSP){
    int* statusOperacao = (int*) malloc(sizeof(int));
    if(statusOperacao == NULL){
        return ERRO_MEMORIA_INSUFICIENTE;
    }

    //Sorteando vértice do grafo pelo qual o algoritmo começará
    int numeroCidades = getDimensaoInstanciaTSP(instanciaTSP);
    int idCidadeInicio = sortearNumeroAleatorio(1, numeroCidades);

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

    //Recuperando grafo da instância de TSP e ordenando todas as suas
    //arestas em ordem crescente de peso
    VerticeGrafo* grafo = getGrafoInstanciaTSP(instanciaTSP);
    if(grafo == NULL){
        free(statusOperacao);
        deletarTour(solucaoInstanciaTSP);
        return ERRO_GRAFO_INSTANCIA_TSP_VAZIO;
    }

    ordenarPesosArestasGrafo(grafo);

    VerticeGrafo* verticeAtual = getVerticeGrafo(grafo, idCidadeInicio);
    VerticeVizinho* vizinhoAtual = verticeAtual->verticeVizinho;
    int cidadeInseridaSucesso = 0;
    for(int i = 0; i < numeroCidades-1; i++){
        cidadeInseridaSucesso = 0;
        
        while(vizinhoAtual != NULL){

            if(vizinhoAtual->id == idCidadeInicio){
                vizinhoAtual = vizinhoAtual->proximoVizinho;
                continue;
            }

            solucaoInstanciaTSP = inserirCidadeFimTour(vizinhoAtual->id, solucaoInstanciaTSP, statusOperacao);
            if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE){
                free(statusOperacao);
                deletarTour(solucaoInstanciaTSP);
                return ERRO_MEMORIA_INSUFICIENTE;
            }
            if(*statusOperacao == OK){
                cidadeInseridaSucesso = 1;

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

    free(statusOperacao);
    setMelhorSolucaoInstanciaTSP(instanciaTSP, solucaoInstanciaTSP);
    return OK;
}

int solucionarHeuristicaVizinhoMaisProximoDoisLados(InstanciaTSP* instanciaTSP){
    int numCidades = getDimensaoInstanciaTSP(instanciaTSP);
    VerticeGrafo* grafo = instanciaTSP->grafo;
    int statusOperacao, cidadeInicial, cidadeFinal, cidadesVisitadas[numCidades];
    
    //Ordenando arestas do grafo pelos pesos
    ordenarPesosArestasGrafo(grafo);

    //Inicializando a solução e incluindo a cidade de partida nela
    cidadeInicial = sortearNumeroAleatorio(1, numCidades);
    NoTour* solucao = criarTour();
    solucao = inserirCidadeFimTour(cidadeInicial, solucao, &statusOperacao);
    if(statusOperacao != OK){
        deletarTour(solucao);
        return statusOperacao;
    }
    cidadesVisitadas[0] = cidadeInicial;

    cidadeFinal = cidadeInicial;
    VerticeVizinho *aux1, *aux2;
    for(int i=1; i < numCidades; i++){
        aux1 = getVerticeGrafo(grafo, cidadeFinal)->verticeVizinho;        
        while(1){
            if(!elementoEstaVetor(aux1->id, cidadesVisitadas, i)){
                break;
            }
            aux1 = aux1->proximoVizinho;
        }
        aux2 = getVerticeGrafo(grafo, cidadeInicial)->verticeVizinho;
        while(1){
            if(!elementoEstaVetor(aux2->id, cidadesVisitadas, i)){
                break;
            }
            aux2 = aux2->proximoVizinho;
        }
        if(aux1->pesoAresta <= aux2->pesoAresta){ 
            solucao = inserirCidadeFimTour(aux1->id, solucao, &statusOperacao);
            cidadesVisitadas[i] =  aux1->id;
            cidadeFinal = aux1->id;
        }
        else{
            solucao = inserirCidadeInicioTour(aux2->id, solucao, &statusOperacao);
            cidadesVisitadas[i] =  aux2->id;
            cidadeInicial = aux2->id;
        }
        if(statusOperacao != OK){
            deletarTour(solucao);
            return statusOperacao;
        }
    }

    solucao = inserirCidadeFimTour(getIdTour(solucao), solucao, &statusOperacao);
    if(statusOperacao != OK){
        deletarTour(solucao);
        return statusOperacao;
    }

    removerSolucaoInstanciaTSP(instanciaTSP);
    setMelhorSolucaoInstanciaTSP(instanciaTSP, solucao);
    return OK;
}

int solucionarHeuristicaVizinhoMaisProximoDoisLadosRandomizada(InstanciaTSP* instanciaTSP, double alpha){
    int numCidades = getDimensaoInstanciaTSP(instanciaTSP);
    VerticeGrafo* grafo = instanciaTSP->grafo;
    int statusOperacao, cidadeInicial, cidadeFinal, cidadesVisitadas[numCidades];
    
    //Ordenando arestas do grafo pelos pesos
    ordenarPesosArestasGrafo(grafo);

    //Inicializando a solução e incluindo a cidade de partida nela
    cidadeInicial = sortearNumeroAleatorio(1, numCidades);
    NoTour* solucao = criarTour();
    solucao = inserirCidadeFimTour(cidadeInicial, solucao, &statusOperacao);
    if(statusOperacao != OK){
        deletarTour(solucao);
        return statusOperacao;
    }
    cidadesVisitadas[0] = cidadeInicial;

    cidadeFinal = cidadeInicial;
    int escolhasInicio[numCidades], numEscolhasInicio, escolhasFim[numCidades], numEscolhasFim;
    for(int i=1; i < numCidades; i++){
        //Obtendo possíveis cidades que podem ser inseridas (LCR) de acordo com o valor de alpha
        getVetorPossiveisCidadesEscolhidas(cidadeFinal, grafo, cidadesVisitadas, i, alpha, escolhasFim, &numEscolhasFim);
        getVetorPossiveisCidadesEscolhidas(cidadeInicial, grafo, cidadesVisitadas, i, alpha, escolhasInicio, &numEscolhasInicio);

        //Sorteando a cidade que será inserida e adicionando-a ao tour
        if(sortearNumeroAleatorio(0, 1)){
            int temp = sortearNumeroAleatorio(0, numEscolhasInicio-1);
            cidadeInicial = escolhasInicio[temp];
            solucao = inserirCidadeInicioTour(cidadeInicial, solucao, &statusOperacao);            
            cidadesVisitadas[i] = cidadeInicial;
        }
        else{
            int temp = sortearNumeroAleatorio(0, numEscolhasFim-1);
            cidadeFinal = escolhasFim[temp];
            solucao = inserirCidadeFimTour(cidadeFinal, solucao, &statusOperacao);            
            cidadesVisitadas[i] = cidadeFinal;
        }

        if(statusOperacao != OK){
            deletarTour(solucao);
            return statusOperacao;
        }
    }

    solucao = inserirCidadeFimTour(getIdTour(solucao), solucao, &statusOperacao);
    if(statusOperacao != OK){
        deletarTour(solucao);
        return statusOperacao;
    }

    removerSolucaoInstanciaTSP(instanciaTSP);
    setMelhorSolucaoInstanciaTSP(instanciaTSP, solucao);
    return OK;
}

void getVetorPossiveisCidadesEscolhidas(int cidade, VerticeGrafo* grafo, int* cidadesVisitadas, int numCidadesVisitadas, double alpha, int* possiveisCidadesEscolhidas, int* numPossiveisCidadesEscolhidas){
    ordenarPesosArestasGrafo(grafo);
    VerticeVizinho* aux1 = getVerticeGrafo(grafo, cidade)->verticeVizinho;
    VerticeVizinho* melhorVizinho = aux1;
    while(1){
        if(!elementoEstaVetor(melhorVizinho->id, cidadesVisitadas, numCidadesVisitadas)){
            break;
        }
        melhorVizinho = melhorVizinho->proximoVizinho;
    }
    
    *numPossiveisCidadesEscolhidas = 0;
    while(aux1 != NULL){
        if(!elementoEstaVetor(aux1->id, cidadesVisitadas, numCidadesVisitadas)){
            if(aux1->pesoAresta <= melhorVizinho->pesoAresta*(1 + alpha)){
                possiveisCidadesEscolhidas[*numPossiveisCidadesEscolhidas] = aux1->id;
                *numPossiveisCidadesEscolhidas += 1;
            }
            else{
                return;
            }
        }
        aux1 = aux1->proximoVizinho;
    }
}
