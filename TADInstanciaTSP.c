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
    instanciaTSP->comentario = (char*) malloc(50*sizeof(char));
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

                *statusOperacao = inserirAresta(instanciaTSP->grafo, i+1, j+1, distancia);
                if(*statusOperacao != OK){
                    if(*statusOperacao != ERRO_MEMORIA_INSUFICIENTE){
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

    instanciaTSP->melhorSolucao = NULL;
    instanciaTSP->custoMelhorSolucao = -1;
    
    for(int i = 0; i < instanciaTSP->dimensao; i++){
        deletarPonto(vetorPontos[i]);
    }

    *statusOperacao = OK;
    return instanciaTSP;
}

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximo(InstanciaTSP* instanciaTSP){
    return solucionarHeuristicaVizinhoMaisProximo(instanciaTSP);
}

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLados(InstanciaTSP* instanciaTSP){
    return solucionarHeuristicaVizinhoMaisProximoDoisLados(instanciaTSP);
}

void deletarInstanciaTSP(InstanciaTSP* instanciaTSP){
    if(instanciaTSP == NULL){
        return;
    }
    
    deletarTour(instanciaTSP->melhorSolucao);
    deletarGrafo(instanciaTSP->grafo);
    free(instanciaTSP->tipoPesoAresta);
    free(instanciaTSP->tipo);
    free(instanciaTSP->comentario);
    free(instanciaTSP->nome);
    free(instanciaTSP);
    return;
}

int getDimensaoInstanciaTSP(InstanciaTSP* instanciaTSP){
    return instanciaTSP->dimensao;
}

VerticeGrafo* getGrafoInstanciaTSP(InstanciaTSP* instanciaTSP){
    return instanciaTSP->grafo;
}

void setMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP, NoTour* tour){
    instanciaTSP->melhorSolucao = tour;
    calcularCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
}

void calcularCustoMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP){
    if(instanciaTSP->melhorSolucao == NULL){
        instanciaTSP->custoMelhorSolucao = -1;
        return;
    }

    double custoMelhorSolucao = 0, custoArestaAtual = -1;
    NoTour *cidadeOrigem = NULL, *cidadeDestino = NULL;

    //O retorno dessa função nunca será NULL pois foi testado anteriormente
    //se instanciaTSP->melhorSolucao era NULL. Como o resultado foi negativo
    //instanciaTSP->melhorSolucao possui um tour completo. Logo, possui ao menos
    //dois nós
    cidadeOrigem = getNoTourPosicao(instanciaTSP->melhorSolucao, 0);
    int i = 1;
    while(1){
        cidadeDestino = getNoTourPosicao(instanciaTSP->melhorSolucao, i);
        i++;

        if(cidadeDestino == NULL){
            break;
        }

        custoArestaAtual = getPesoArestaGrafo(instanciaTSP->grafo, getIdTour(cidadeOrigem), getIdTour(cidadeDestino));
        if(custoArestaAtual == -1){
            instanciaTSP->custoMelhorSolucao = -1;
            return;
        }
        custoMelhorSolucao = custoMelhorSolucao + custoArestaAtual;

        cidadeOrigem = cidadeDestino;
    }

    instanciaTSP->custoMelhorSolucao = custoMelhorSolucao;
    return;
}

void salvarInstanciaTSP(InstanciaTSP* instanciaTSP, FILE* arquivo){
    fprintf(arquivo, "Instância TSP:\n");
    
    if(instanciaTSP == NULL){
        return;
    }

    fprintf(arquivo, "instanciaTSP->nome: %s\n", instanciaTSP->nome);
    fprintf(arquivo, "instanciaTSP->comentario: %s\n", instanciaTSP->comentario);
    fprintf(arquivo, "instanciaTSP->tipo: %s\n", instanciaTSP->tipo);
    fprintf(arquivo, "instanciaTSP->dimensao: %d\n", instanciaTSP->dimensao);
    fprintf(arquivo, "instanciaTSP->tipoPesoAresta: %s\n", instanciaTSP->tipoPesoAresta);
    fprintf(arquivo, "instanciaTSP->grafo:\n");
    salvarGrafo(instanciaTSP->grafo, arquivo);
    fprintf(arquivo, "\ninstanciaTSP->melhorSolucao:\n");
    salvarTour(instanciaTSP->melhorSolucao, arquivo);
    fprintf(arquivo, "\ninstanciaTSP->custoMelhorSolucao: %lf\n", instanciaTSP->custoMelhorSolucao);
    return;
}

char* getNomeInstanciaTSP(InstanciaTSP* instanciaTSP){
    return instanciaTSP->nome;
}

//////////////////DEBUG///////////////////////////////
void imprimirInstanciaTSP(InstanciaTSP* instanciaTSP){
    printf("Instância TSP:\n");
    
    if(instanciaTSP == NULL){
        return;
    }

    printf("instanciaTSP->nome: %s\n", instanciaTSP->nome);
    printf("instanciaTSP->comentario: %s\n", instanciaTSP->comentario);
    printf("instanciaTSP->tipo: %s\n", instanciaTSP->tipo);
    printf("instanciaTSP->dimensao: %d\n", instanciaTSP->dimensao);
    printf("instanciaTSP->tipoPesoAresta: %s\n", instanciaTSP->tipoPesoAresta);
    printf("instanciaTSP->grafo:\n");
    imprimirGrafo(instanciaTSP->grafo);
    printf("\ninstanciaTSP->melhorSolucao:\n");
    imprimirTour(instanciaTSP->melhorSolucao);
    printf("\ninstanciaTSP->custoMelhorSolucao: %lf\n", instanciaTSP->custoMelhorSolucao);
    return;
}
