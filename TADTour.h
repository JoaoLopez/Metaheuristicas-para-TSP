#ifndef TADTOUR_H
#define TADTOUR_H

typedef struct verticeTour{
    int id;
    struct verticeTour* proximoVerticeTour;
}VerticeTour;

#endif