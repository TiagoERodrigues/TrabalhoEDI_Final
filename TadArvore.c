/****************************************************************************/
/*                   Trabalho Estrutura de Dados I                          */
/*    TADARVORE.C                                                           */
/*                                                                          */
/*         Tiago Emanuel Rodrigues   - 0041191                              */
/*                                                                          */
/****************************************************************************/


#include "TadArvore.h"

/*
 *  TREE FUNCTIONS
 */

AVL *InitiAvl() {
    AVL *arv = (AVL *) malloc(sizeof(AVL));
    arv->raiz = NULL;
    return arv;
}

void addElemento(AVL *arv, char dado) {
    No *aux;
    if (arv->raiz) {
        short int flag; //a flag é para verificar se o dado inserido é maior ou menor.
        aux = arv->raiz; //arv->raiz recebe a árvore inteira
        No *temp = aux; //variável temporária que guarda a posição anterior (o pai dele)
        /* Percorre na árvore pra ver onde vai inserir */
        while (aux) {
            temp = aux;
            if (aux->dado >= dado) {
                aux = aux->SAE;
                flag = 0; //insere o nó a esquerda
            } else {
                aux = aux->SAD;
                flag = 1; // insere o nó a direita
            }
        }
        // Alocando mais um nó para o aux
        aux = (No *) malloc(sizeof(No));
        if (flag) {
            temp->SAD = aux;

        } else {
            temp->SAE = aux;

        }
    } else {
        // Caso a árvore seja vazia, irá inserir seu primeiro elemento.
        aux = (No *) malloc(sizeof(No));
        arv->raiz = aux;

    }

    // Inserindo o dado definitivamente
    // Fazendo seus apontadores serem para NULL
    aux->dado = dado;
    aux->SAE = NULL;
    aux->SAD = NULL;
    aux->altura = 0;

    calcAltBalac(arv->raiz);
    arv->raiz = verificaBalac(arv->raiz, arv->raiz, 3);

    return;
}

void rmElemento(AVL *arv, char dado) {
    // caso onde é a raiz o dado a ser removido.
    No *aux = arv->raiz;
    No *temp;
    short int flag = 3;
    // Busca do dado a ser removido.
    while (aux) {
        temp = aux;
        if (aux->dado > dado) {
            aux = aux->SAE;
            flag = 0;

        } else if (aux->dado < dado) {
            aux = aux->SAD;
            flag = 1;

        }
        if (aux)
            if (aux->dado == dado) {
                // Caso onde o dado a ser removido é uma folha completa(sem filhos).
                if ((!aux->SAE) && (!aux->SAD)) {
                    if (flag == 3) {
                        free(arv->raiz);
                        arv->raiz = NULL;
                        return;
                    }
                    if (flag) {
                        temp->SAD = NULL;
                    } else {
                        temp->SAE = NULL;
                    }
                    // Caso onde existe sub-arv (filho) a esq.
                } else if (!aux->SAD) {
                    if (flag) {
                        temp->SAD = aux->SAE;
                    } else {
                        temp->SAE = aux->SAE;
                    }
                    // Caso onde existe sub-arv (filho) a dir.
                } else if (!aux->SAE) {
                    if (flag) {
                        temp->SAD = aux->SAD;
                    } else {
                        temp->SAE = aux->SAD;
                    }
                    // Caso onde existe ambas sub-arv.
                    // Busca o elemento anterior, um apara SAE e busca a SAD
                } else {
                    temp = aux; // temp e aux são o no que sera excluido
                    // verifica se existe SAD da SAE do no de exclusão
                    if (aux->SAE->SAD) {
                        aux = aux->SAE; // desloca para SAE
                        while (1) {
                            // Faz busca profunda na SAD
                            // Ate antes do ultimo no
                            if (aux->SAD->SAD)
                                aux = aux->SAD; // caminhamento na SAD
                            else
                                break;  // Sai do aprofundamento quando chegar antes do ultimo
                        }
                        // Troca os dados
                        temp->dado = aux->SAD->dado;
                        // Faz a conexão da arvore, se nao existir ira virar NULL
                        temp = aux;
                        aux = aux->SAD;
                        temp->SAD = aux->SAE;
                    } else {  // Caso onde não existe SAD na SAE do no a ser removido
                        // Troca os dados
                        temp->dado = aux->SAE->dado;
                        // Faz a conexão da arvore, se nao existir ira virar NULL
                        aux = aux->SAE;
                        temp->SAE = aux->SAE;
                    }
                    // Desaloca para ambos caso
                }
                free(aux);
                calcAltBalac(arv->raiz);
                arv->raiz = verificaBalac(arv->raiz, arv->raiz, 3);
                return;
            }
    } // end while
    return;
}

