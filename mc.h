#ifndef MC_H
#define MC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RAND (((double) rand() )/(RAND_MAX+1.0))

#define ALCANCE_INT 2 // Note que estas funções são intermediárias... quando a interação for generalizada elas desaparecem!!!
#define ALCANCE_SALTO 2
#define INTERVALO_METROPOLIS ALCANCE_SALTO + ALCANCE_INT

// TODO Eliminar as variávels ALCANCE_INT INTERVALO_METROPOLIS ALCANCE_SALTO

// Deve ter algo mais inteligente a se fazer, mas isso está simplificando minha vida!!!!!
// Seria interessante setar uma variável que informa se um parâmetro foi informado pelo usuário ou não!!!!
typedef struct  
{    
    unsigned int semente;     // semente aleatória
    
    double e1, e2, t; // Parametros do gas de rede
    long int L,           // tamanho da rede
    N;           // numero de particulas    
    
    long int pmc,       // passos de monte carlo
    imp;       // passos entre impressões
    
    FILE  *arq_simulacao;
    
    long int numero_sistemas,        //  Implementado para que existam vários sistemas.
    relatorio,               // Habilita/desabilita a impressão de relatório de simulação
    imprime_configuracao;    // Habilita/desabilita a impressão da configuração do sistema - FUNCIONALIDADE RETIRADA DO CÓDIGO!!!
} t_parametros;



// Estrutura para coletar os dados de uma simulação em um sistema específico
// TODO Incluir configurações (?) - Aparentemente não é necessário!!!!!
typedef struct
{
    long int np,                   // número de pontos
    *tempo;               // aloca np valores de tempo
    double *energia,               // para alocar np valores de energia
    *deltae,                // Aloca np valores de variacao de energia, deltae (em relação ao último passo de Monte Carlo).
    *dqm;                   // para alocar np valores de deslocamento quadrático médio.
} t_dados;

typedef struct
{
    long int np,                   // número de pontos - Retirar apos incluir calculo da constante de difusão
             *tempo;               // np valores de tempo - Retirar também após calcular a difusão.  
    double energia,                // energia média
           calor_especifico_v,     // TODO IMPLEMENTAR CALCULO DO CALOR ESPECÍFICO A VOLUME CONSTANTE
           D,                      // Constante de auto-difusão.
           *dqm;                   // para alocar np valores de deslocamento quadrático médio-médio. TODO RETIRAR ESTA VARIÁVEL APÓS CALCULAR A DIFUSÃO!
} t_media_dados;

// Definição básica de um sistema
typedef struct
{
  int numero_sistema;        // indice do sistema atual

  unsigned int semente;     // semente aleatória
  
  double e1, e2, t, energia; // Parametros do gas de rede
  long int *r, 			     // ponteiro para uma rede unidimensional
       L, 			// tamanho da rede
      *p,  			// ponteiro com as posicoes de cada particula na rede
      *d,  			// deslocamento individual de cada particula desde o início da simulação
       N; 			// numero de particulas
       
       t_dados *dados; 
} t_sistema;

// Definição básica de uma simulação
typedef struct
{    
    long int pmc,       // passos de monte carlo
             imp;       // passos entre impressões

    FILE  *arq_simulacao;

    int numero_sistemas,        //  Implementado para que existam vários sistemas.
        relatorio,               // Habilita/desabilita a impressão de relatório de simulação
        imprime_configuracao;    // Habilita/desabilita a impressão da configuração do sistema - FUNCIONALIDADE RETIRADA DO CÓDIGO!!!

    t_sistema *sis;
    
    t_media_dados *media_dados;

} t_simulacao;

// Criar uma função para inicializar vetor saltos arbitrários...
long int saltos[4];

long int sorteia_numero_salto ( long int alcance );

long int sorteia_salto ( long int alcance );

long int sorteia_particula ( long int N);

#endif /* MC_H */
