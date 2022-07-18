#include "BuscaLocal.h"
#include "TADHeuristicas.h"
#include "TADInstanciaTSP.h"
#include "TADMetricas.h"
#include "TADTour.h"
#include "TADErro.h"
#include <time.h>

int executarILSv2(InstanciaTSP* instanciaTSP, Metricas* metricas, int numeroRepeticoes, double alpha);
int executarILS(InstanciaTSP* instanciaTSP, Metricas* metricas, int numeroRepeticoes, double alpha);
void perturbarSolucao(InstanciaTSP* instanciaTSP);