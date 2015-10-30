// GRU ENTRADA E SAÍDA.
#include "gru-es.h"

#include "mc.h"
#include "mc-ies.h"
#include "mc-calc.h"
#include "mytools.h"

#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#define GRU_VERSAO "2015.10.15_2"

/* específico GRU
 O código está ficando meio feio novamente. Possivelmente será interessante criar uma estrutura para receber os parâmetros da simulação e depois função
prepara_simulacao chama as funcoes apropriadas uma única vez, com os parâmetros certos.

 */
void inicializa_parametros_padronizados(t_parametros *p)
{
    // arquivo de saída e relatório
    p -> arq_simulacao = stdout;
    p -> relatorio = 0;

    // Configurações padrão da simulação
    p -> numero_sistemas = 1;
    p -> pmc = 10000;
    p -> imp = 10;
    p -> imprime_configuracao = 0;

    // Configurações padrão do sistema - Gas de rede apenas com interações atrativas entre primeiros vizinhos
    p -> semente = time(NULL);

    p -> e1 = -1.0;  // TODO Checar notação científica -1E0 ?
    p -> e2 = 0.0;
    p -> t  = 0.4;
    p -> L  = 100;
    p -> N  = 72;
}

t_simulacao *prepara_simulacao(int argc, char *argv[])
{
	t_simulacao *sim;
    t_sistema *s;

    t_parametros *p = calloc( 1, sizeof(t_parametros) );

    inicializa_parametros_padronizados( p );
    le_parametros_gru ( argc, argv, p );

    // Preparando simulação
    size_t numero_simulacoes = 1;
    sim = aloca_memoria_simulacao( numero_simulacoes );
    // inicializa_simulacao_padrao ( sim );
    // Dados da simulação // Parâmetros padronizados
    sim -> pmc             = p -> pmc;
    sim -> imp             = p -> imp;
    sim -> arq_simulacao   = p -> arq_simulacao;
    sim -> numero_sistemas = p -> numero_sistemas;
    sim -> relatorio       = p -> relatorio;
    sim -> imprime_configuracao = p -> imprime_configuracao;

    /** ATENÇÃO: cada passo corresponde a uma tentativa de movimento por partícula,
     *           por isso o número de passos de Monte Carlo e o número de passos entre impressões
     *           são multiplicados por N
     */
    sim->pmc = sim->pmc*p->N;
    sim->imp = sim->imp*p->N;

    sim->media_dados = aloca_memoria_media_dados( sim->pmc / sim->imp );

    // Operações específicas de um sistema a ser simulado!
    int i;
    s = aloca_memoria_sistema( sim -> numero_sistemas );
    sim -> sis = s;
    srand (time(NULL)); // a semente será inicializada novamente mais à frente, para cada simulação.
    for ( i=0; i < sim -> numero_sistemas; i++ )
    {
        //        inicializa_sistema_padrao ( s );
        // Note que cada sistema tem um conjunto de dados associados a ele...
        s -> dados = aloca_memoria_dados( sim->pmc / sim->imp );
        s -> L  =  p -> L;
        s -> N  =  p -> N;
        s -> t  =  p -> t;
        s -> e1 =  p -> e1;
        s -> e2 =  p -> e2;

        aloca_vetores_sistema ( s );
        gera_configuracao_aleatoria ( s );
        calcula_energia_inicial( s);

        s++;  // TODO Transferir para dentro do for
    }

	if ( sim-> relatorio ) gera_relatorio( argc, argv, sim );

	return (sim);
}

