#include "TADPonto.h"

Ponto* criarPonto(double x, double y, int* statusOperacao){
    Ponto* ponto = (Ponto*) malloc(sizeof(Ponto));
    if(ponto == NULL){
        *statusOperacao = ERRO_MEMORIA_INSUFICIENTE;
        return NULL;
    }

    ponto->x = x;
    ponto->y = y;

    *statusOperacao = OK;

    return ponto;
}

double calcularDistanciaPontos(Ponto* ponto1, Ponto* ponto2){
    double distanciaQuadrado = pow((ponto1->x - ponto2->x), 2) + pow((ponto1->y - ponto2->y), 2);
    return sqrt(distanciaQuadrado);
}

void deletarPonto(Ponto* ponto){
    free(ponto);
    return;
}

//////////////DEBUG!!!!!!!!!!!!!!!!!
void imprimirPonto(Ponto* ponto){
    printf("Ponto:\n");
    printf("x = %lf\n", ponto->x);
    printf("y = %lf\n", ponto->y);
}