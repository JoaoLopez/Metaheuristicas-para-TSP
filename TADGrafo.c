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
    if(pesoAresta <= 0){
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
}
