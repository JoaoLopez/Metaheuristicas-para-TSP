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
    }


    int statusOperacao = -1;
    for(int i = 0; i < numeroRepeticoes; i++){
        statusOperacao = solucionarHeuristicaVizinhoMaisProximoDoisLadosRandomizada(instanciaTSP, alpha);
        if(statusOperacao != OK){
            deletarTour(melhorSolucao);
            return ERRO_GERAR_SOLUCAO_INSTANCIA_TSP;
        }
        
        statusOperacao = executarBuscaLocalMelhorAprimorante(instanciaTSP);
        if(statusOperacao != OK){
            deletarTour(melhorSolucao);
            deletarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP));
            apagarSolucaoInstanciaTSP(instanciaTSP);
            return ERRO_EXECUTAR_BUSCA_LOCAL_INSTANCIA_TSP;
        }



        ///////////////DEBUG
        printf("MELHOR SOLUÇÃO ENCONTRADA:\n");
        printf("Custo: %lf\n", instanciaTSP->custoMelhorSolucao);
        imprimirTour(instanciaTSP->melhorSolucao);
        printf("\nMELHOR SOLUÇÃO ATÉ AGORA:\n");
        printf("Custo: %lf\n", melhorCusto);
        printf("\n\n\n");
        ///////////////////////////////




        if(getCustoMelhorSolucaoInstanciaTSP(instanciaTSP) < melhorCusto ||
        melhorCusto == -1){
            if(melhorSolucao != NULL){
                deletarTour(melhorSolucao);
            }

            melhorCusto = getCustoMelhorSolucaoInstanciaTSP(instanciaTSP);
            melhorSolucao = getMelhorSolucaoInstanciaTSP(instanciaTSP);
        }
        else{
            deletarTour(getMelhorSolucaoInstanciaTSP(instanciaTSP));
        }

        //Resetando solução de instância TSP
        //Necessário pois a heurística construtiva só sobrescreverá a solução
        //de instanciaTSP se encntrar uma solução melhor.
        apagarSolucaoInstanciaTSP(instanciaTSP);
    }

    setMelhorSolucaoInstanciaTSP(instanciaTSP, melhorSolucao);
    return OK;
}