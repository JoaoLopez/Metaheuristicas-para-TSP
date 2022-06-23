#include "BuscaLocal.h"
#include "TADHeuristicas.h"
#include "TADInstanciaTSP.h"
#include "TADTour.h"
#include "TADErro.h"

int executarILS(InstanciaTSP* instanciaTSP, int numeroRepeticoes);

void perturbarSolucao(InstanciaTSP* instanciaTSP);