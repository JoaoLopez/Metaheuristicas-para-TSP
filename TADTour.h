#ifndef TADTOUR_H
#define TADTOUR_H

///////////DEBUG////////
#include <stdio.h>
////////////////////////

#include <stdlib.h>
#include "TADErro.h"

typedef struct noTour{
    int idCidade;
    struct noTour* proximoNoTour;
}NoTour;

NoTour* criarTour();
int tourEstaCompleto(NoTour* tour);
int insercaoCidadeTourValida(int idCidade, NoTour* tour, int insercaoInicioTour);
NoTour* inserirCidadeInicioTour(int idCidade, NoTour* tour, int* statusOperacao);
NoTour* inserirCidadeFimTour(int idCidade, NoTour* tour, int* statusOperacao);
int realizarMovimento2otimo(NoTour* tour, int cidadeOrigem1, int cidadeOrigem2);
NoTour* getNoTourPosicao(NoTour* tour, int posicao);
NoTour* deletarCidadeTour(int idCidade, NoTour* tour);
void deletarArestasTour(NoTour* tour, int* arestas, int num_arestas, NoTour** subtours_gerados);
NoTour* unirSubTours(NoTour* subtour1, NoTour* subtour2);
void deletarTour(NoTour* tour);
void salvarTour(NoTour* tour, FILE* arquivo);
int getIdTour(NoTour* tour);

////////////DEBUG///////////
void imprimirTour(NoTour* tour);

#endif