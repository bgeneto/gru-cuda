#ifndef MC_CALC_H
#define MC_CALC_H

#include "mc.h"

double energia_particula( t_sistema *sis, long int x );
double energia_local( t_sistema *sis, long int x );

void calcula_energia_inicial(t_sistema *sis); 
/* Por consistencia nao seria melhor ter uma única função que calcula a energia total do sistema e o cálculo da energia  inicial 
 * apenas seria uma atribuição?
*/
double calcula_deslocamento_quadratico(t_sistema *sis);
t_media_dados *calcula_media_dados(t_simulacao *sim);

t_dados *simula_mc(t_simulacao *sim, int n);

#endif /* MC_CALC_H */
