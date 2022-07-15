#include "BuscaLocal.h"

int executarBuscaLocalPrimeiroAprimorante(InstanciaTSP* instanciaTSP){
    double custoSolucaoAntiga = -1, custoSolucaoNova = -1;
    int statusOperacao = -1;
    do{
        custoSolucaoAntiga = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
        statusOperacao = encontrarPrimeiroAprimoranteBuscaLocalMovimento2Otimo(instanciaTSP);
        custoSolucaoNova = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
        
        if(statusOperacao != OK){
            return statusOperacao;
        }
    }while(custoSolucaoNova < custoSolucaoAntiga);

    return OK;
}

int encontrarPrimeiroAprimoranteBuscaLocalMovimento2Otimo(InstanciaTSP* instanciaTSP){
    int numeroCidades = getDimensaoInstanciaTSP(instanciaTSP);

    //Verificando se é possível fazer movimentos 2-ótimo nesse grafo
    if(numeroCidades <= 3){
        return OK;
    }

    NoTour* melhorSolucao = getMelhorSolucaoInstanciaTSP(instanciaTSP);
    double custoSolucaoAtual = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
    
    //Apesar de nesse trabalho serem executadas apenas instâncias de TSP
    //não havendo instâncias de ATSP, optei por nomear as variáveis abaixo
    //"noTourOrigemAresta", "cidadeOrigemAresta", "noTourDestinoAresta" e "cidadeDestinoAresta"
    //para ficar mais claro como os dois loops for abaixo trabalham
    NoTour *noTourOrigemAresta1 = NULL, *noTourDestinoAresta1 = NULL;
    NoTour *noTourOrigemAresta2 = NULL, *noTourDestinoAresta2 = NULL;
    int cidadeOrigemAresta1 = -1, cidadeDestinoAresta1 = -1;
    int cidadeOrigemAresta2 = -1, cidadeDestinoAresta2 = -1;
    double pesoAresta1 = -1, pesoAresta2 = -1, pesoNovaAresta1 = -1, pesoNovaAresta2 = -1, saldoNovaSolucao = 0;
    int solucaoAprimorada = 0;
    for(int i = 0; i < numeroCidades; i++){
        noTourOrigemAresta1 = getNoTourPosicao(melhorSolucao, i);
        noTourDestinoAresta1 = getNoTourPosicao(melhorSolucao, i+1);
        if(noTourOrigemAresta1 == NULL || noTourDestinoAresta1 == NULL){
            return ERRO_RECUPERAR_NO_TOUR;
        }
        cidadeOrigemAresta1 = getIdTour(noTourOrigemAresta1);
        cidadeDestinoAresta1 = getIdTour(noTourDestinoAresta1);

        for(int j = i + 2; j < numeroCidades; j++){
            noTourOrigemAresta2 = getNoTourPosicao(melhorSolucao, j);
            noTourDestinoAresta2 = getNoTourPosicao(melhorSolucao, j+1);
            if(noTourOrigemAresta2 == NULL || noTourDestinoAresta2 == NULL){
                return ERRO_RECUPERAR_NO_TOUR;
            }
            
            cidadeOrigemAresta2 = getIdTour(noTourOrigemAresta2);
            cidadeDestinoAresta2 = getIdTour(noTourDestinoAresta2);
            
            //Recuperando pesos das arestas selecionadas para serem substituidas
            //e peso das novas arestas obtidas com o movimento 2-ótimo

            //Devido ao modo como os loops for estão estruturados, temos que,
            //ao removermos as duas arestas selecionadas do tour, este passará
            //a ser um grafo com duas componentes conexas, sendo que em uma delas
            //estarão as cidades "cidadeDestinoAresta1" e "cidadeOrigemAresta2"
            //enquanto na outra se encontrarão as cidades "cidadeOrigemAresta1" e "cidadeDestinoAresta2"
            //Logo, as duas novas arestas adicionadas seriam
            //("cidadeDestinoAresta1", "cidadeDestinoAresta2") e ("cidadeOrigemAresta1", "cidadeOrigemAresta2");

            pesoAresta1 = getPesoArestaGrafo(getGrafoInstanciaTSP(instanciaTSP), cidadeOrigemAresta1, cidadeDestinoAresta1);
            pesoAresta2 = getPesoArestaGrafo(getGrafoInstanciaTSP(instanciaTSP), cidadeOrigemAresta2, cidadeDestinoAresta2);
            if(pesoAresta1 == -1 || pesoAresta2 == -1){
                return ERRO_RECUPERAR_PESO_ARESTA_GRAFO;
            }

            pesoNovaAresta1 = getPesoArestaGrafo(getGrafoInstanciaTSP(instanciaTSP), cidadeOrigemAresta1, cidadeOrigemAresta2);
            pesoNovaAresta2 = getPesoArestaGrafo(getGrafoInstanciaTSP(instanciaTSP), cidadeDestinoAresta1, cidadeDestinoAresta2);
            //Como os grafos deste trabalho são completos, a ocorrência do erro abaixo seria inesperada
            //pois as novas arestas deveriam existir no grafo
            if(pesoNovaAresta1 == -1 || pesoNovaAresta2 == -1){
                return ERRO_RECUPERAR_PESO_ARESTA_GRAFO;
            }

            //Calculando saldo da nova solução
            saldoNovaSolucao = pesoAresta1 + pesoAresta2 - pesoNovaAresta1 - pesoNovaAresta2;

            if(saldoNovaSolucao > 0){
                int statusOperacao = realizarMovimento2otimo(melhorSolucao, cidadeOrigemAresta1, cidadeOrigemAresta2);
                if(statusOperacao != OK && statusOperacao != ERRO_CIDADES_INVALIDAS){
                    return ERRO_REALIZAR_MOVIMENTO_2_OTIMO;
                }

                if(statusOperacao == OK){
                    atualizarCustoMelhorSolucaoInstanciaTSPMovimentoBuscaLocal(instanciaTSP, saldoNovaSolucao);
                    solucaoAprimorada = 1;

                    break;
                }
            }

        }

        if(solucaoAprimorada){
            break;
        }
    }

    return OK;
}

