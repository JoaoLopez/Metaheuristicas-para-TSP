#include "TADGrafo.h"

VerticeGrafo* criarGrafo(){
    return NULL;
}

int verticeEstaGrafo(VerticeGrafo* grafo, int idVertice){
    int verticeEncontrado = 0;
    while(grafo != NULL){
        if(grafo->id == idVertice){
            verticeEncontrado = 1;
            break;
        }
        grafo = grafo->proximoVerticeGrafo;
    }
    return verticeEncontrado;
}

VerticeGrafo* inserirVertice(VerticeGrafo* grafo, int idVertice, int* statusOperacao){
    if(verticeEstaGrafo(grafo, idVertice)){
        *statusOperacao = OK;
        return grafo;
    }

    //Alocando memória para o novo vértice
    VerticeGrafo* novoVertice = (VerticeGrafo*) malloc(sizeof(VerticeGrafo));
    if(novoVertice == NULL){
        *statusOperacao = ERRO_MEMORIA_INSUFICIENTE;
        return grafo;
    }

    novoVertice->id = idVertice;
    novoVertice->verticeVizinho = NULL;
    novoVertice->proximoVerticeGrafo = grafo;

    *statusOperacao = OK;
    return novoVertice;
}

int arestaEstaGrafo(VerticeGrafo* grafo, int idVerticeOrigem, int idVerticeDestino){
    int arestaEncontrada = 0;
    
    VerticeVizinho* vizinhoAuxiliar = NULL;
    while(grafo != NULL){
        if(grafo->id == idVerticeOrigem){
            vizinhoAuxiliar = grafo->verticeVizinho;
            break;
        }
        grafo = grafo->proximoVerticeGrafo;
    }
    while(vizinhoAuxiliar != NULL){
        if(vizinhoAuxiliar->id == idVerticeDestino){
            arestaEncontrada = 1;
            break;
        }
        vizinhoAuxiliar = vizinhoAuxiliar->proximoVizinho;
    }

    return arestaEncontrada;
}

//Não é necessário retornar o grafo atualizado pois para inserir uma aresta
//é necessário que o grafo já possua pelo menos um vértice. Assim, as alterações
//feitas já poderão ser acessadas a partir da variável "grafo" passada por parâmetro
//para esse procedimento
int inserirAresta(VerticeGrafo* grafo, int idVerticeOrigem, int idVerticeDestino, double pesoAresta){
    if(!verticeEstaGrafo(grafo, idVerticeOrigem) || !verticeEstaGrafo(grafo, idVerticeDestino)){
        return ERRO_VERTICE_NAO_ENCONTRADO;
    }
    if(arestaEstaGrafo(grafo, idVerticeOrigem, idVerticeDestino)){
        return ERRO_ARESTA_JA_EXISTENTE;
    }
    if(pesoAresta < 0){
        return ERRO_PESO_ARESTA_NAO_POSITIVO;
    }

    //Buscando vértice de origem
    //Já foi verificado que ele se encontra no grafo
    VerticeGrafo* verticeOrigem = grafo;
    while(verticeOrigem != NULL){
        if(verticeOrigem->id == idVerticeOrigem){
            break;
        }
        verticeOrigem = verticeOrigem->proximoVerticeGrafo;
    }

    VerticeVizinho* novoVerticeVizinho = (VerticeVizinho*) malloc(sizeof(VerticeVizinho));
    if(novoVerticeVizinho == NULL){
        return ERRO_MEMORIA_INSUFICIENTE;
    }

    novoVerticeVizinho->id = idVerticeDestino;
    novoVerticeVizinho->pesoAresta = pesoAresta;
    novoVerticeVizinho->proximoVizinho = verticeOrigem->verticeVizinho;
    verticeOrigem->verticeVizinho = novoVerticeVizinho;
    return OK;
}

