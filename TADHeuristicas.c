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


imprimirGrafo(grafo);



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


printf("CidadeInicial: %d CidadeFinal: %d\n", cidadeInicial, cidadeFinal);


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
    /*
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

    VerticeGrafo* verticeInicioTour = getVerticeGrafo(grafo, idCidadeInicio);
    VerticeGrafo* verticeFimTour = getVerticeGrafo(grafo, idCidadeInicio);
    VerticeVizinho* vizinhoInicioTour = verticeInicioTour->verticeVizinho;
    VerticeVizinho* vizinhoFimTour = verticeFimTour->verticeVizinho;
    int cidadeInseridaSucesso = 0, idCidadeSeraInserida = -1, insercaoCidadeInicioTour = -1;
    for(int i = 0; i < numeroCidades-1; i++){
        cidadeInseridaSucesso = 0;

        while(vizinhoInicioTour != NULL ||
        vizinhoFimTour != NULL){
            idCidadeSeraInserida = -1;
            insercaoCidadeInicioTour = -1;

            //Verificando necessidade de atualizar "VerticeVizinho*" para
            //que não haja risco de ao inserir uma cidade, crie-se um tour
            //sem utilizar todos os nós do grafo
            if(verticeVizinhoPrecisaSerAtualizadoHeuristicaVizinhoMaisProximoDoisLados(vizinhoInicioTour, verticeFimTour->id)){
                vizinhoInicioTour = vizinhoInicioTour->proximoVizinho;
                continue;
            }
            if(verticeVizinhoPrecisaSerAtualizadoHeuristicaVizinhoMaisProximoDoisLados(vizinhoFimTour, verticeInicioTour->id)){
                vizinhoFimTour = vizinhoFimTour->proximoVizinho;
                continue;
            }

            //Escolhendo qual será a próxima cidade a ser inserida no tour
            if(vizinhoInicioTour == NULL){
                idCidadeSeraInserida = vizinhoFimTour->id;
                insercaoCidadeInicioTour = 0;
            }
            if(vizinhoFimTour == NULL){
                idCidadeSeraInserida = vizinhoInicioTour->id;
                insercaoCidadeInicioTour = 1;
            }
            if(idCidadeSeraInserida == -1){
                if(vizinhoInicioTour->pesoAresta <= vizinhoFimTour->pesoAresta){
                    idCidadeSeraInserida = vizinhoInicioTour->id;
                    insercaoCidadeInicioTour = 1;
                }
                else{
                    idCidadeSeraInserida = vizinhoFimTour->id;
                    insercaoCidadeInicioTour = 0;
                }
            }

            //Tentando inserir cidade ao tour
            if(insercaoCidadeInicioTour){
                solucaoInstanciaTSP = inserirCidadeInicioTour(idCidadeSeraInserida, solucaoInstanciaTSP, statusOperacao);
            }
            else{
                solucaoInstanciaTSP = inserirCidadeFimTour(idCidadeSeraInserida, solucaoInstanciaTSP, statusOperacao);
            }

            if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE){
                free(statusOperacao);
                deletarTour(solucaoInstanciaTSP);
                return ERRO_MEMORIA_INSUFICIENTE;
            }
            if(*statusOperacao == OK){
                cidadeInseridaSucesso = 1;

                break;
            }

            if(insercaoCidadeInicioTour){
                vizinhoInicioTour = vizinhoInicioTour->proximoVizinho;
            }
            else{
                vizinhoFimTour = vizinhoFimTour->proximoVizinho;
            }
        }

        if(cidadeInseridaSucesso){
            if(insercaoCidadeInicioTour){
                //Não há risco de "getVerticeGrafo" retornar NULL pois
                //estamos buscando o vértice de destino de uma aresta que
                //se encontra no grafo
                verticeInicioTour = getVerticeGrafo(grafo, idCidadeSeraInserida);                
            }
            else{
                //Não há risco de "getVerticeGrafo" retornar NULL pois
                //estamos buscando o vértice de destino de uma aresta que
                //se encontra no grafo
                verticeFimTour = getVerticeGrafo(grafo, idCidadeSeraInserida);
            }
            
            vizinhoInicioTour = verticeInicioTour->verticeVizinho;
            vizinhoFimTour = verticeFimTour->verticeVizinho;
        }
        else{
            free(statusOperacao);
            deletarTour(solucaoInstanciaTSP);
            return ERRO_IMPOSSIVEL_GERAR_TOUR_GRAFO_INSTANCIA_TSP;
        }
    }

    //Inserindo última cidade ao tour (a cidade de início)
    solucaoInstanciaTSP = inserirCidadeFimTour(verticeInicioTour->id, solucaoInstanciaTSP, statusOperacao);
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
    */
}

