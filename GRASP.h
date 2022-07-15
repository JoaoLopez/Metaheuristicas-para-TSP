#include "BuscaLocal.h"
#include "TADHeuristicas.h"
#include "TADInstanciaTSP.h"
#include "TADMetricas.h"
#include "TADTour.h"
#include "TADErro.h"
#include <time.h>

int executarGRASP(InstanciaTSP* instanciaTSP, Metricas* metricas, int numeroRepeticoes, double alpha);
