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




int criarVetoresPossiveisCidadesInseridas(VerticeGrafo* verticeInicioTour, VerticeGrafo* verticeFimTour, double alpha, int* cidadesAindaNaoInseridasTour, int tamanhoVetorCidadesAindaNaoInseridas, VerticeVizinho*** possiveisCidadesInseridasInicioTour, int* tamanhoVetorInicioTour, VerticeVizinho*** possiveisCidadesInseridasFimTour, int* tamanhoVetorFimTour){
    if(verticeInicioTour == NULL && verticeFimTour == NULL){
        *tamanhoVetorInicioTour = -1;
        *tamanhoVetorFimTour = -1;
        possiveisCidadesInseridasInicioTour[0] = NULL;
        possiveisCidadesInseridasFimTour[0] = NULL;
        return ERRO_VERTICES_INVALIDOS;
    }
    if(verticeFimTour == NULL){
        return criarVetoresPossiveisCidadesInseridas(verticeInicioTour, verticeInicioTour, alpha, cidadesAindaNaoInseridasTour, tamanhoVetorCidadesAindaNaoInseridas, possiveisCidadesInseridasInicioTour, tamanhoVetorInicioTour, possiveisCidadesInseridasFimTour, tamanhoVetorFimTour);
    }
    if(verticeInicioTour == NULL){
        return criarVetoresPossiveisCidadesInseridas(verticeFimTour, verticeFimTour, alpha, cidadesAindaNaoInseridasTour, tamanhoVetorCidadesAindaNaoInseridas, possiveisCidadesInseridasInicioTour, tamanhoVetorInicioTour, possiveisCidadesInseridasFimTour, tamanhoVetorFimTour);
    }

    //1º Adicionando todas as arestas que podem ser utilizadas na próxima inserção
    //tanto para o "possiveisCidadesInseridasInicioTour" como para "possiveisCidadesInseridasFimTour"
    VerticeVizinho* vizinhoAtual = verticeInicioTour->verticeVizinho;
    VerticeVizinho** aux = NULL;
    possiveisCidadesInseridasInicioTour[0] = NULL;
    *tamanhoVetorInicioTour = 0;
    while(vizinhoAtual != NULL){
        if(elementoEstaVetor(vizinhoAtual->id, cidadesAindaNaoInseridasTour, tamanhoVetorCidadesAindaNaoInseridas)){
            aux = (VerticeVizinho**) malloc((*tamanhoVetorInicioTour + 1) * sizeof(VerticeVizinho*));
            if(aux == NULL){
                if(possiveisCidadesInseridasInicioTour[0] != NULL){
                    free(possiveisCidadesInseridasInicioTour[0]);
                }
                *tamanhoVetorInicioTour = -1;
                *tamanhoVetorFimTour = -1;
                possiveisCidadesInseridasInicioTour[0] = NULL;
                possiveisCidadesInseridasFimTour[0] = NULL;
                return ERRO_MEMORIA_INSUFICIENTE;
            }

            for(int i = 0; i < *tamanhoVetorInicioTour; i++){
                aux[i] = possiveisCidadesInseridasInicioTour[0][i];
            }
            aux[*tamanhoVetorInicioTour] = vizinhoAtual;

            if(possiveisCidadesInseridasInicioTour[0] != NULL){
                free(possiveisCidadesInseridasInicioTour[0]);
            }
            possiveisCidadesInseridasInicioTour[0] = aux;
            *tamanhoVetorInicioTour = *tamanhoVetorInicioTour + 1;
        }
        vizinhoAtual = vizinhoAtual->proximoVizinho;
    }
    
    possiveisCidadesInseridasFimTour[0] = NULL;
    *tamanhoVetorFimTour = 0;
    if(verticeInicioTour != verticeFimTour){
        vizinhoAtual = verticeFimTour->verticeVizinho;
        while(vizinhoAtual != NULL){
            if(elementoEstaVetor(vizinhoAtual->id, cidadesAindaNaoInseridasTour, tamanhoVetorCidadesAindaNaoInseridas)){
                aux = (VerticeVizinho**) malloc((*tamanhoVetorFimTour + 1) * sizeof(VerticeVizinho*));
                if(aux == NULL){
                    if(possiveisCidadesInseridasInicioTour[0] != NULL){
                        free(possiveisCidadesInseridasInicioTour[0]);
                    }
                    if(possiveisCidadesInseridasFimTour[0] != NULL){
                        free(possiveisCidadesInseridasFimTour[0]);
                    }
                    *tamanhoVetorInicioTour = -1;
                    *tamanhoVetorFimTour = -1;
                    possiveisCidadesInseridasInicioTour[0] = NULL;
                    possiveisCidadesInseridasFimTour[0] = NULL;
                    return ERRO_MEMORIA_INSUFICIENTE;
                }

                for(int i = 0; i < *tamanhoVetorFimTour; i++){
                    aux[i] = possiveisCidadesInseridasFimTour[0][i];
                }
                aux[*tamanhoVetorFimTour] = vizinhoAtual;

                if(possiveisCidadesInseridasFimTour[0] != NULL){
                    free(possiveisCidadesInseridasFimTour[0]);
                }
                possiveisCidadesInseridasFimTour[0] = aux;
                *tamanhoVetorFimTour = *tamanhoVetorFimTour + 1;
            }
            vizinhoAtual = vizinhoAtual->proximoVizinho;
        }
    }

    //2º Buscando maior e menor peso dentre as arestas de
    //"possiveisCidadesInseridasInicioTour" e "possiveisCidadesInseridasFimTour"
    double custoMinimo = -1, custoMaximo = -1;
    for(int i = 0; i < *tamanhoVetorInicioTour; i++){
        if(possiveisCidadesInseridasInicioTour[0][i]->pesoAresta > custoMaximo){
            custoMaximo = possiveisCidadesInseridasInicioTour[0][i]->pesoAresta;
        }
        if(possiveisCidadesInseridasInicioTour[0][i]->pesoAresta < custoMinimo ||
        custoMinimo == -1){
            custoMinimo = possiveisCidadesInseridasInicioTour[0][i]->pesoAresta;
        }
    }
    for(int i = 0; i < *tamanhoVetorFimTour; i++){
        if(possiveisCidadesInseridasFimTour[0][i]->pesoAresta > custoMaximo){
            custoMaximo = possiveisCidadesInseridasFimTour[0][i]->pesoAresta;
        }
        if(possiveisCidadesInseridasFimTour[0][i]->pesoAresta < custoMinimo ||
        custoMinimo == -1){
            custoMinimo = possiveisCidadesInseridasFimTour[0][i]->pesoAresta;
        }
    }

    //3º Retirando arestas que possuem peso fora dos padrão estipulado por "alpha"
    for(int i = 0; i < *tamanhoVetorInicioTour; i++){
        if(possiveisCidadesInseridasInicioTour[0][i]->pesoAresta > custoMinimo + alpha*(custoMaximo - custoMinimo)){
            if(*tamanhoVetorInicioTour == 1){
                free(possiveisCidadesInseridasInicioTour[0]);
                possiveisCidadesInseridasInicioTour[0] = NULL;
                *tamanhoVetorInicioTour = 0;
            }
            else{
                aux = (VerticeVizinho**) malloc((*tamanhoVetorInicioTour - 1) * sizeof(VerticeVizinho*));
                if(aux == NULL){
                    free(possiveisCidadesInseridasInicioTour[0]);
                    if(possiveisCidadesInseridasFimTour[0] != NULL){
                        free(possiveisCidadesInseridasFimTour[0]);
                    }
                    *tamanhoVetorInicioTour = -1;
                    *tamanhoVetorFimTour = -1;
                    possiveisCidadesInseridasInicioTour[0] = NULL;
                    possiveisCidadesInseridasFimTour[0] = NULL;
                    return ERRO_MEMORIA_INSUFICIENTE;
                }

                int k = 0;
                for(int j = 0; j < *tamanhoVetorInicioTour; j++){
                    if(i == j){
                        continue;
                    }
                    aux[k] = possiveisCidadesInseridasInicioTour[0][j];
                    k++;
                }

                free(possiveisCidadesInseridasInicioTour[0]);
                possiveisCidadesInseridasInicioTour[0] = aux;
                *tamanhoVetorInicioTour = *tamanhoVetorInicioTour - 1;
            }
            i = -1;
        }
    }

    for(int i = 0; i < *tamanhoVetorFimTour; i++){
        if(possiveisCidadesInseridasFimTour[0][i]->pesoAresta > custoMinimo + alpha*(custoMaximo - custoMinimo)){
            if(*tamanhoVetorFimTour == 1){
                free(possiveisCidadesInseridasFimTour[0]);
                possiveisCidadesInseridasFimTour[0] = NULL;
                *tamanhoVetorFimTour = 0;
            }
            else{
                aux = (VerticeVizinho**) malloc((*tamanhoVetorFimTour - 1) * sizeof(VerticeVizinho*));
                if(aux == NULL){
                    if(possiveisCidadesInseridasInicioTour[0] != NULL){
                        free(possiveisCidadesInseridasInicioTour[0]);
                    }
                    free(possiveisCidadesInseridasFimTour[0]);
                    *tamanhoVetorInicioTour = -1;
                    *tamanhoVetorFimTour = -1;
                    possiveisCidadesInseridasInicioTour[0] = NULL;
                    possiveisCidadesInseridasFimTour[0] = NULL;
                    return ERRO_MEMORIA_INSUFICIENTE;
                }

                int k = 0;
                for(int j = 0; j < *tamanhoVetorFimTour; j++){
                    if(i == j){
                        continue;
                    }
                    aux[k] = possiveisCidadesInseridasFimTour[0][j];
                    k++;
                }

                free(possiveisCidadesInseridasFimTour[0]);
                possiveisCidadesInseridasFimTour[0] = aux;
                *tamanhoVetorFimTour = *tamanhoVetorFimTour - 1;
            }
            i = -1;
        }
    }
    
    return OK;
}

