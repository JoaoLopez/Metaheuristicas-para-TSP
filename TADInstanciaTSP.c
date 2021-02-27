#include "TADInstanciaTSP.h"

InstanciaTSP* criarInstanciaTSP(char* pathArquivoTSP, int* statusOperacao){
    //Abrindo arquivo .tsp
    FILE* arquivoTSP = fopen(pathArquivoTSP, "rt");
    if(arquivoTSP == NULL){
        *statusOperacao = ERRO_ABRIR_ARQUIVO;
        return NULL;
    }

    //Alocando memória para a instância de TSP
    InstanciaTSP* instanciaTSP = (InstanciaTSP*) malloc(sizeof(InstanciaTSP));
    instanciaTSP->nome = (char*) malloc(20*sizeof(char));
    instanciaTSP->comentario = (char*) malloc(30*sizeof(char));
    instanciaTSP->tipo = (char*) malloc(4*sizeof(char));
    instanciaTSP->tipoPesoAresta = (char*) malloc(7*sizeof(char));
    
    char* temp = (char*) malloc(sizeof(char));
    
    //Verificando se todas as variáveis foram alocadas corretamente
    if(instanciaTSP == NULL){
        fclose(arquivoTSP);
        *statusOperacao = ERRO_MEMORIA_INSUFICIENTE;
        return NULL;
    }
    if(instanciaTSP->nome == NULL){
        free(instanciaTSP);
        fclose(arquivoTSP);
        *statusOperacao = ERRO_MEMORIA_INSUFICIENTE;
        return NULL;
    }
    if(instanciaTSP->comentario == NULL){
        free(instanciaTSP->nome);
        free(instanciaTSP);
        fclose(arquivoTSP);
        *statusOperacao = ERRO_MEMORIA_INSUFICIENTE;
        return NULL;
    }
    if(instanciaTSP->tipo == NULL){
        free(instanciaTSP->comentario);
        free(instanciaTSP->nome);
        free(instanciaTSP);
        fclose(arquivoTSP);
        *statusOperacao = ERRO_MEMORIA_INSUFICIENTE;
        return NULL;
    }
    if(instanciaTSP->tipoPesoAresta == NULL){
        free(instanciaTSP->tipo);
        free(instanciaTSP->comentario);
        free(instanciaTSP->nome);
        free(instanciaTSP);
        fclose(arquivoTSP);
        *statusOperacao = ERRO_MEMORIA_INSUFICIENTE;
        return NULL;
    }
    if(temp == NULL){
        free(instanciaTSP->tipoPesoAresta);
        free(instanciaTSP->tipo);
        free(instanciaTSP->comentario);
        free(instanciaTSP->nome);
        free(instanciaTSP);
        fclose(arquivoTSP);
        *statusOperacao = ERRO_MEMORIA_INSUFICIENTE;
        return NULL;
    }

    //Lendo dados do arquivo
    fscanf(arquivoTSP, "NAME : %s\n", instanciaTSP->nome);
    fscanf(arquivoTSP, "COMMENT : %s\n", instanciaTSP->comentario);
    fscanf(arquivoTSP, "TYPE : %s\n", instanciaTSP->tipo);
    fscanf(arquivoTSP, "DIMENSION : %d\n", &(instanciaTSP->dimensao));
    fscanf(arquivoTSP, "EDGE_WEIGHT_TYPE : %s\n", instanciaTSP->tipoPesoAresta);
    fscanf(arquivoTSP, "%s\n", temp);

    free(temp);

    //Criando grafo da instância
    instanciaTSP->grafo = criarGrafo();

    //Lendo dados do arquivo e instanciando os pontos em "vetorPontos"
    int idVertice;
    double posicaoX, posicaoY;
    Ponto* vetorPontos[instanciaTSP->dimensao];
    for(int i = 0; i < instanciaTSP->dimensao; i++){
        fscanf(arquivoTSP, "%d %lf %lf", &idVertice, &posicaoX, &posicaoY);
        Ponto* novoPonto = criarPonto(posicaoX, posicaoY, statusOperacao);
        if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE){
            deletarGrafo(instanciaTSP->grafo);
            free(instanciaTSP->tipoPesoAresta);
            free(instanciaTSP->tipo);
            free(instanciaTSP->comentario);
            free(instanciaTSP->nome);
            free(instanciaTSP);
            fclose(arquivoTSP);

            for(int j = 0; j < i; j++){
                deletarPonto(vetorPontos[j]);
            }

            return NULL;
        }
        vetorPontos[i] = novoPonto;
        
        instanciaTSP->grafo = inserirVertice(instanciaTSP->grafo, idVertice, statusOperacao);
        if(*statusOperacao == ERRO_MEMORIA_INSUFICIENTE){
            deletarGrafo(instanciaTSP->grafo);
            free(instanciaTSP->tipoPesoAresta);
            free(instanciaTSP->tipo);
            free(instanciaTSP->comentario);
            free(instanciaTSP->nome);
            free(instanciaTSP);
            fclose(arquivoTSP);

            for(int j = 0; j <= i; j++){
                deletarPonto(vetorPontos[j]);
            }

            return NULL;
        }
    }

    //Fechando arquivo .tsp
    fclose(arquivoTSP);

    //Calculando as distâncias entre cada par de pontos
    //Adicionando os vértices e as arestas ao grafo
    Ponto *ponto1, *ponto2;
    double distancia;
    for(int i = 0; i < instanciaTSP->dimensao; i++){
        for(int j = 0; j < instanciaTSP->dimensao; j++){
            if(i != j){
                ponto1 = vetorPontos[i];
                ponto2 = vetorPontos[j];
                distancia = calcularDistanciaPontos(ponto1, ponto2);

                printf("i, j, distancia: %d, %d, %lf\n", i, j, distancia);

                *statusOperacao = inserirAresta(instanciaTSP->grafo, i+1, j+1, distancia);
                if(*statusOperacao != OK){
                    if(*statusOperacao != ERRO_MEMORIA_INSUFICIENTE){
                        printf("*statusOperacao = %d", *statusOperacao);
                        *statusOperacao = ERRO_CRIAR_GRAFO;
                    }

                    for(int k = 0; k < instanciaTSP->dimensao; k++){
                        deletarPonto(vetorPontos[k]);
                    }
                    deletarGrafo(instanciaTSP->grafo);
                    free(instanciaTSP->tipoPesoAresta);
                    free(instanciaTSP->tipo);
                    free(instanciaTSP->comentario);
                    free(instanciaTSP->nome);
                    free(instanciaTSP);
                    return NULL;
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////
    printf("instanciaTSP->nome: %s\n", instanciaTSP->nome);
    printf("instanciaTSP->comentario: %s\n", instanciaTSP->comentario);
    printf("instanciaTSP->tipo: %s\n", instanciaTSP->tipo);
    printf("instanciaTSP->dimensao: %d\n", instanciaTSP->dimensao);
    printf("instanciaTSP->tipoPesoAresta: %s\n", instanciaTSP->tipoPesoAresta);
    printf("instanciaTSP->grafo:\n");
    imprimirGrafo(instanciaTSP->grafo);
    ///////////////////////////////////////////////////
    
    for(int i = 0; i < instanciaTSP->dimensao; i++){
        deletarPonto(vetorPontos[i]);
    }

    *statusOperacao = OK;
    return instanciaTSP;
}

/*
void solucionarInstanciaGRASP(InstanciaTSP* instanciaTSP);
*/

void deletarInstanciaTSP(InstanciaTSP* instanciaTSP){
    deletarTour(instanciaTSP->melhorSolucao);
    deletarGrafo(instanciaTSP->grafo);
    free(instanciaTSP->tipoPesoAresta);
    free(instanciaTSP->tipo);
    free(instanciaTSP->comentario);
    free(instanciaTSP->nome);
    free(instanciaTSP);
    return;
}