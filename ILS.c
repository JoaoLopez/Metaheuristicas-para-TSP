#include "ILS.h"

int executarILS(InstanciaTSP* instanciaTSP, int numeroRepeticoes){
    NoTour *melhorSolucao = NULL, *solucao1, *solucoes_antigas[5], *aux;
    double melhorCusto = -1, custo1, custos_antigos[5];

    //Obtendo uma solução inicial
    int statusOperacao = -1, repeticoes_sem_melhora = 0;
    if(getMelhorSolucaoInstanciaTSP(instanciaTSP) != NULL){
        melhorSolucao = copiarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP), &statusOperacao);
        melhorCusto = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
        if(statusOperacao != OK){
            deletarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP));
            apagarSolucaoInstanciaTSP(instanciaTSP);
            return ERRO_COPIAR_TOUR;
        }
    }
    else{
        statusOperacao = solucionarHeuristicaVizinhoMaisProximoDoisLados(instanciaTSP);
        if(statusOperacao != OK)
            return ERRO_GERAR_SOLUCAO_INSTANCIA_TSP;
        statusOperacao = executarBuscaLocalMelhorAprimorante(instanciaTSP);
        if(statusOperacao != OK){
            deletarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP));
            apagarSolucaoInstanciaTSP(instanciaTSP);
            return ERRO_EXECUTAR_BUSCA_LOCAL_INSTANCIA_TSP;
        }
        melhorSolucao = copiarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP), &statusOperacao);
        melhorCusto = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
        if(statusOperacao != OK){
            deletarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP));
            apagarSolucaoInstanciaTSP(instanciaTSP);
            return ERRO_COPIAR_TOUR;
        }
    }
    
    //solucao1 e custo 1 armazenam a solução que o ILS está utilizando como base
    solucao1 = melhorSolucao;
    custo1 = melhorCusto;
    for(int i = 0; i < numeroRepeticoes; i++){
        perturbarSolucao(instanciaTSP);
        statusOperacao = executarBuscaLocalMelhorAprimorante(instanciaTSP);
        if(statusOperacao != OK){
            deletarTour(melhorSolucao);
            if(melhorSolucao != solucao1)
                deletarTour(solucao1);
            for(int j=0; j < repeticoes_sem_melhora; j++)
                deletarTour(solucoes_antigas[j]);
            apagarSolucaoInstanciaTSP(instanciaTSP);
            return ERRO_EXECUTAR_BUSCA_LOCAL_INSTANCIA_TSP;
        }
        //Se a nova solução for a melhor obtida até agora será a utilizada na próxima perturbação
        if(getCustoMelhorSolucaoInstanciaTSP(instanciaTSP) < melhorCusto){
            printf("Melhorei\n");

            deletarTour(melhorSolucao);
            if(solucao1 != melhorSolucao)
                deletarTour(solucao1);
            for(int j=0; j < repeticoes_sem_melhora; j++)
                deletarTour(solucoes_antigas[j]);
            repeticoes_sem_melhora = 0;

            melhorSolucao = getMelhorSolucaoInstanciaTSP(instanciaTSP);
            melhorCusto = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
            solucao1 = melhorSolucao;
            custo1 = melhorCusto;
        }
        //Se a nova solução não foi a melhor de todas e ainda há espaço no histórico,
        //então ela é armazenada no histórico e solucao1 e a mesma solução anterior será
        //perturbada novamente
        else if(repeticoes_sem_melhora < 5){
            printf("Histórico\n");

            solucoes_antigas[repeticoes_sem_melhora] = getMelhorSolucaoInstanciaTSP(instanciaTSP);
            custos_antigos[repeticoes_sem_melhora] = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
            repeticoes_sem_melhora++;
        }
        //Se a nova solução não foi a melhor de todas e não há mais espaço no histórico,
        //então a melhor solução do histórico será escolhida para ser perturbada
        else{
            printf("Piorei\n");

            int temp = custos_antigos[0];
            NoTour* temp2 = solucoes_antigas[0];
            for(int j=1; j < repeticoes_sem_melhora; j++){
                if(custos_antigos[j] < temp){
                    deletarTour(temp2);
                    temp = custos_antigos[j];
                    temp2 = solucoes_antigas[j];
                }
                else{
                    deletarTour(solucoes_antigas[j]);
                }
            }
            repeticoes_sem_melhora = 0;
            
            if(solucao1 != melhorSolucao)
                deletarTour(solucao1);
            custo1 = temp;
            solucao1 = temp2;
        }
        aux = copiarTour(solucao1, &statusOperacao);
        if(statusOperacao != OK){
            deletarTour(melhorSolucao);
            if(melhorSolucao != solucao1)
                deletarTour(solucao1);
            for(int j=0; j < repeticoes_sem_melhora; j++)
                deletarTour(solucoes_antigas[j]);
            apagarSolucaoInstanciaTSP(instanciaTSP);
            return ERRO_COPIAR_TOUR;
        }
        apagarSolucaoInstanciaTSP(instanciaTSP);
        setMelhorSolucaoInstanciaTSP(instanciaTSP, aux);
    }
    return OK;

    if(solucao1 != melhorSolucao)
        deletarTour(solucao1);
    for(int j=0; j < repeticoes_sem_melhora; j++)
        deletarTour(solucoes_antigas[j]);
    apagarSolucaoInstanciaTSP(instanciaTSP);
    setMelhorSolucaoInstanciaTSP(instanciaTSP, melhorSolucao);
    return OK;
}

