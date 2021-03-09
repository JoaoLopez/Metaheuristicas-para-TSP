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
int insercaoCidadeTourValida(int idCidade, NoTour* tour);
NoTour* inserirCidadeFimTour(int idCidade, NoTour* tour, int* statusOperacao);
NoTour* deletarCidadeTour(int idCidade, NoTour* tour);
void deletarTour(NoTour* tour);

////////////DEBUG///////////
void imprimirTour(NoTour* tour);

#endif