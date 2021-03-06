#include "TADTour.h"

NoTour* criarTour(){
    return NULL;
}

NoTour* inserirCidadeFimTour(int idCidade, NoTour* tour, int* statusOperacao){
    //Alocando novo NoTour
    NoTour* novoNoTour = (NoTour*) malloc(sizeof(NoTour));
    if(novoNoTour == NULL){
        *statusOperacao = ERRO_MEMORIA_INSUFICIENTE;
        return tour;
    }

    novoNoTour->idCidade = idCidade;
    novoNoTour->proximoNoTour = NULL;

    if(tour == NULL){
        tour = novoNoTour;
    }
    else{
        //Percorrendo "tour" até o último nó
        NoTour* aux = tour;
        while(aux->proximoNoTour != NULL){
            aux = aux->proximoNoTour;
        }
        aux->proximoNoTour = novoNoTour;
    }
    
    *statusOperacao = OK;
    return tour;
}

NoTour* deletarCidadeTour(int idCidade, NoTour* tour){
    if(tour == NULL){
        return tour;
    }

    //Percorrendo o tour em busca da cidade a ser deletada
    NoTour* aux = tour;
    NoTour* anterior = NULL;
    while(aux->proximoNoTour != NULL && aux->idCidade != idCidade){
        anterior = aux;
        aux = aux->proximoNoTour;
    }

    //Testando se a cidade foi encontrada
    if(aux->idCidade == idCidade){
        //Ajustando ponteiros do final do tour
        if(anterior == NULL){
            tour = NULL;
        }
        else{
            anterior->proximoNoTour = NULL;
        }

        //Deletando subtour que inicia com a cidade a ser deletada
        NoTour* proximo = aux->proximoNoTour;
        while(proximo != NULL){
            free(aux);
            aux = proximo;
            proximo = proximo->proximoNoTour;
        }
        free(aux);
    }

    return tour;
}

void deletarTour(NoTour* tour){
    if(tour != NULL){
        //O retorno da função "deletarCidadeTour" será NULL
        //Não é necessário capturá-lo pois essa função não possui retorno
        //nem irá utilizá-lo
        deletarCidadeTour(tour->idCidade, tour);
    }

    return;
}

////////////DEBUG///////////
void imprimirTour(NoTour* tour){
    printf("Tour\n");
    while(tour != NULL){
        printf("idCidade: %d\n", tour->idCidade);
        tour = tour->proximoNoTour;

        if(tour != NULL){
            printf("|\n");
        }
    }

    return;
}