void perturbarSolucao(InstanciaTSP* instanciaTSP){
    double porcent_modificacao = sortearNumeroAleatorio(10, 30)/100.0;
    int num_modificacoes = (int) (instanciaTSP->dimensao*porcent_modificacao);
    int arestas_sorteadas[num_modificacoes];
    NoTour* subtours_gerados[num_modificacoes+1];
    sortearNumerosAleatoriosDistintos(num_modificacoes, 1, instanciaTSP->dimensao, arestas_sorteadas);
    ordenar_vetor(arestas_sorteadas, num_modificacoes);
    deletarArestasTour(instanciaTSP->melhorSolucao, arestas_sorteadas, num_modificacoes, subtours_gerados);
    int subtour_i, subtour_j, lim_sup = num_modificacoes;
    NoTour* aux;
    
    /*
    printf("porcent: %lf\n", porcent_modificacao);
    printf("num_modificacoes: %d\n", num_modificacoes);
    for(int i=0; i < num_modificacoes+1; i++){
        printf("Imprimindo subtour_gerado[%d]:\n", i);
        imprimirTour(subtours_gerados[i]);
        printf("\n\n");
    }
    */
    while(lim_sup > 2){
        //O primeiro e o último subtours precisam ser unidos sempre à esquerda e à direita
        //respectivamente. Por isso são excluidos das uniões aleatórias
        subtour_i = lim_sup-1;
        subtour_j = sortearNumeroAleatorio(1, lim_sup-2);
        if(sortearNumeroAleatorio(0, 1)){
            aux = unirSubTours(subtours_gerados[subtour_i], subtours_gerados[subtour_j]);
        }
        else{
            aux = unirSubTours(subtours_gerados[subtour_j], subtours_gerados[subtour_i]);
        }
        subtours_gerados[subtour_j] = aux;
        lim_sup -= 1;
        
        /*
        printf("i: %d\n", subtour_i);
        printf("j: %d\n\n", subtour_j);

        for(int i=0; i < lim_sup+1; i++){
            printf("Imprimindo subtour_gerado[%d]:\n", i);
            imprimirTour(subtours_gerados[i]);
            printf("\n\n");
        }
        */
    }
    /*
    imprimirTour(subtours_gerados[0]);
    printf("\n\n");
    imprimirTour(subtours_gerados[1]);
    printf("\n\n");
    imprimirTour(subtours_gerados[num_modificacoes]);
    */

    aux = unirSubTours(subtours_gerados[0], subtours_gerados[1]);
    aux = unirSubTours(aux, subtours_gerados[num_modificacoes]);

    /*
    imprimirTour(aux);
    printf("\n\n");
    imprimirTour(instanciaTSP->melhorSolucao);
    */
    return;
}
