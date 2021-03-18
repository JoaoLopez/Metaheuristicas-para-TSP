#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <sys/resource.h>

int sortearNumeroAleatorio(int valorMinimo, int valorMaximo);

int elementoEstaVetor(int elemento, int* vetor, int tamanhoVetor);

void Tempo_CPU_Sistema(double *seg_CPU_total, double *seg_sistema_total);

#endif