/*
  Busca binaria na arvore, recursivamente
  retorna 1 se não encontrar ou 0 se encontrar elemento
*/
int buscaAVL(No *no, char dado) {
    if (!no) {
        return 1;
    }
    if (no->dado == dado) {
        return 0;
    } else if (no->dado > dado) {
        return (buscaAVL(no->SAE, dado));
    } else return (buscaAVL(no->SAD, dado));
}

No *verificaBalac(No *no, No *raiz, char direct) {
    if (!no)
        return NULL;

    /* Caminho em posordem recursivamente setando os fatores de
        balanceamento, os parametros são o no atual da expansão
        o "pai" desse no, e qual o sentido em relação ao pai
          1 = SAD (Sub arvore a direita )
          0 = SAE (Sub arvore a esquerda)
    */
    verificaBalac(no->SAD, no, 1);
    verificaBalac(no->SAE, no, 0);
    // Calcula altura e o fator de balanceamento
    calcAltBalac(raiz);
    /*
        Caso 1 e 2, quando o nó tem um fator maior q 2
        e seu no adjacente adequado a rotação é positivo ou negativo
          se for positivo entao caso 1, rotação a esquerda
          se for negativo entao caso 2, rotacao dupla, direita e depois esquerda
    */
    if (no->balanceamento > 1) {
        if (no->SAD->balanceamento >= 0) {
            // Rotaçao a esquerda, caso 1
            if (direct == 1) {
                raiz->SAD = rotaEsq(no);
            } else if (direct == 0) {
                raiz->SAE = rotaEsq(no);
            } else {
                raiz = rotaEsq(no);
            }
        } else {
            // Rotação dupla, caso 2
            if (direct == 1) {
                raiz->SAD->SAD = rotaDir(no->SAD);
                raiz->SAD = rotaEsq(no);
            } else if (direct == 0) {
                raiz->SAE->SAD = rotaDir(no->SAD);
                raiz->SAE = rotaEsq(no);
            } else {
                raiz->SAD = rotaDir(no->SAD);
                raiz = rotaEsq(no);
            }
        }
        /*
            Caso 3 e 4, quando o nó tem um fator menor q 2
            e seu no adjacente adequado a rotação é positivo ou negativo
              se for negativo entao caso 3, rotação a direita
              se for positivo entao caso 4, rotacao dupla, esquerda depois direita
        */
    } else if (no->balanceamento < -1) {
        if (no->SAE->balanceamento < 0) {
            // rotação a direita, caso 3
            if (direct == 1) {
                raiz->SAD = rotaDir(no);
            } else if (direct == 0) {
                raiz->SAE = rotaDir(no);
            } else {
                raiz = rotaDir(no);
            }
        } else {
            // rotação dupla, caso 4
            if (direct == 1) {
                raiz->SAD->SAE = rotaEsq(no->SAE);
                raiz->SAD = rotaDir(no);
            } else if (direct == 0) {
                raiz->SAE->SAE = rotaEsq(no->SAE);
                raiz->SAE = rotaDir(no);
            } else {
                raiz->SAE = rotaEsq(no->SAE);
                raiz = rotaDir(no);
            }
        }
    }
    calcAltBalac(raiz);
    return raiz;
}