void ordenarPesosArestasGrafo(VerticeGrafo* grafo){
    if(grafo == NULL){
        return;
    }

    //Neste loop "verticeAtual" armazenará o valor do vértice do grafo que está sendo processado
    //"vizinhoAux" percorrerá a lista de vizinhos consultando o peso de cada aresta
    //"vizinhoAtual" apontará para o último "VerticeVizinho" já ordenado na lista de vizinhos
    //"vizinhoMenorPeso" e "valorVizinhoMenorPeso" armazenarão o "VerticeVizinho" e o peso da aresta
    //de menor peso até agora encontrada e não ordenada
    //"vizinhoAnteriorMenorPeso" apontará para o "VerticeVizinho" localizado imediatamente antes de "vizinhoMenorPeso"
    VerticeGrafo* verticeAtual = grafo;
    VerticeVizinho *vizinhoAnteriorMenorPeso = NULL, *vizinhoMenorPeso = NULL, *vizinhoAtual = NULL, *vizinhoAux = NULL;
    int valorVizinhoMenorPeso = -1;
    while(verticeAtual != NULL){
        //Inicializando variáveis
        vizinhoAtual = NULL;
        vizinhoAux = verticeAtual->verticeVizinho;
        do{
            //Inicializando variáveis
            vizinhoAnteriorMenorPeso = NULL;
            vizinhoMenorPeso = NULL;
            valorVizinhoMenorPeso = -1;

            //Verificando se a lista de vizinhos está vazia
            if(verticeAtual->verticeVizinho == NULL){
                break;
            }

            //Buscando aresta de menor peso ainda desordenada
            while(vizinhoAux != NULL){
                if(vizinhoAux->pesoAresta < valorVizinhoMenorPeso || valorVizinhoMenorPeso == -1){
                    valorVizinhoMenorPeso = vizinhoAux->pesoAresta;
                    vizinhoMenorPeso = vizinhoAux;
                }

                vizinhoAux = vizinhoAux->proximoVizinho;
            }

            //Buscando nó imediatamente anterior a "vizinhoMenorPeso"
            //na lista de vizinhos
            vizinhoAux = verticeAtual->verticeVizinho;
            while(vizinhoAux != vizinhoMenorPeso){
                vizinhoAnteriorMenorPeso = vizinhoAux;
                vizinhoAux = vizinhoAux->proximoVizinho;
            }

            //Retirando "vizinhoMenorPeso" da lista de vizinhos, adicionando-o
            //na posição correta da lista de vizinhos e atualizando "vizinhoAtual"

            //Caso em que "vizinhoMenorPeso" já está na primeira posição da
            //lista de vizinhos
            if(vizinhoAnteriorMenorPeso == NULL){
                vizinhoAtual = vizinhoMenorPeso;
            }
            else{
                //Excluindo "vizinhoMenorPeso" da lista de vizinhos
                vizinhoAnteriorMenorPeso->proximoVizinho = vizinhoMenorPeso->proximoVizinho;

                //Verificando se "vizinhoMenorPeso" é o primeiro vizinho
                //sendo ordenado
                if(vizinhoAtual == NULL){
                    vizinhoMenorPeso->proximoVizinho = verticeAtual->verticeVizinho;
                    verticeAtual->verticeVizinho = vizinhoMenorPeso;
                    vizinhoAtual = vizinhoMenorPeso;
                }
                else{
                    vizinhoMenorPeso->proximoVizinho = vizinhoAtual->proximoVizinho;
                    vizinhoAtual->proximoVizinho = vizinhoMenorPeso;
                    vizinhoAtual = vizinhoMenorPeso;
                }
            }

            //Atualizando "vizinhoAux"
            vizinhoAux = vizinhoAtual->proximoVizinho;

        }while(vizinhoAtual->proximoVizinho != NULL);
        
        verticeAtual = verticeAtual->proximoVerticeGrafo;
    }

    return;
}

void deletarGrafo(VerticeGrafo* grafo){
    while(grafo != NULL){
        grafo = deletarVertice(grafo, grafo->id);
    }
}

