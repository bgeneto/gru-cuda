#include "mc-ies.h"

#include <stdlib.h>
#include <time.h>

t_simulacao *aloca_memoria_simulacao(size_t n)
{
	t_simulacao *sim;
	sim = ( t_simulacao * ) calloc( n, sizeof ( t_simulacao ) );
	return (sim);
}


t_sistema *aloca_memoria_sistema(size_t n)
{
	t_sistema *sis;

    sis = ( t_sistema * )   calloc( n, sizeof ( t_sistema ) );
	return (sis);
}

t_dados *aloca_memoria_dados(size_t n)
{
    t_dados *dados;

    dados          = ( t_dados * ) calloc( 1, sizeof ( t_dados ) );

    dados->np      = n;
    dados->tempo   = (long int *) calloc( n, sizeof (long int) );
    dados->energia = (double *) calloc( n, sizeof (double) );
    dados->deltae  = (double *) calloc( n, sizeof (double) );
    dados->dqm     = (double *) calloc( n, sizeof (double) );

    return (dados);
}

t_media_dados *aloca_memoria_media_dados( size_t n )
{
    t_media_dados *md;
    
    md          = ( t_media_dados * ) calloc( 1, sizeof ( t_media_dados ) );
    
    // Retirar estas alocações de memória após incluir o cálculo da difusão.
    md->np      = n; 
    md -> tempo = (long int *) calloc( n, sizeof (long int) );    
    md->dqm     = (double *) calloc( n, sizeof (double) );
    
    return (md);
}

void aloca_vetores_sistema ( t_sistema *sis )
{
	sis->r = (long int *) calloc( sis->L + 1, sizeof( long int ) ); // Checar se dá para trocar o +1 por 0 e o +2 por +1 (+1 é necessário porque o indice 0 refere-se aos buracos), neste código.
	sis->p = (long int *) calloc( sis->N + 2, sizeof( long int ) );
	sis->d = (long int *) calloc( sis->N + 2, sizeof( long int ) ); // Este é o único vetor que realmente precisa ser ( long int *). Checar se é possível converter os outros para (int).
}

void libera_vetores_sistema ( t_sistema *sis )
{
	free(sis->r);
	free(sis->p);
	free(sis->d);
}

// Note que esta função é extremamente não otimizada... para altas densidades ela roda for - ever
void gera_configuracao_aleatoria(t_sistema *sis)
{
	long int i, x;

	for ( i=1; i<=sis->N; i++ )
	{
		do
		{
			x = floor( ( double ) sis->L*RAND ); // transformar em MACRO ou em função inline!!!
		} while ( sis->r[x] );

		sis->r[x] = i;
		sis->p[i] = x;
	}
}

// Impressão de dados
void imprime_dados(t_dados *d, FILE *arq)
{
    int i;

    fprintf(arq,"# tempo  energia  deltae   dqm \n");
    for ( i=0; i < d->np; i++ )
    {
        fprintf(arq,"%ld %.6f %.6f %.6f \n", d->tempo[i], d->energia[i], d->deltae[i], d->dqm[i]);
        //  if ( sim->imprime_configuracao ) print_lint_vec( sim->arq_simulacao, sis->r, sis->L ); // TODO Reimplementar impressão de configuração - usar outra função
    }
    fprintf( arq, "\n\n" );
}

void imprime_media_dados(t_media_dados *md, FILE *arq)
{
    int i;
    
    fprintf(arq,"# t  <energia>   <dqm(t)>   - Média sobre simulações diferentes. \n");  
    for ( i = 0; i < md -> np; i++ )     
        fprintf(arq,"%ld %.6f %.6f \n", md -> tempo[i], md -> energia, md -> dqm[i] );
    fprintf( arq, "\n\n" );
}

// FUNÇÕES EXCLUIDAS!!!!
// void inicializa_sistema_padrao ( t_sistema *sis )
// {
//  // Configurações padrão do sistema - Gas de rede apenas com interações atrativas entre primeiros vizinhos
//     sis -> semente = time(NULL);
//     srand(sis -> semente);
//     
//  sis -> e1 = -1.0;  // TODO Checar notação científica -1E0 ?
//  sis -> e2 = 0.0;
//  sis -> t  = 0.4;
//  sis -> L  = 100;
//  sis -> N  = 72;
// }


// void inicializa_simulacao_padrao(t_simulacao *sim)
// {
//  // arquivo de saída e relatório
//  sim -> arq_simulacao = stdout;
//  sim -> relatorio = 0;
// 
//  // Configurações padrão da simulação
//     sim -> numero_sistemas = 1;
//  sim -> pmc = 10000;
//     sim -> imp = 10;
//     sim -> imprime_configuracao = 0;   
//     
// }







//   TRECHO DE CODIGO PARA IMPORTAR CONFIGURAÇÃO INICIAL

//   if ( !indicou_configuracao_inicial )
//    gera_configuracao_aleatoria( );
//   else
//   {
//     if ( !le_configuracao_inicial(sis.r, nome_arq_inicial) )
//     {
//       printf("O arquivo com a configuracao inicial: %s, esta com algum problema!\n", nome_arq_inicial);
//       exit(0);
//     }
//   }
