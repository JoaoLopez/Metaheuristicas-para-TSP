#ifndef TADPONTO_H
#define TADPONTO_H

////////DEBUG////////
#include <stdio.h>
/////////////////////
#include <math.h>
#include <stdlib.h>
#include "TADErro.h"

typedef struct ponto{
    double x;
    double y;
}Ponto;

Ponto* criarPonto(double x, double y, int* statusOperacao);

double calcularDistanciaPontos(Ponto* ponto1, Ponto* ponto2);

void deletarPonto(Ponto* ponto);

///////////////DEBUG!!!!!!!!!!!!!!!!
void imprimirPonto(Ponto* ponto);
////////////////////////////////////

#endif