// TODO: implementar checagem de erro nos parâmetros (getopt_long faz isso, não?)
void le_parametros_gru(int argc, char *argv[], t_parametros *par)
{
	int i;

	for ( i = 1; i < argc ; i += 2)
		switch(argv[i][1])
		{
            // Opções do sistema
			case 'L':  // tamanho da rede
				par->L = atoi( argv[i+1] );
				break;
			case 'N': // numero de particulas na rede
				par->N = atoi( argv[i+1] );
				break;
            case 'P': // interacao com o segundo vizinho
                par->e2 = atof(argv[i+1]);
                break;
            case 'W': // interacao com o primeiro vizinho
                par->e1 = atof(argv[i+1]);
                break;
            case 'T': // temperatura
                par->t = atof( argv[i+1] );
                break;
            case 'S': // random semente
                par->semente = atoi( argv[i+1] );
                srand (par->semente);
                break;

            // Opções de simulação
            case 'n':
                par->numero_sistemas = atoi( argv[i+1] );
                break;
			case 't': // tempo de simulacao
				par->pmc = atoi( argv[i+1] );
				break;
			case 's': // tempo entre gravações
				par->imp = atoi( argv[i+1] );
				break;
            // FUNÇÃO DESABILITADA
// 			case 'g': // habilita a impressão das configuracões
// 				par->imprime_configuracao = 1; //
//                 i--;
// 				break;
            case 'a':
                par->arq_simulacao = fopen(argv[i+1], "w"); // Isso aqui pode dar erro!!!!
                break;

            // Opções de exibição:
			case 'r':
				par->relatorio = 1;
				i--;
				break;
			case 'h':
				ajuda_gru();
				exit(EXIT_SUCCESS);
				break;
			default:
                versao_gru(stderr);
				fprintf(stderr, "Erro: O parâmetro de entrada %s não existe! Encerrando o programa.\n",argv[i]);
				exit(EXIT_FAILURE);
		}

		detecta_erro_parametros_simulacao ( par );
}

// TODO melhorar a nomenclatura dos parâmetros
void ajuda_gru()
{
    versao_gru(stdout);
	fprintf( stdout,
	"Opções do sistema:\n"
	"  -L 99999   Tamanho da rede (Por padrão é 100).\n"
	"  -N 99999   Numero de particulas na rede (Por padrão é 50).\n"
	"  -P 9.999   Interação com segundos vizinhos (Pode ser atrativa ou repulsiva. Por padrão é nula, 0.0).\n"
	"  -W 9.999   Interação com primeiros vizinhos (Pode ser atrativa ou repulsiva. Por padrão é -1.0).\n"
	"  -T 9.999   Temperatura (Por padrão é 1.0).\n"
    "  -S 999     Semente Aleatória (Por padrão utiliza o tempo do sistema).\n\n"
	""
	"Opções da simulação:\n"

    "  -n 99      Simula n cópias do sistema e calcula valores médios ao final (Por padrão n=1).\n"
    "  -t 99999   Número total de passos.\n"
	"  -s 999     Passos entre impressões (Por padrão imprime sempre). \n"
//	"  -g         Habilita a impressão das configurações da rede (Por padrao é desabilitada).\n"
    "  -a x.dat   Nome do arquivo de saida.\n\n"
    ""
	"Opções de exibição:\n"
	"  -r         Gera um relatório no início da simulação.\n"
	"  -h         Imprime esta ajuda.\n\n");

    //        TODO ""  -i arq     Arquivo com a conformacao inicial ( por ser implementada ). Conformacao aleatoria por padrao.\n"
}

void versao_gru(FILE *arq)
{
    fprintf(arq,
            "# GRU - Simulação de Monte Carlo para o Gas de Rede Unidimensional. LCC/PPG-CIMA/FUP/UnB.\n"
            "# Versão: %s\n\n", GRU_VERSAO);
//            "Autor: Marco A. A. Barbosa - aureliobarbosa@gmail.com\n\n"  );   // Em breve teremos mais um autor!
}

int detecta_erro_parametros_simulacao(t_parametros *par)
{
	if ( par->imp > par->pmc )
	{
        versao_gru(stderr);
		fprintf(stderr,"Erro: Tempo de gravação maior do que o tempo de impressão!\n");
		exit(EXIT_FAILURE);
	}

	if (par->N >= par->L+1)
	{
        versao_gru(stderr);
		fprintf(stderr, "Erro: O número de partículas é maior ou igual ao tamanho da rede!\n");
		exit(EXIT_FAILURE);
	}

	return 1;
}

// TODO Incluir data, separa relatório inicial e final... indicar tempo de execução!
void gera_relatorio(int argc, char *argv[], t_simulacao *sim)
{
    versao_gru(sim->arq_simulacao);
	fprintf(sim->arq_simulacao,	"# Parâmetros da simulação:\n# ");

	print_program_opts(sim->arq_simulacao,argc,argv); // REIMPRIME OS PARAMETROS DE SIMULAÇÃO

	fprintf(sim->arq_simulacao,"\n\n");
}
