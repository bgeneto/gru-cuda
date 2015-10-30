#ifndef MC_IES_H
#define MC_IES_H

#include "mc.h"

// TODO Ser� que vale a pena fazer checagem de erro na aloca��o de mem�ria?

//Manipula��o de estruturas b�sicas
t_simulacao *aloca_memoria_simulacao(size_t n);

t_dados *aloca_memoria_dados(size_t n);
t_media_dados *aloca_memoria_media_dados(size_t n);

t_sistema *aloca_memoria_sistema(size_t n);


void aloca_vetores_sistema ( t_sistema *sis );
void libera_vetores_sistema ( t_sistema *sis );

// Entrada e sa�da...
void gera_configuracao_aleatoria(t_sistema *sim);
void imprime_configuracao(t_sistema *sis, FILE *arq);
void imprime_dados(t_dados *d, FILE *arq);
void imprime_media_dados(t_media_dados *md, FILE *arq);

#endif /* MC_IES_H */

// Fun��es retiradas do c�digo
// void inicializa_sistema_padrao ( t_sistema *sis ); // Talvez seja desnecess�rio...
// void inicializa_simulacao_padrao(t_simulacao *sim); // talvez seja desnecess�ria...
