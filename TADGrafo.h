#ifndef TAD_GRAFO_H
#define TAD_GRAFO_H
#include <stdlib.h>
#include "TADErro.h"
///DEBUG!!!!!!!!!!
#include <stdio.h>
///////////////////

typedef struct verticeVizinho{
    int id;
    double pesoAresta;
    struct verticeVizinho* proximoVizinho;
}VerticeVizinho;

typedef struct verticeGrafo{
    int id;
    VerticeVizinho* verticeVizinho;
    struct verticeGrafo* proximoVerticeGrafo;
}VerticeGrafo;

VerticeGrafo* criarGrafo();

int verticeEstaGrafo(VerticeGrafo* grafo, int idVertice);

VerticeGrafo* inserirVertice(VerticeGrafo* grafo, int idVertice, int* statusOperacao);

int arestaEstaGrafo(VerticeGrafo* grafo, int idVerticeOrigem, int idVerticeDestino);

//Não é necessário retornar o grafo atualizado pois para inserir uma aresta
//é necessário que o grafo já possua pelo menos um vértice. Assim, as alterações
//feitas já poderão ser acessadas a partir da variável "grafo" passada por parâmetro
//para esse procedimento
int inserirAresta(VerticeGrafo* grafo, int idVerticeOrigem, int idVerticeDestino, double pesoAresta);

void ordenarPesosArestasGrafo(VerticeGrafo* grafo);

void deletarGrafo(VerticeGrafo* grafo);

VerticeGrafo* deletarVertice(VerticeGrafo* grafo, int idVertice);

//Não é necessário retornar o grafo atualizado pois para deletar uma aresta
//é necessário que o grafo já possua pelo menos um vértice. Assim, as alterações
//feitas já poderão ser acessadas a partir da variável "grafo" passada por parâmetro
//para esse procedimento
void deletarAresta(VerticeGrafo* grafo,  int idVerticeOrigem, int idVerticeDestino);

void salvarGrafo(VerticeGrafo* grafo, FILE* arquivo);

VerticeGrafo* getVerticeGrafo(VerticeGrafo* grafo,  int idVertice);

double getPesoArestaGrafo(VerticeGrafo* grafo, int idVerticeOrigem, int idVerticeDestino);

//Atualmente não está sendo utilizada
VerticeVizinho* getArestaMaiorCustoVertice(VerticeGrafo* verticeGrafo);

//Atualmente não está sendo utilizada
VerticeVizinho* getArestaMenorCustoVertice(VerticeGrafo* verticeGrafo);

//DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!
void imprimirGrafo(VerticeGrafo* grafo);

#endif