VerticeGrafo* deletarVertice(VerticeGrafo* grafo, int idVertice){
    if(!verticeEstaGrafo(grafo, idVertice)){
        return grafo;
    }

    //Apagar todos os VerticeVizinho com id == idVertice
    VerticeGrafo* verticeGrafoAtual = grafo;
    VerticeVizinho* vizinhoAtual = NULL;
    VerticeVizinho* vizinhoAnterior = NULL;
    while(verticeGrafoAtual != NULL){
        vizinhoAtual = verticeGrafoAtual->verticeVizinho;
        vizinhoAnterior = NULL;
        while(vizinhoAtual != NULL){
            if(vizinhoAtual->id == idVertice){
                deletarAresta(grafo, verticeGrafoAtual->id, idVertice);
                
                if(vizinhoAnterior == NULL){
                    vizinhoAtual = verticeGrafoAtual->verticeVizinho;
                }
                else{
                    vizinhoAtual = vizinhoAnterior->proximoVizinho;
                }
            }
            else{
                vizinhoAnterior = vizinhoAtual;
                vizinhoAtual = vizinhoAtual->proximoVizinho;
            }
        }
        verticeGrafoAtual = verticeGrafoAtual->proximoVerticeGrafo;
    }

    //Apagar VerticeGrafo com id == idVertice
    VerticeGrafo* verticeSeraApagado = grafo;
    VerticeGrafo* verticeAnterior = NULL;
    while(verticeSeraApagado != NULL){
        if(verticeSeraApagado->id == idVertice){
            //Apagando todas as arestas que tem origem em "verticeSeraApagado"
            while(verticeSeraApagado->verticeVizinho != NULL){
                deletarAresta(grafo, idVertice, verticeSeraApagado->verticeVizinho->id);
            }

            //Apagando VerticeGrafo com id == idVertice
            if(verticeAnterior == NULL){
                grafo = verticeSeraApagado->proximoVerticeGrafo;
            }
            else{
                verticeAnterior->proximoVerticeGrafo = verticeSeraApagado->proximoVerticeGrafo;
            }
            free(verticeSeraApagado);
        }
        verticeAnterior = verticeSeraApagado;
        verticeSeraApagado = verticeSeraApagado->proximoVerticeGrafo;
    }

    return grafo;
}

//Não é necessário retornar o grafo atualizado pois para deletar uma aresta
//é necessário que o grafo já possua pelo menos um vértice. Assim, as alterações
//feitas já poderão ser acessadas a partir da variável "grafo" passada por parâmetro
//para esse procedimento
void deletarAresta(VerticeGrafo* grafo,  int idVerticeOrigem, int idVerticeDestino){
    if(!verticeEstaGrafo(grafo, idVerticeOrigem) || !verticeEstaGrafo(grafo, idVerticeDestino)
    || !arestaEstaGrafo(grafo, idVerticeOrigem, idVerticeDestino)){
        return;
    }

    //Buscando vértice de origem
    //Já foi verificado que ele se encontra no grafo
    VerticeGrafo* verticeOrigem = grafo;
    VerticeVizinho* verticeDestino = NULL;
    VerticeVizinho* verticeVizinhoAnterior = NULL;
    while(verticeOrigem != NULL){
        if(verticeOrigem->id == idVerticeOrigem){
            verticeDestino = verticeOrigem->verticeVizinho;
            break;
        }
        verticeOrigem = verticeOrigem->proximoVerticeGrafo;
    }

    //Buscando vértice de destino
    //Já foi verificado que ele se encontra no grafo
    while(verticeDestino != NULL){
        if(verticeDestino->id == idVerticeDestino){
            if(verticeVizinhoAnterior == NULL){
                verticeOrigem->verticeVizinho = verticeDestino->proximoVizinho;
            }
            else{
                verticeVizinhoAnterior->proximoVizinho = verticeDestino->proximoVizinho;
            }

            free(verticeDestino);
            return;
        }
        verticeVizinhoAnterior = verticeDestino;
        verticeDestino = verticeDestino->proximoVizinho;
    }
}

