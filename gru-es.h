#ifndef GRU_ES_H
#define GRU_ES_H

#include "mc.h"

void inicializa_parametros_padronizados(t_parametros *p);

t_simulacao *prepara_simulacao(int argc, char *argv[]);
void le_parametros_gru(int argc, char *argv[], t_parametros *par);
int detecta_erro_parametros_simulacao(t_parametros *par);
void ajuda_gru();
void versao_gru();
void gera_relatorio(int argc, char *argv[], t_simulacao *sim);

#endif // GRU_ES_H
