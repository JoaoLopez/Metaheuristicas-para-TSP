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
                distancia = (int) (calcularDistanciaPontos(ponto1, ponto2) + 0.5);

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

int carregarSolucaoInstanciaTSP(char* pathArquivoSolucao, InstanciaTSP* instanciaTSP){
    //Abrindo arquivo que contém a solução da instância TSP
    FILE* arquivoSolucao = fopen(pathArquivoSolucao, "rt");
    if(arquivoSolucao == NULL){
        return ERRO_ABRIR_ARQUIVO;
    }

    int* statusOperacao = (int*) malloc(sizeof(int));
    if(statusOperacao == NULL){
        return ERRO_MEMORIA_INSUFICIENTE;
    }

    char aux[100];
    do{
        fgets(aux, 100, arquivoSolucao);
    }while(strcmp(aux, "instanciaTSP->melhorSolucao:\n") != 0);
    fscanf(arquivoSolucao, "Tour\n");
    
    NoTour* solucao = criarTour();
    int idCidadeAtual = -1;
    for(int i = 0; i <= instanciaTSP->dimensao; i++){
        fscanf(arquivoSolucao, "idCidade: %d\n", &idCidadeAtual);

        solucao = inserirCidadeFimTour(idCidadeAtual, solucao, statusOperacao);
        if(*statusOperacao != OK){
            deletarTour(solucao);
            free(statusOperacao);
            fclose(arquivoSolucao);
            return ERRO_CARREGAR_SOLUCAO;
        }
        
        if(i + 1 <= instanciaTSP->dimensao){
            fscanf(arquivoSolucao, "|\n");
        }
    }

    instanciaTSP->melhorSolucao = solucao;
    instanciaTSP->custoMelhorSolucao = calcularCustoSolucaoInstanciaTSP(instanciaTSP, solucao);

    free(statusOperacao);
    fclose(arquivoSolucao);

    return OK;
}

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximo(InstanciaTSP* instanciaTSP){
    return solucionarHeuristicaVizinhoMaisProximo(instanciaTSP);
}

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLados(InstanciaTSP* instanciaTSP){
    return solucionarHeuristicaVizinhoMaisProximoDoisLados(instanciaTSP);
}

int solucionarInstanciaTSPHeuristicaVizinhoMaisProximoDoisLadosRandomizada(InstanciaTSP* instanciaTSP, double alpha){
    return solucionarHeuristicaVizinhoMaisProximoDoisLadosRandomizada(instanciaTSP, alpha);
}

int melhorarSolucaoInstanciaTSPBuscaLocalPrimeiroAprimorante(InstanciaTSP* instanciaTSP){
    return executarBuscaLocalPrimeiroAprimorante(instanciaTSP);
}

int melhorarSolucaoInstanciaTSPBuscaLocalMelhorAprimorante(InstanciaTSP* instanciaTSP){
    return executarBuscaLocalMelhorAprimorante(instanciaTSP);
}

int solucionarInstanciaTSPGRASP(InstanciaTSP* instanciaTSP, Metricas* metricas, int numeroRepeticoes, double alpha){
    return executarGRASP(instanciaTSP, metricas, numeroRepeticoes, alpha);
}

int solucionarInstanciaTSPILS(InstanciaTSP* instanciaTSP, Metricas* metricas, int numeroRepeticoes, double alpha){
    return executarILS(instanciaTSP, metricas, numeroRepeticoes, alpha);
}

int solucionarInstanciaTSPILSv2(InstanciaTSP* instanciaTSP, Metricas* metricas, int numeroRepeticoes, double alpha){
    return executarILSv2(instanciaTSP, metricas, numeroRepeticoes, alpha);
}

double calcularCustoSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP, NoTour* tour){
    if(instanciaTSP->grafo == NULL || tour == NULL){
        return -1;
    }

    double custoSolucao = 0, custoArestaAtual = -1;
    NoTour *cidadeOrigem = NULL, *cidadeDestino = NULL;

    //O retorno dessa função nunca será NULL pois foi testado anteriormente
    //se "tour" era NULL. Como o resultado foi negativo "tour" possui um tour completo.
    //Logo, possui ao menos dois nós
    cidadeOrigem = getNoTourPosicao(tour, 0);
    int i = 1;
    while(1){
        cidadeDestino = getNoTourPosicao(tour, i);
        i++;

        if(cidadeDestino == NULL){
            break;
        }

        custoArestaAtual = getPesoArestaGrafo(instanciaTSP->grafo, getIdTour(cidadeOrigem), getIdTour(cidadeDestino));
        if(custoArestaAtual == -1){
            return -1;
        }
        custoSolucao = custoSolucao + custoArestaAtual;

        cidadeOrigem = cidadeDestino;
    }

    return custoSolucao;
}

void atualizarCustoMelhorSolucaoInstanciaTSPMovimentoBuscaLocal(InstanciaTSP* instanciaTSP, double saldoNovoCusto){
    instanciaTSP->custoMelhorSolucao = instanciaTSP->custoMelhorSolucao - saldoNovoCusto;
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

void removerSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP){
    instanciaTSP->melhorSolucao = NULL;
    instanciaTSP->custoMelhorSolucao = -1;
}

void setMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP, NoTour* tour){
    if(instanciaTSP->melhorSolucao == NULL){
        instanciaTSP->melhorSolucao = tour;
        instanciaTSP->custoMelhorSolucao = calcularCustoSolucaoInstanciaTSP(instanciaTSP, tour);
        return;        
    }

    double custoNovaSolucao = calcularCustoSolucaoInstanciaTSP(instanciaTSP, tour);

    if(custoNovaSolucao < instanciaTSP->custoMelhorSolucao){
        instanciaTSP->melhorSolucao = tour;
        instanciaTSP->custoMelhorSolucao = custoNovaSolucao;
    }
}

char* getNomeInstanciaTSP(InstanciaTSP* instanciaTSP){
    return instanciaTSP->nome;
}

double getCustoMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP){
    return instanciaTSP->custoMelhorSolucao;
}

VerticeGrafo* getGrafoInstanciaTSP(InstanciaTSP* instanciaTSP){
    return instanciaTSP->grafo;
}

int getDimensaoInstanciaTSP(InstanciaTSP* instanciaTSP){
    return instanciaTSP->dimensao;
}

NoTour* getMelhorSolucaoInstanciaTSP(InstanciaTSP* instanciaTSP){
    return instanciaTSP->melhorSolucao;
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