void salvarGrafo(VerticeGrafo* grafo, FILE* arquivo){
    VerticeGrafo* grafoAuxiliar = grafo;
    VerticeVizinho* vizinhoAuxiliar = NULL;
    fprintf(arquivo, "####### IMPRIMINDO GRAFO #######\n");
    while(grafoAuxiliar != NULL){
        if(grafo != grafoAuxiliar){
            fprintf(arquivo, "\n|\n");
        }
        fprintf(arquivo, "%d", grafoAuxiliar->id);
        vizinhoAuxiliar = grafoAuxiliar->verticeVizinho;
        while(vizinhoAuxiliar != NULL){
            fprintf(arquivo, "-%d(%lf)", vizinhoAuxiliar->id, vizinhoAuxiliar->pesoAresta);
            vizinhoAuxiliar = vizinhoAuxiliar->proximoVizinho;
        }
        grafoAuxiliar = grafoAuxiliar->proximoVerticeGrafo;
    }
    fprintf(arquivo, "\n");
}

VerticeGrafo* getVerticeGrafo(VerticeGrafo* grafo,  int idVertice){
    while(grafo != NULL){
        if(grafo->id == idVertice){
            return grafo;
        }

        grafo = grafo->proximoVerticeGrafo;
    }

    return NULL;
}

double getPesoArestaGrafo(VerticeGrafo* grafo, int idVerticeOrigem, int idVerticeDestino){
    while(grafo != NULL){
        if(grafo->id == idVerticeOrigem){
            break;
        }
        
        grafo = grafo->proximoVerticeGrafo;
    }
    if(grafo == NULL){
        return -1;
    }

    VerticeVizinho* vizinho = grafo->verticeVizinho;
    while(vizinho != NULL){
        if(vizinho->id == idVerticeDestino){
            break;
        }
        
        vizinho = vizinho->proximoVizinho;
    }
    if(vizinho == NULL){
        return -1;
    }
    
    return vizinho->pesoAresta;
}

VerticeVizinho* getArestaMaiorCustoVertice(VerticeGrafo* verticeGrafo){
    if(verticeGrafo == NULL){
        return NULL;
    }

    int custoMaiorAresta = -1;
    VerticeVizinho *vizinhoAtual = verticeGrafo->verticeVizinho, *vizinhoMaiorCusto = NULL;
    while(vizinhoAtual != NULL){
        if(vizinhoAtual->pesoAresta > custoMaiorAresta){
            custoMaiorAresta = vizinhoAtual->pesoAresta;
            vizinhoMaiorCusto = vizinhoAtual;
        }

        vizinhoAtual = vizinhoAtual->proximoVizinho;
    }

    return vizinhoMaiorCusto;
}

VerticeVizinho* getArestaMenorCustoVertice(VerticeGrafo* verticeGrafo){
    if(verticeGrafo == NULL){
        return NULL;
    }

    int custoMenorAresta = -1;
    VerticeVizinho *vizinhoAtual = verticeGrafo->verticeVizinho, *vizinhoMenorCusto = NULL;
    while(vizinhoAtual != NULL){
        if(vizinhoAtual->pesoAresta < custoMenorAresta ||
        custoMenorAresta == -1){
            custoMenorAresta = vizinhoAtual->pesoAresta;
            vizinhoMenorCusto = vizinhoAtual;
        }

        vizinhoAtual = vizinhoAtual->proximoVizinho;
    }

    return vizinhoMenorCusto;
}

//DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!
void imprimirGrafo(VerticeGrafo* grafo){
    VerticeGrafo* grafoAuxiliar = grafo;
    VerticeVizinho* vizinhoAuxiliar = NULL;
    printf("####### IMPRIMINDO GRAFO #######\n");
    while(grafoAuxiliar != NULL){
        if(grafo != grafoAuxiliar){
            printf("\n|\n");
        }
        printf("%d", grafoAuxiliar->id);
        vizinhoAuxiliar = grafoAuxiliar->verticeVizinho;
        while(vizinhoAuxiliar != NULL){
            printf("-%d(%lf)", vizinhoAuxiliar->id, vizinhoAuxiliar->pesoAresta);
            vizinhoAuxiliar = vizinhoAuxiliar->proximoVizinho;
        }
        grafoAuxiliar = grafoAuxiliar->proximoVerticeGrafo;
    }
    printf("\n");
}