int getCidadesAindaNaoInseridasTour(NoTour* tour, VerticeGrafo* grafo, int** cidadesAindaNaoInseridasTour, int* tamanhoVetor){
    if(grafo == NULL){
        cidadesAindaNaoInseridasTour[0] = NULL;
        *tamanhoVetor = 0;
        return OK;
    }

    //Adicionando todas as cidades do grafo a "cidadesAindaNaoInseridasTour"
    cidadesAindaNaoInseridasTour[0] = NULL;
    *tamanhoVetor = 0;
    int* aux = NULL;
    while(grafo != NULL){
        aux = (int*) malloc((*tamanhoVetor + 1) * sizeof(int));
        if(aux == NULL){
            if(cidadesAindaNaoInseridasTour[0] != NULL){
                free(cidadesAindaNaoInseridasTour[0]);
            }
            cidadesAindaNaoInseridasTour[0] = NULL;
            *tamanhoVetor = -1;
            return ERRO_MEMORIA_INSUFICIENTE;
        }
        
        for(int i = 0; i < *tamanhoVetor; i++){
            aux[i] = cidadesAindaNaoInseridasTour[0][i];
        }
        aux[*tamanhoVetor] = grafo->id;
        
        if(cidadesAindaNaoInseridasTour[0] != NULL){
            free(cidadesAindaNaoInseridasTour[0]);
        }
        cidadesAindaNaoInseridasTour[0] = aux;
        *tamanhoVetor = *tamanhoVetor + 1;

        grafo = grafo->proximoVerticeGrafo;
    }

    //Retirando cidades que já se encontram no tour de "cidadesAindaNaoInseridasTour"
    if(tour == NULL){
        return OK;
    }

    while(tour != NULL){
        if(elementoEstaVetor(tour->idCidade, cidadesAindaNaoInseridasTour[0], *tamanhoVetor)){
            if(*tamanhoVetor == 1){
                free(cidadesAindaNaoInseridasTour[0]);
                cidadesAindaNaoInseridasTour[0] = NULL;
                *tamanhoVetor = 0;
                return OK;
            }
            else{
                aux = (int*) malloc((*tamanhoVetor - 1) * sizeof(int));
                if(aux == NULL){
                    free(cidadesAindaNaoInseridasTour[0]);
                    cidadesAindaNaoInseridasTour[0] = NULL;
                    *tamanhoVetor = -1;
                    return ERRO_MEMORIA_INSUFICIENTE;
                }
                
                int k = 0;
                for(int i = 0; i < *tamanhoVetor; i++){
                    if(cidadesAindaNaoInseridasTour[0][i] == tour->idCidade){
                        continue;
                    }
                    aux[k] = cidadesAindaNaoInseridasTour[0][i];
                    k++;
                }
                
                free(cidadesAindaNaoInseridasTour[0]);
                cidadesAindaNaoInseridasTour[0] = aux;
                *tamanhoVetor = *tamanhoVetor - 1;
            }
        }

        tour = tour->proximoNoTour;
    }

    return OK;
}
