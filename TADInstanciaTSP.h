#ifndef TADInstanciaTSP
#define TADInstanciaTSP

#include "TADGrafo.h"
#include "TADPonto.h"
#include "TADTour.h"
#include "TADErro.h"
#include <stdlib.h>

typedef struct instanciaTSP{
    char* nome;
    char* comentario;
    char* tipo;
    int dimensao;
    char* tipoPesoAresta;
    VerticeGrafo* grafo;
    NoTour* melhorSolucao;
}InstanciaTSP;

InstanciaTSP* criarInstanciaTSP(char* pathArquivoTSP, int* statusOperacao);

/*
void solucionarInstanciaGRASP(InstanciaTSP* instanciaTSP);
*/

void deletarInstanciaTSP(InstanciaTSP* instanciaTSP);

#endif