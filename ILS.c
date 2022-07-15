#include "ILS.h"

int executarILS(InstanciaTSP* instanciaTSP, int numeroRepeticoes, double alpha){
    NoTour *melhorSolucao = NULL;
    double melhorCusto = -1;

/*
printf("Grafo\n");
imprimirGrafo(instanciaTSP->grafo);
printf("Grafo\n");
*/
    //Obtendo uma solução inicial
    int statusOperacao = -1;
    if(getMelhorSolucaoInstanciaTSP(instanciaTSP) == NULL){
        statusOperacao = solucionarHeuristicaVizinhoMaisProximoDoisLadosRandomizada(instanciaTSP, alpha);
        if(statusOperacao != OK)
            return ERRO_GERAR_SOLUCAO_INSTANCIA_TSP;
        statusOperacao = executarBuscaLocalMelhorAprimorante(instanciaTSP);
        if(statusOperacao != OK){
            deletarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP));
            removerSolucaoInstanciaTSP(instanciaTSP);
            return ERRO_EXECUTAR_BUSCA_LOCAL_INSTANCIA_TSP;
        }
    }
    melhorSolucao = copiarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP), &statusOperacao);
    melhorCusto = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
    if(statusOperacao != OK){
        deletarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP));
        removerSolucaoInstanciaTSP(instanciaTSP);
        return ERRO_COPIAR_TOUR;
    }

/*    
printf("Custo Solução Inicial: %lf\n", melhorCusto);
printf("Solução Inicial:\n");
imprimirTour(melhorSolucao);
*/

    //solucao1 e custo1 armazenam a solução que o ILS está utilizando como base
    NoTour *solucao1 = melhorSolucao, *solucoes_antigas[5], *aux;
    double custo1 = melhorCusto, custos_antigos[5];
    int repeticoes_sem_melhora = 0;
    for(int i = 0; i < numeroRepeticoes; i++){
        perturbarSolucao(instanciaTSP);

/*
printf("-------------------------- NOVA ITERAÇÃO --------------------------\n");
printf("Custo da Pertubação: %lf\n", getCustoMelhorSolucaoInstanciaTSP(instanciaTSP));
*/
        statusOperacao = executarBuscaLocalMelhorAprimorante(instanciaTSP);

/*
printf("Custo da Nova Solução: %lf\n", getCustoMelhorSolucaoInstanciaTSP(instanciaTSP));
*/

        if(statusOperacao != OK){
            deletarTour(melhorSolucao);
            if(melhorSolucao != solucao1)
                deletarTour(solucao1);
            for(int j=0; j < repeticoes_sem_melhora; j++)
                deletarTour(solucoes_antigas[j]);
            removerSolucaoInstanciaTSP(instanciaTSP);
            return ERRO_EXECUTAR_BUSCA_LOCAL_INSTANCIA_TSP;
        }

        //Se a nova solução for a melhor obtida até agora será a utilizada na próxima perturbação
        if(getCustoMelhorSolucaoInstanciaTSP(instanciaTSP) < melhorCusto){
/*
printf("\nSolução Melhor Encontrada!\n");
imprimirTour(instanciaTSP->melhorSolucao);
*/

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



/*
printf("\nCusto da Próxima Solução Perturbada: %lf\n", custo1);
*/



        }
        //Se a nova solução não foi a melhor de todas e ainda há espaço no histórico,
        //então ela é armazenada no histórico. A solução perturbada na próxima iteração
        //será a mesma que foi perturbada nessa iteração
        else if(repeticoes_sem_melhora < 5){
/*
printf("Solução Adicionada ao Histórico\n");
*/

            solucoes_antigas[repeticoes_sem_melhora] = getMelhorSolucaoInstanciaTSP(instanciaTSP);
            custos_antigos[repeticoes_sem_melhora] = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
            repeticoes_sem_melhora++;
        }
        //Se a nova solução não foi a melhor de todas e não há mais espaço no histórico,
        //então a melhor solução do histórico será escolhida para ser perturbada
        else{
/*
printf("Atualização com Piora da Solução Base\n");
*/

            double temp = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
            NoTour* temp2 = getMelhorSolucaoInstanciaTSP(instanciaTSP);
            for(int j=0; j < repeticoes_sem_melhora; j++){
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

/*
printf("\nCusto da Próxima Solução Perturbada: %lf\n", custo1);
*/


        }
        aux = copiarTour(solucao1, &statusOperacao);
        if(statusOperacao != OK){
            deletarTour(melhorSolucao);
            if(melhorSolucao != solucao1)
                deletarTour(solucao1);
            for(int j=0; j < repeticoes_sem_melhora; j++)
                deletarTour(solucoes_antigas[j]);
            removerSolucaoInstanciaTSP(instanciaTSP);
            return ERRO_COPIAR_TOUR;
        }
        removerSolucaoInstanciaTSP(instanciaTSP);
        setMelhorSolucaoInstanciaTSP(instanciaTSP, aux);
    }

    if(solucao1 != melhorSolucao)
        deletarTour(solucao1);
    for(int j=0; j < repeticoes_sem_melhora; j++)
        deletarTour(solucoes_antigas[j]);
    removerSolucaoInstanciaTSP(instanciaTSP);
    setMelhorSolucaoInstanciaTSP(instanciaTSP, melhorSolucao);
    return OK;
}

void perturbarSolucao(InstanciaTSP* instanciaTSP){
    int numCidades = getDimensaoInstanciaTSP(instanciaTSP);
    double porcentModificacao = sortearNumeroAleatorio(10, 30)/100.0;
    int numModificacoes = (int) (numCidades*porcentModificacao);
    int arestasSorteadas[numModificacoes];
    NoTour* subtoursGerados[numModificacoes+1];
    sortearNumerosAleatoriosDistintos(numModificacoes, 1, numCidades, arestasSorteadas);
    ordenar_vetor(arestasSorteadas, numModificacoes);
    deletarArestasTour(getMelhorSolucaoInstanciaTSP(instanciaTSP), arestasSorteadas, numModificacoes, subtoursGerados);
    int subtour_i, subtour_j, lim_sup = numModificacoes;
    NoTour* aux;
    while(lim_sup > 2){
        //O primeiro e o último subtours precisam ser unidos sempre à esquerda e à direita
        //respectivamente. Por isso são excluidos das uniões aleatórias
        subtour_i = lim_sup-1;
        subtour_j = sortearNumeroAleatorio(1, lim_sup-2);
        if(sortearNumeroAleatorio(0, 1)){
            aux = unirSubTours(subtoursGerados[subtour_i], subtoursGerados[subtour_j]);
        }
        else{
            aux = unirSubTours(subtoursGerados[subtour_j], subtoursGerados[subtour_i]);
        }
        subtoursGerados[subtour_j] = aux;
        lim_sup -= 1;
    }

    aux = unirSubTours(subtoursGerados[0], subtoursGerados[1]);
    if(numModificacoes > 1)
        aux = unirSubTours(aux, subtoursGerados[numModificacoes]);

    removerSolucaoInstanciaTSP(instanciaTSP);
    setMelhorSolucaoInstanciaTSP(instanciaTSP, aux);
    return;
}