// Rotaçao a Esquerda
No *rotaEsq(No *no) {
    No *auxEsq, *auxDir;
    auxDir = no->SAD;
    auxEsq = auxDir->SAE;
    auxDir->SAE = no;
    no->SAD = auxEsq;
    no = auxDir;
    return no;
}

// Rotaçao a Direita
No *rotaDir(No *no) {
    No *auxEsq, *auxDir;
    auxEsq = no->SAE;
    auxDir = auxEsq->SAD;
    auxEsq->SAD = no;
    no->SAE = auxDir;
    no = auxEsq;
    return no;
}

/* Função que calcula altura e fator de balanceamento
      de cada nó, sendo a altura a distancia entre ele
      e um nó folha, sendo altura nos no folha igual a 0,
      e o fator de balanceamento sendo:
          FB = ALT(SAD) - ALT(SAE)

          FB       = fator de balanceamento
          ALT(SAD) = altura do no a direita
          ALT(SAE) = altura do no a esquerda
*/
void calcAltBalac(No *no) {
    if (!no)
        return;
    // Caminho em posOrdem na arvore
    calcAltBalac(no->SAE);
    calcAltBalac(no->SAD);
    // Caso onde existe ambas sub-arvores
    if ((no->SAE) && (no->SAD)) {
        // Calculo da altura, sendo o no filho de maior altura mais um.
        if (no->SAE->altura >= no->SAD->altura)
            no->altura = no->SAE->altura + 1;
        else
            no->altura = no->SAD->altura + 1;
        // Calculo do balanceamento igual altura do no direita menos o da esquerda.
        no->balanceamento = (no->SAD->altura + 1) - (no->SAE->altura + 1);
        return;
    } else if (no->SAE) {
        /* caso onde existe apenas a sub-arvore a esquerda
            logo altura dessa sub-arvore mais um.
        */
        no->altura = no->SAE->altura + 1;
        // fator igual altura mais um, vezes menos um.
        no->balanceamento = -(no->SAE->altura + 1);
    } else if (no->SAD) {
        /* caso onde existe apenas a sub-arvore a direita
            logo altura dessa sub-arvore mais um.
        */
        no->altura = no->SAD->altura + 1;
        // fator igual altura mais um
        no->balanceamento = no->SAD->altura + 1;
    } else {
        // no folha, altura e fator iguais a zero.
        no->altura = 0;
        no->balanceamento = no->altura;
    }


    return;
}

// caminho em preOrdem
void preOrdem(No *no) {
    if (!no) {
        return;
    }
    printf("dado = %3d, altura = %2d, balac = %1d\n", no->dado, no->altura, no->balanceamento);
    addVet(no);
    preOrdem(no->SAE);
    preOrdem(no->SAD);
    return;
}

// Caminho em posOrdem
void posOrdem(No *no) {
    if (!no) {
        return;
    }
    posOrdem(no->SAE);
    posOrdem(no->SAD);
    printf("dado = %3d, altura = %2d, balac = %1d\n", no->dado, no->altura, no->balanceamento);
    addVet(no);
    return;
}

//caminho em inOrdem
void inOrdem(No *no) {
    if (!no) {
        return;
    }
    inOrdem(no->SAE);
    printf("dado = %3d, altura= %2d, balac = %1d\n", no->dado, no->altura, no->balanceamento);
    addVet(no);
    inOrdem(no->SAD);
    return;
}

/*
  Função que add no ao vetor para chamadas
    de caminho em ordens, assim podendo salvar
    no aquivo em qualquer formato
*/
void addVet(No *no) {
    if (!no) {
        return;
    }
    if (!vetOrdem) {
        vetOrdem = (int *) malloc(sizeof(int));
        length = 1;
        vetOrdem[length - 1] = no->dado;
    } else {
        vetOrdem = (int *) realloc(vetOrdem, (++length) * sizeof(int));
        vetOrdem[length - 1] = no->dado;
    }
}

