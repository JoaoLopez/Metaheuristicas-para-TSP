#include "util.h"

int sortearNumeroAleatorio(int valorMinimo, int valorMaximo){
    return (rand() % (valorMaximo - valorMinimo + 1)) + valorMinimo;
}

void sortearNumerosAleatoriosDistintos(int qtd, int valorMinimo, int valorMaximo, int* resultado){
    int aux, i=0;
    while(i < qtd){
        aux = sortearNumeroAleatorio(valorMinimo, valorMaximo);
        if(!elementoEstaVetor(aux, resultado, i)){
            resultado[i] = aux;
            i++;
        }
    }
}

int elementoEstaVetor(int elemento, int* vetor, int tamanhoVetor){
    if(vetor == NULL){
        return 0;
    }
    
    for(int i = 0; i < tamanhoVetor; i++){
        if(vetor[i] == elemento){
            return 1;
        }
    }
    return 0;
}

void ordenar_vetor(int* vetor, int tam_vetor){
    mergeSort(vetor, 0, tam_vetor-1);
}

void mergeSort(int arr[], int l, int r){
    if (l < r){
        int m = l+(r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

void merge(int arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2){
        if (L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}

void Tempo_CPU_Sistema(double *seg_CPU_total, double *seg_sistema_total){

  long seg_CPU, seg_sistema, mseg_CPU, mseg_sistema;
  struct rusage ptempo;

  getrusage(0,&ptempo);

  seg_CPU = ptempo.ru_utime.tv_sec;
  mseg_CPU = ptempo.ru_utime.tv_usec;
  seg_sistema = ptempo.ru_stime.tv_sec;
  mseg_sistema = ptempo.ru_stime.tv_usec;

 *seg_CPU_total     = (seg_CPU + 0.000001 * mseg_CPU);
 *seg_sistema_total = (seg_sistema + 0.000001 * mseg_sistema);
}