int executarBuscaLocalMelhorAprimorante(InstanciaTSP* instanciaTSP){
    double custoSolucaoAntiga = -1, custoSolucaoNova = -1;
    int statusOperacao = -1;


imprimirGrafo(instanciaTSP->grafo);



    do{



printf("---------------------------- NOVA ITERAÇÃO ----------------------------\n");



        custoSolucaoAntiga = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
        statusOperacao = encontrarMelhorAprimoranteBuscaLocalMovimento2Otimo(instanciaTSP);
        custoSolucaoNova = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
        
        if(statusOperacao != OK){
            return statusOperacao;
        }
    }while(custoSolucaoNova < custoSolucaoAntiga);
    

imprimirTour(instanciaTSP->melhorSolucao);


    return OK;
}

int encontrarMelhorAprimoranteBuscaLocalMovimento2Otimo(InstanciaTSP* instanciaTSP){
    int numeroCidades = getDimensaoInstanciaTSP(instanciaTSP);

    //Verificando se é possível fazer movimentos 2-ótimo nesse grafo
    if(numeroCidades <= 3){
        return OK;
    }

    NoTour* melhorSolucao = getMelhorSolucaoInstanciaTSP(instanciaTSP);
    double custoSolucaoAtual = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
    
    //Apesar de nesse trabalho serem executadas apenas instâncias de TSP
    //não havendo instâncias de ATSP, optei por nomear as variáveis abaixo
    //"noTourOrigemAresta", "cidadeOrigemAresta", "noTourDestinoAresta" e "cidadeDestinoAresta"
    //para ficar mais claro como os dois loops for abaixo trabalham
    NoTour *noTourOrigemAresta1 = NULL, *noTourDestinoAresta1 = NULL;
    NoTour *noTourOrigemAresta2 = NULL, *noTourDestinoAresta2 = NULL;
    int cidadeOrigemAresta1 = -1, cidadeDestinoAresta1 = -1, melhorCidadeOrigemAresta1 = -1;
    int cidadeOrigemAresta2 = -1, cidadeDestinoAresta2 = -1, melhorCidadeOrigemAresta2 = -1;
    double pesoAresta1 = -1, pesoAresta2 = -1, pesoNovaAresta1 = -1, pesoNovaAresta2 = -1, saldoNovaSolucao = 0, melhorSaldoNovaSolucao = 0;
    for(int i = 0; i < numeroCidades; i++){
        noTourOrigemAresta1 = getNoTourPosicao(melhorSolucao, i);
        noTourDestinoAresta1 = getNoTourPosicao(melhorSolucao, i+1);
        if(noTourOrigemAresta1 == NULL || noTourDestinoAresta1 == NULL){
            return ERRO_RECUPERAR_NO_TOUR;
        }
        cidadeOrigemAresta1 = getIdTour(noTourOrigemAresta1);
        cidadeDestinoAresta1 = getIdTour(noTourDestinoAresta1);

        for(int j = i + 2; j < numeroCidades; j++){
            noTourOrigemAresta2 = getNoTourPosicao(melhorSolucao, j);
            noTourDestinoAresta2 = getNoTourPosicao(melhorSolucao, j+1);
            if(noTourOrigemAresta2 == NULL || noTourDestinoAresta2 == NULL){
                return ERRO_RECUPERAR_NO_TOUR;
            }
            
            cidadeOrigemAresta2 = getIdTour(noTourOrigemAresta2);
            cidadeDestinoAresta2 = getIdTour(noTourDestinoAresta2);

            //Testando se as cidades escolhidas correspondem às duas primeiras
            //e às duas últimas do tour. Nesse caso, as arestas são consecutivas
            //e não é possível realizar o movimento 2-ótimo
            if(cidadeOrigemAresta1 == cidadeDestinoAresta2){
                continue;
            }
            
            //Recuperando pesos das arestas selecionadas para serem substituidas
            //e peso das novas arestas obtidas com o movimento 2-ótimo

            //Devido ao modo como os loops for estão estruturados, temos que,
            //ao removermos as duas arestas selecionadas do tour, este passará
            //a ser um grafo com duas componentes conexas, sendo que em uma delas
            //estarão as cidades "cidadeDestinoAresta1" e "cidadeOrigemAresta2"
            //enquanto na outra se encontrarão as cidades "cidadeOrigemAresta1" e "cidadeDestinoAresta2"
            //Logo, as duas novas arestas adicionadas seriam
            //("cidadeDestinoAresta1", "cidadeDestinoAresta2") e ("cidadeOrigemAresta1", "cidadeOrigemAresta2");

            pesoAresta1 = getPesoArestaGrafo(getGrafoInstanciaTSP(instanciaTSP), cidadeOrigemAresta1, cidadeDestinoAresta1);
            pesoAresta2 = getPesoArestaGrafo(getGrafoInstanciaTSP(instanciaTSP), cidadeOrigemAresta2, cidadeDestinoAresta2);
            if(pesoAresta1 == -1 || pesoAresta2 == -1){
                return ERRO_RECUPERAR_PESO_ARESTA_GRAFO;
            }

            pesoNovaAresta1 = getPesoArestaGrafo(getGrafoInstanciaTSP(instanciaTSP), cidadeOrigemAresta1, cidadeOrigemAresta2);
            pesoNovaAresta2 = getPesoArestaGrafo(getGrafoInstanciaTSP(instanciaTSP), cidadeDestinoAresta1, cidadeDestinoAresta2);
            //Como os grafos deste trabalho são completos, a ocorrência do erro abaixo seria inesperada
            //pois as novas arestas deveriam existir no grafo
            if(pesoNovaAresta1 == -1 || pesoNovaAresta2 == -1){
                return ERRO_RECUPERAR_PESO_ARESTA_GRAFO;
            }

            //Calculando saldo da nova solução
            saldoNovaSolucao = pesoAresta1 + pesoAresta2 - pesoNovaAresta1 - pesoNovaAresta2;


printf("Cidades iniciais da troca: %d %d    Saldo: %lf\n", cidadeOrigemAresta1, cidadeOrigemAresta2, saldoNovaSolucao);


            if(saldoNovaSolucao > melhorSaldoNovaSolucao){


printf("\nSolução Melhor Encontrada!\n\n");



                melhorSaldoNovaSolucao = saldoNovaSolucao;
                melhorCidadeOrigemAresta1 = cidadeOrigemAresta1;
                melhorCidadeOrigemAresta2 = cidadeOrigemAresta2;
            }
        }
    }

    if(melhorSaldoNovaSolucao > 0){



printf("\nSolução Atualizada!\n\n");



        if(realizarMovimento2otimo(melhorSolucao, melhorCidadeOrigemAresta1, melhorCidadeOrigemAresta2) != OK){
            return ERRO_REALIZAR_MOVIMENTO_2_OTIMO;
        }

        atualizarCustoMelhorSolucaoInstanciaTSPMovimentoBuscaLocal(instanciaTSP, melhorSaldoNovaSolucao);
    }

    return OK;
}