/*
 *  FILA FUNCTIONS
 */

Fila *InitFila() {
    Fila *fila = (Fila *) malloc(sizeof(Fila));
    fila->inicio = NULL;
    fila->fim = NULL;
}

void addFila(Fila *fila, char dado) {

    apont auxAdd = (Fila *) malloc(sizeof(Fila));
    auxAdd->dado = dado;
    auxAdd->prox = NULL;

    if (fila->inicio == NULL) {
        fila->inicio = auxAdd;

    } else {
        fila->fim->prox = auxAdd;
    }

    fila->tamanho++;
}

void removeFila(Fila *fila, char dado) {

    if (fila == NULL) {
        printf("Sem fila\n");
    }

    dado = fila->inicio->prox;
    apont apagar = fila->inicio;
    fila->inicio = fila->inicio->prox;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }
    free(apagar);
}

int tamanhoFila(Fila *fila) {

    return fila->tamanho;
}

void mostra(Fila *fila, char dado) {

    apont aux = fila->inicio;
    while (aux != NULL) {
        printf("%s\n", aux);
        aux = aux->prox;
    }
}

void zeraFila(Fila *fila) {

    apont aux = fila->inicio; // inicio da Porto
    while (aux != NULL) { //enquanto o auxiliar n for nulo
        apont apagar = aux; // vai apagar o auxiliar
        aux = aux->prox; // vai andar para o proximo
        free(apagar); // desaloca
    }

    fila->inicio = NULL;
    fila->fim = NULL;

    printf("\n\tFila Zerada\n");


}

/*
 * HASH FUNCTIONS
 */

/*funcaoHash  recebe como parametro uma variavel do tipo inteiro(num),
retorna o resto da divisao do valor dessa variavel pela tamanho da tabela*/
int funcaoHash(int num) {
    return (num % tam);
}

//O procedimento inicializaHash
void inicializaHash(Hash hash) {
    int i=0;
    for (i = 0; i < tam; i++) {
        hash[i] = NULL;
    }
}

/*O procedimento insererHash e caso esta posicao ja esteja preenchida,
como colisao esta sendo adotado neste procedimento o encadeamento direto.*/
void insereHash(Hash hash, int num) {
    int i = 0;
    int chave = funcaoHash(num);
    HASH *auxhash = hash[chave];

    while (auxhash != NULL) {
        if (auxhash->dado == num) {
            break;
        }
        auxhash = auxhash->prox;
    }
    if (auxhash == NULL) {
        auxhash = (HASH *) malloc(sizeof(HASH));
        auxhash->dado = num;
        auxhash->prox = hash[chave];
        hash[chave] = auxhash;
    }
}

//O procedimento buscaHash
void buscaHash(Hash hash, int num) {
    int pos = num;

    if (num > tam || num < 0) {
        printf("\nPosicao nao encontrada!");
        return;
    } else {
        imprimeColisao(hash, pos);

    }
}

//O procedimento imprimeHash
void imprimeHash(Hash hash) {
    int i = 0, cont = 0;
    for (i = 0; i < tam; i++) {
        if (hash[i] != NULL) {
            printf("\n %d", hash[i]);
            HASH *aux = hash[i];
            while (aux != NULL) {

                printf(" -> %3d", aux->dado);
                aux = aux->prox;
            }
        }
    }
}

//O procedimento imprimeColisao mostra uma posicao e todas as suas colisoes
void imprimeColisao(Hash hash, int pos) {

    HASH *aux = hash[pos];

    if (aux == NULL) {
        printf("Esta posicao esta vazia!");
        return;
    } else {

        if (aux != NULL) {

            printf("%3d", aux->dado);
            while (aux->prox != NULL) {

                printf(" -> %d", aux->prox->dado);
                aux = aux->prox;
            }
        }
    }
}