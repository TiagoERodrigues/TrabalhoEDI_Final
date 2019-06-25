/****************************************************************************/
/*                   Trabalho Estrutura de Dados I                          */
/*    TADARVORE.H                                                           */
/*                                                                          */
/*         Tiago Emanuel Rodrigues   - 0041191                              */
/*                                                                          */
/****************************************************************************/

#ifndef TADARVORE
#define  TADARVORE

#include <stdlib.h>
#include <stdio.h>

#define tam 100

typedef struct arvore AVL;      // Rename da Struct arvore.
typedef struct no No;       // Rename da Struct no.
typedef struct fila Fila;

struct arvore {
    No *raiz;       // Aponta pra raiz da arvore.
};

struct no {
    char dado;       // Dado armazenado.
    No *SAE, *SAD;      //  Sub-Arvores ("Filhos").
    short int balanceamento;       // Fator de balanceamento.
    unsigned int altura;        //  Altura do no em relação ao folha mais distante.
};

typedef struct aux {

    char dado;
    struct aux *prox;

} *apont;

struct fila {
    apont inicio;
    apont fim;
    int tamanho;
};

typedef struct aux HASH;
typedef struct HASH *Hash[tam];

int *vetOrdem;      // Vetor de retorno dos percursos (preOrdem, inOrdem, posOrdem).
int length;     //  Comprimento do vetor acima.
/*
 * AVL
 */
AVL *InitiAvl();

void addElemento(AVL *arv, char dado);

void rmElemento(AVL *arv, char dado);

int buscaAVL(No *no, char dado);

void calcAltBalac(No *no);

void preOrdem(No *no);

void posOrdem(No *no);

void inOrdem(No *no);

void addVet(No *no);

No *verificaBalac(No *no, No *raiz, char direct);

No *rotaEsq(No *no);

No *rotaDir(No *no);

/*
 * FILA
 */
Fila *InitFila();

void addFila(Fila *fila, char dado);

void removeFila(Fila *fila, char dado);

int tamanhoFila(Fila *fila);

void mostra(Fila *fila, char dado);

void zeraFila(Fila *fila);

/*
 * HASH
 */

int funcaoHash(int num);/**/
void inicializaHash(Hash tab); /**/
void insereHash(Hash tab, int num);

void buscaHash(Hash tab, int num);

void imprimeHash(Hash tab);

void removeHash(Hash tab, int num);

void imprimeColisao(Hash hash, int pos);

#endif