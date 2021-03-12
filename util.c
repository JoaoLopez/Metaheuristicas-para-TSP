#include "util.h"

int sortearNumeroAleatorio(int valorMinimo, int valorMaximo){
    return (rand() % (valorMaximo - valorMinimo + 1)) + valorMinimo;
}

void Tempo_CPU_Sistema(double *seg_CPU_total, double *seg_sistema_total){

  long seg_CPU, seg_sistema, mseg_CPU, mseg_sistema;
  struct rusage ptempo;

  getrusage(0,&ptempo);

  seg_CPU = ptempo.ru_utime.tv_sec;
  mseg_CPU = ptempo.ru_utime.tv_usec;
  seg_sistema = ptempo.ru_stime.tv_sec;
  mseg_sistema = ptempo.ru_stime.tv_usec;

 *seg_CPU_total     = (seg_CPU + 0.000001 * mseg_CPU);
 *seg_sistema_total = (seg_sistema + 0.000001 * mseg_sistema);
}
