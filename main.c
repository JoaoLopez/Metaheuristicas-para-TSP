#ifndef MAIN_C
#define MAIN_C

#include <stdio.h>
#include "TADGrafo.h"
#include "TADErro.h"

VerticeGrafo* criarGrafo(char* pathArquivoTSP){
    FILE* arquivoTSP = open(pathArquivoTSP, "r");
    
}

int main(void){
    VerticeGrafo* grafo = criarGrafo("");
    imprimirGrafo(grafo);
    return 0;
}

#endif