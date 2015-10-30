#ifndef MC_IES_H
#define MC_IES_H

#include "mc.h"

// TODO Será que vale a pena fazer checagem de erro na alocação de memória?

//Manipulação de estruturas básicas
t_simulacao *aloca_memoria_simulacao(size_t n);

t_dados *aloca_memoria_dados(size_t n);
t_media_dados *aloca_memoria_media_dados(size_t n);

t_sistema *aloca_memoria_sistema(size_t n);


void aloca_vetores_sistema ( t_sistema *sis );
void libera_vetores_sistema ( t_sistema *sis );

// Entrada e saída...
void gera_configuracao_aleatoria(t_sistema *sim);
void imprime_configuracao(t_sistema *sis, FILE *arq);
void imprime_dados(t_dados *d, FILE *arq);
void imprime_media_dados(t_media_dados *md, FILE *arq);

#endif /* MC_IES_H */

// Funções retiradas do código
// void inicializa_sistema_padrao ( t_sistema *sis ); // Talvez seja desnecessário...
// void inicializa_simulacao_padrao(t_simulacao *sim); // talvez seja desnecessária...
