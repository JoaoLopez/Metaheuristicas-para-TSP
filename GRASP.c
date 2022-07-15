#include "GRASP.h"

int executarGRASP(InstanciaTSP* instanciaTSP, int numeroRepeticoes, double alpha){
    NoTour* melhorSolucao = NULL;
    double melhorCusto = -1;

    //Verificando se a instância TSP já possui uma solução conhecida
    //Nesse caso ela será armazenada em duas variáveis externas, pois ao
    //gerarmos as novas soluções do GRASP, as funções alterarão a variável
    //"instanciaTSP"
    if(getMelhorSolucaoInstanciaTSP(instanciaTSP) != NULL){
        melhorSolucao = getMelhorSolucaoInstanciaTSP(instanciaTSP);
        melhorCusto = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);

        removerSolucaoInstanciaTSP(instanciaTSP);
    }


printf("Custo Solução inicial: %lf\n", melhorCusto);
printf("Solução Inicial:\n");
imprimirTour(melhorSolucao);


    int statusOperacao = -1;
    for(int i = 0; i < numeroRepeticoes; i++){



printf("------------------------------- Nova Iteração -------------------------------\n");



        statusOperacao = solucionarHeuristicaVizinhoMaisProximoDoisLadosRandomizada(instanciaTSP, alpha);
        if(statusOperacao != OK){
            deletarTour(melhorSolucao);
            return ERRO_GERAR_SOLUCAO_INSTANCIA_TSP;
        }
        
        statusOperacao = executarBuscaLocalMelhorAprimorante(instanciaTSP);
        if(statusOperacao != OK){
            deletarTour(melhorSolucao);
            deletarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP));
            removerSolucaoInstanciaTSP(instanciaTSP);
            return ERRO_EXECUTAR_BUSCA_LOCAL_INSTANCIA_TSP;
        }


printf("Custo Nova Solução: %lf\n", getCustoMelhorSolucaoInstanciaTSP(instanciaTSP));


        if(getCustoMelhorSolucaoInstanciaTSP(instanciaTSP) < melhorCusto ||
        melhorCusto == -1){
            if(melhorSolucao != NULL){
                deletarTour(melhorSolucao);
            }

            melhorCusto = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
            melhorSolucao = getMelhorSolucaoInstanciaTSP(instanciaTSP);


printf("\nMelhor Solução Atualizada!\n");
printf("Nova Solução:\n");
imprimirTour(melhorSolucao);
printf("\n");



        }
        else{
            deletarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP));
        }

        //Resetando solução de instância TSP
        //Necessário pois a heurística construtiva só sobrescreverá a solução
        //de instanciaTSP se encontrar uma solução melhor.
        removerSolucaoInstanciaTSP(instanciaTSP);
    }

printf("------------------------------- FIM -------------------------------\n");
printf("Custo da Melhor Solução Encontrada: %lf\n", melhorCusto);
printf("Melhor Solução Encontrada:\n");
imprimirTour(melhorSolucao);


    setMelhorSolucaoInstanciaTSP(instanciaTSP, melhorSolucao);
    return OK;
}