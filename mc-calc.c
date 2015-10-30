#include "mc-calc.h"
#include <time.h>

//Cálculos no sistema

t_dados *simula_mc(t_simulacao *sim, int n)
{
    t_sistema *sis = &(sim->sis[n]); // Checar se esta é a forma "mais correta" de se fazer referência

    srand(time(NULL));

    double enei, enef, dene;
    long int i, l, part, salto, x, xv;
    for (i=0; i < sim->pmc; i++) // corrida
    {
        // sorteio da particula
        part = (long int) sorteia_particula(sis->N);
        x = sis->p[part]; // descobre a posição de part

        // sorteio do salto a ser dado
        salto = (long int) sorteia_salto(ALCANCE_SALTO);

        // posição do salto, com condições periódicas de contorno
        xv =  (x + salto + sis->L )%sis->L;

        if ( !sis->r[xv] ) // NÃO COLIDIU
        {
            // criterio de Metropolis
            enei = energia_local( sis, x );
            sis->r[xv] = sis->r[x];
            sis->r[x]  = 0;
            enef = energia_local(sis, x);
            dene = enef - enei;

            if ( RAND > exp(-dene/sis->t) )     // Não passou por Metrópolis
            {
                sis->r[x] = sis->r[xv];
                sis->r[xv] = 0;
            }
            else                                // Passou por Metrópolis  -- > Atualizacao dos valores
            {
                sis->p[part] = xv;
                sis->d[part] += salto;
                sis->energia += dene;
            }
        }

        if ( !( i % sim->imp ) ) // Condição para armazenador informações da simulação em dados.
        {
            l = i / sim->imp;

            sis->dados->tempo[l] = i/sis->N;
            sis->dados->dqm[l] = calcula_deslocamento_quadratico (sis); // TODO trocar acesso matricial por acesso via incremento de ponteiros. será que vale a pena?
            sis->dados->energia[l] = sis->energia;
            sis->dados->deltae[l] = dene;
        }

    } // fim do laco da corrida

    return (sis->dados);
}

//VERSÃO ANTIGA, QUE DÁ CERTO!!!!
double energia_particula( t_sistema *sis, long int x )
// mudar o parametro de entrada... pedir o número da partícula, ao invés de pedir a posição dela
// Generalizar a interação com um vetor de interações... é só retirar o for, colocando um while...
{
    double ene = 0.0;
    long int xv, i;

//    x = sis->p[part];

    for ( i=-1; i<2; i++ )
    {
        if (!i) continue;
        xv =  (x + i + sis->L )%sis->L;
        if ( sis->r[xv] )
            ene +=  sis->e1;
        else if ( sis->e2 != 0.000000000 ) // se o sitio estiver vazio passa para o segundo vizinho, se houver interação
        {
            xv =  (x + 2*i + sis->L )%sis->L;
            if (sis->r[xv]) ene += sis->e2;
        }
    }

    return ene;
}

// VERSÃO NOVA, DÁ ERRADO!!!!
// double energia_particula( t_sistema *sis, long int part ) // CHECAR O QUE DEU ERRADO AQUI!!!
// // TODO Generalizar a interação com um vetor de interações...
// // TODO Checar se o algoritmo nessa versão não atrapalha a paralelização em CUDA!
// {
//  double ene = 0.0;
//  long int x, xv, i, sinal;
//
//     x = sis->p[part];
//
//     for (sinal=-1; sinal<2;sinal+=2) // Caminhar para a esquerda (-1) ou para a direita (+1)
//     {
//         i = 0;
//         do
//         {
//             i++;
//             xv =  (x + sinal*i + sis->L )%sis->L;
//         } while ( !sis->r[xv] ); // Possivelmente o while é problemático em CUDA...
//
//         if ( i == 1 ) ene +=  sis->e1; // Esse trecho pode ser trocado por uma linha se for implementado um vetor de interações...
//         else
//         {
//             if ( i == 2 ) ene += sis->e2;
//         }
//     }
//  return ene;
// }

// Energia na vizinhança da partícula no sítio x, considerando a região onde a partícula pode interagir após o salto...
// TODO CUDA: Esta função pode ser modificada para fazer uma leitura linear, calculando a energia em torno de uma vizinhança
double energia_local( t_sistema *sis, long int x ) // Checar se não seria possível reduzir a vizinhança em que é necessário calcular as energias... para otimizar o código (?).
{
	double ene = 0.0;
	long int i, xv;
	for ( i=-ALCANCE_SALTO-ALCANCE_INT; i<ALCANCE_SALTO+ALCANCE_INT+1; i++ )
	{
		xv =  (x + i + sis->L )%sis->L;
		if ( sis->r[xv] ) ene += energia_particula(sis,xv)/2.0;
	}
	return ene;
}

void calcula_energia_inicial( t_sistema *sis )
{
	int part;
	sis->energia = 0.0;

	for ( part = 1; part < (sis->N+1); part++ ) sis->energia += energia_particula( sis, sis->p[part] )/2.0;
}


// TODO A princípio está tudo ok, mas vale a pena checar as condições para o cálculo do deslocamento quadrático médio explodir!!!!
double calcula_deslocamento_quadratico(t_sistema *sis)
{
    long int i, x2;

    x2 = 0; // Deslocamento quadrático médio
    for ( i=1; i < sis->N+1 ; i++ )
    {
        x2 += sis->d[i]*sis->d[i]; // será que nao estamos tendo erros numericos explosões aqui??
    }
    return (double) x2 / sis->N;

}

/* Esta funçaõ calcula a média dos dados simulados em diversos sistemas e armazena
em uma estrutura t_media_dados dentro da estrutura sim */
t_media_dados *calcula_media_dados(t_simulacao *sim)
{
    t_dados *dados;
    t_sistema *sis;
    t_media_dados *media;

    double energia_media_sistema;
    int i, j;

    sis   = sim -> sis;
    media = sim -> media_dados;

    media -> np = sis -> dados -> np;
    // as inicializações abaixo são desnecessárias porque a memória é alocada com valores nulos.
//     media -> energia = 0;
//     media -> calor_especifico_v = 0;
//     media -> D = 0;
//
//     for ( j=0; j < dados -> np; j++ ) media -> dqm[j] = 0;

    for ( i=0; i < sim->numero_sistemas; i++ )
    {
        dados = sis -> dados; // trocar por matrizes... aqui
        energia_media_sistema = 0;

        for ( j=0; j < dados -> np; j++ )
        {
            energia_media_sistema += dados -> energia[j];
            media -> dqm [j] += dados -> dqm [j];
        }

        media -> energia += energia_media_sistema / dados -> np;

        if( i != sim->numero_sistemas-1) sis++; // trocar por matrizes... e aqui.
    }

    media -> energia /= sim -> numero_sistemas;

    dados = sim -> sis -> dados;
    for ( j=0; j < dados -> np; j++ )
    {
        media -> tempo[j] = dados -> tempo [j];
        media -> dqm[j] /= sim -> numero_sistemas;
    }
    return (media);
}

// t_dados *calcula_media_dados(t_simulacao *sim)
// {
//
//     int i;
//     t_dados *d,
//     t_media_dados *md;
//
//     d = sim->sis->dados;
//     md = sim->media_dados;
//
//     for (i=1;i < d->np; i++)
//     {
//
//         d++;
//     }
//
//
//     return sim->media_dados;
// }
