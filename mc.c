#include "mc.h"

// TODO Transferir as funções de salto de manipulação de números aleatórios para "myrand.c"

long int saltos[4] = {-2,-1,1,2}; // bgeneto: renamed from 'salto' to 'saltos'

long int sorteia_numero_salto ( long int alcance )
{
    return (long int) floor ( (double) 2.0*alcance*RAND );
}

long int sorteia_salto ( long int alcance )
{
    return saltos[ sorteia_numero_salto( alcance ) ];
}

long int sorteia_particula ( long int N)
{
    return floor( (double) N*RAND ) + 1;
}