int verticeVizinhoPrecisaSerAtualizadoHeuristicaVizinhoMaisProximoDoisLados(VerticeVizinho* verticeVizinho, int idCidade){
    if(verticeVizinho == NULL){
        return 0;
    }
    if(verticeVizinho->id == idCidade){
        return 1;
    }
    return 0;
}

int solucionarHeuristicaVizinhoMaisProximoDoisLadosRandomizada(InstanciaTSP* instanciaTSP, double alpha){
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

    VerticeGrafo* verticeInicioTour = getVerticeGrafo(grafo, idCidadeInicio);
    VerticeGrafo* verticeFimTour = getVerticeGrafo(grafo, idCidadeInicio);
    int idCidadeSeraInserida = -1, insercaoCidadeInicioTour = -1, numeroSorteado = -1;

    VerticeVizinho **vetorInicioTour[1] = {NULL}, **vetorFimTour[1] = {NULL};
    VerticeVizinho **possiveisCidadesInseridasInicioTour = NULL, **possiveisCidadesInseridasFimTour = NULL;
    int *tamanhoVetorInicioTour = (int*) malloc(sizeof(int));
    if(tamanhoVetorInicioTour == NULL){
        free(statusOperacao);
        deletarTour(solucaoInstanciaTSP);
        return ERRO_MEMORIA_INSUFICIENTE;
    }
    int *tamanhoVetorFimTour = (int*) malloc(sizeof(int));
    if(tamanhoVetorFimTour == NULL){
        free(tamanhoVetorInicioTour);
        free(statusOperacao);
        deletarTour(solucaoInstanciaTSP);
        return ERRO_MEMORIA_INSUFICIENTE;
    }
    int* vetorCidadesAindaNaoInseridasTour[1] = {NULL}, *cidadesAindaNaoInseridasTour = NULL;
    int* tamanhoCidadesAindaNaoInseridas = (int*) malloc(sizeof(int));
    if(tamanhoCidadesAindaNaoInseridas == NULL){
        free(tamanhoVetorInicioTour);
        free(tamanhoVetorFimTour);
        free(statusOperacao);
        deletarTour(solucaoInstanciaTSP);
        return ERRO_MEMORIA_INSUFICIENTE;
    }

    for(int i = 0; i < numeroCidades-1; i++){
        *statusOperacao = getCidadesAindaNaoInseridasTour(solucaoInstanciaTSP, grafo, vetorCidadesAindaNaoInseridasTour, tamanhoCidadesAindaNaoInseridas);
        if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE){
            free(tamanhoCidadesAindaNaoInseridas);
            free(tamanhoVetorInicioTour);
            free(tamanhoVetorFimTour);
            free(statusOperacao);
            deletarTour(solucaoInstanciaTSP);
            return ERRO_MEMORIA_INSUFICIENTE;
        }
        cidadesAindaNaoInseridasTour = vetorCidadesAindaNaoInseridasTour[0];

        *statusOperacao = criarVetoresPossiveisCidadesInseridas(verticeInicioTour, verticeFimTour, alpha, cidadesAindaNaoInseridasTour, *tamanhoCidadesAindaNaoInseridas, vetorInicioTour, tamanhoVetorInicioTour, vetorFimTour, tamanhoVetorFimTour);

        free(cidadesAindaNaoInseridasTour);
        cidadesAindaNaoInseridasTour = NULL;
        *tamanhoCidadesAindaNaoInseridas = -1;

        if(*statusOperacao == ERRO_VERTICES_INVALIDOS){
            free(tamanhoCidadesAindaNaoInseridas);
            free(tamanhoVetorInicioTour);
            free(tamanhoVetorFimTour);
            free(statusOperacao);
            deletarTour(solucaoInstanciaTSP);
            return ERRO_GERAR_VETORES_POSSIVEIS_CIDADES_INSERIDAS;
        }
        if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE){
            free(tamanhoCidadesAindaNaoInseridas);
            free(tamanhoVetorInicioTour);
            free(tamanhoVetorFimTour);
            free(statusOperacao);
            deletarTour(solucaoInstanciaTSP);
            return ERRO_MEMORIA_INSUFICIENTE;
        }
        possiveisCidadesInseridasInicioTour = vetorInicioTour[0];
        possiveisCidadesInseridasFimTour = vetorFimTour[0];

        //Testando se não há nenhuma cidade viável para ser incluída no tour
        if(*tamanhoVetorInicioTour == 0 && *tamanhoVetorFimTour == 0){
            free(tamanhoCidadesAindaNaoInseridas);
            free(possiveisCidadesInseridasInicioTour);
            free(possiveisCidadesInseridasFimTour);
            free(tamanhoVetorFimTour);
            free(tamanhoVetorInicioTour);
            free(statusOperacao);
            deletarTour(solucaoInstanciaTSP);
            return ERRO_IMPOSSIVEL_GERAR_TOUR_GRAFO_INSTANCIA_TSP;
        }

        numeroSorteado = sortearNumeroAleatorio(1, *tamanhoVetorInicioTour + *tamanhoVetorFimTour);
        if(numeroSorteado > *tamanhoVetorInicioTour){
            insercaoCidadeInicioTour = 0;
            idCidadeSeraInserida = possiveisCidadesInseridasFimTour[numeroSorteado - *tamanhoVetorInicioTour - 1]->id;
        }
        else{
            insercaoCidadeInicioTour = 1;
            idCidadeSeraInserida = possiveisCidadesInseridasInicioTour[numeroSorteado - 1]->id;
        }
        free(possiveisCidadesInseridasInicioTour);
        free(possiveisCidadesInseridasFimTour);
        possiveisCidadesInseridasInicioTour = NULL;
        *tamanhoVetorInicioTour = -1;
        possiveisCidadesInseridasFimTour = NULL;
        *tamanhoVetorFimTour = -1;

        //Inserindo cidade ao tour
        if(insercaoCidadeInicioTour){
            solucaoInstanciaTSP = inserirCidadeInicioTour(idCidadeSeraInserida, solucaoInstanciaTSP, statusOperacao);
        }
        else{
            solucaoInstanciaTSP = inserirCidadeFimTour(idCidadeSeraInserida, solucaoInstanciaTSP, statusOperacao);
        }
        if(*statusOperacao == ERRO_INSERCAO_CIDADE_TOUR_INVALIDA ||
        *statusOperacao == ERRO_MEMORIA_INSUFICIENTE){
            int codigoErro = *statusOperacao;
            free(tamanhoCidadesAindaNaoInseridas);
            free(tamanhoVetorFimTour);
            free(tamanhoVetorInicioTour);
            free(statusOperacao);
            deletarTour(solucaoInstanciaTSP);
            return codigoErro;
        }

        if(insercaoCidadeInicioTour){
            //Não há risco de "getVerticeGrafo" retornar NULL pois
            //estamos buscando o vértice de destino de uma aresta que
            //se encontra no grafo
            verticeInicioTour = getVerticeGrafo(grafo, idCidadeSeraInserida);                
        }
        else{
            //Não há risco de "getVerticeGrafo" retornar NULL pois
            //estamos buscando o vértice de destino de uma aresta que
            //se encontra no grafo
            verticeFimTour = getVerticeGrafo(grafo, idCidadeSeraInserida);
        }            
    }

    free(tamanhoCidadesAindaNaoInseridas);
    free(tamanhoVetorFimTour);
    free(tamanhoVetorInicioTour);

    //Inserindo última cidade ao tour (a cidade de início)
    solucaoInstanciaTSP = inserirCidadeFimTour(verticeInicioTour->id, solucaoInstanciaTSP, statusOperacao);
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
