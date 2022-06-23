#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <sys/resource.h>

int sortearNumeroAleatorio(int valorMinimo, int valorMaximo);

void sortearNumerosAleatoriosDistintos(int qtd, int valorMinimo, int valorMaximo, int* resultado);

int elementoEstaVetor(int elemento, int* vetor, int tamanhoVetor);

void ordenar_vetor(int* vetor, int tam_vetor);

void mergeSort(int arr[], int l, int r);

void merge(int arr[], int l, int m, int r);

void Tempo_CPU_Sistema(double *seg_CPU_total, double *seg_sistema_total);

#endif