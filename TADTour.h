#ifndef TADTOUR_H
#define TADTOUR_H

typedef struct noTour{
    int id;
    struct noTour* proximoNoTour;
}NoTour;

void deletarTour(NoTour* tour);

#endif