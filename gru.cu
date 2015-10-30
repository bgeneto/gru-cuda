/* GRU (Gás de rede unidimensional)
 * Autor do código principal: MARCO AURÉLIO A. BARBOSA (aureliobarbosa@gmail.com)
 * Paralelização: BERNHARD ENDERS
 *
 *
 * O programa realiza simulações de Monte Carlo usando o critério de Metrópolis em uma rede unidimensional
 * com partículas com caroço duro e interações de primeiros e segundos vizinhos.
 *
 * DETALHES DO CÓDIGO
 * - Note que as posições na rede estão indexadas de 0 a L-1, mas que as partículas estão indexadas de 1 a N
 *
 * * Checar o arquivo detalhes-codigo.txt para mais informações
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "mc.h"
#include "mc-ies.h"
#include "mc-calc.h"
#include "gru-es.h"
#include "mytools.h"

//TODO ESTUDAR DOXYGEN
//TODO Checar se acesso via algebra de ponteiros é muito mais rápido que acesso matricial
int main (int argc, char *argv[])
{
    t_simulacao *sim;
    t_dados *dados;

    sim = prepara_simulacao( argc, argv ); // Apenas está preparando o sistema para realizar várias simulações

    int i;
    #pragma omp parallel for
    for (i=0;i < sim->numero_sistemas; i++)
    {
        dados = simula_mc( sim, i );
        //imprime_dados( dados, sim->arq_simulacao);
    }

    if (sim->numero_sistemas > 1)
    {
      calcula_media_dados( sim );
      imprime_media_dados( sim -> media_dados, sim->arq_simulacao );
    }

    exit(EXIT_SUCCESS);;
}


