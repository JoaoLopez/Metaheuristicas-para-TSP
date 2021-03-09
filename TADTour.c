#include "TADTour.h"

NoTour* criarTour(){
    return NULL;
}

int tourEstaCompleto(NoTour* tour){
    if(tour == NULL){
        return 0;
    }
    if(tour->proximoNoTour == NULL){
        return 0;
    }

    int idCidadeInicial = tour->idCidade;
    while(tour->proximoNoTour != NULL){
        tour = tour->proximoNoTour;
    }

    if(tour->idCidade == idCidadeInicial){
        return 1;
    }
    else{
        return 0;
    }
}

int insercaoCidadeTourValida(int idCidade, NoTour* tour){
    //Verificando se o tour não possui ainda nenhuma cidade adicionada
    if(tour == NULL){
        return 1;
    }

    if(tourEstaCompleto(tour)){
        return 0;
    }

    //Como a cidade de origem é a única que pode aparecer duas vezes no tour
    //e atualmente está aparecendo apenas uma vez no tour (foi verificado acima
    //que o tour não está completo), não é necessário testar se a cidade de origem
    //é igual a cidade que se deseja inserir
    tour = tour->proximoNoTour;
    while(tour != NULL){
        if(tour->idCidade == idCidade){
            return 0;
        }
        tour = tour->proximoNoTour;
    }

    return 1;
}

NoTour* inserirCidadeFimTour(int idCidade, NoTour* tour, int* statusOperacao){
    //Verificando se a inserção da cidade no tour gerará um tour válido
    if(!insercaoCidadeTourValida(idCidade, tour)){
        *statusOperacao = ERRO_INSERCAO_CIDADE_TOUR_INVALIDA;
        return tour;
    }

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