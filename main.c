/****************************************************************************/
/*                   Trabalho Estrutura de Dados I                          */
/*    MAIN.C                                                                */
/*                                                                          */
/*         Tiago Emanuel Rodrigues   - 0041191                              */
/*                                                                          */
/****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TadArvore.h"
#define length_Buffer 32

void ordem(char *comand, No *no);

void printFile(FILE *arq);

int main(int argc, char *argv[]) {
    FILE *arq_entr;
    FILE *arq_saida;

    arq_entr = fopen("teste.txt", "r");
    arq_saida = fopen("saida.txt", "w");

    AVL *avl = InitiAvl();
    Fila *fila = InitFila();
    int num=100;
    printf("Tamanho da Hash\n");
   // scanf("%d", &num);
    int abrehash = funcaoHash(num);
    Hash *hash;
    inicializaHash(hash);

    // Vetor de char, para ler do arquivo de entrada.
    char *line = (char *) malloc(sizeof(char) * length_Buffer); //criando a linha
    rewind(arq_entr); //Posiciona o cursor no inicio do arquivo
    fgets(line, length_Buffer - 1, arq_entr);

    while (!feof(arq_entr)) {
        // Quebra a linha em tokens.
        line = strtok(line, "\n ");
        // compara os tokens com os comandos.
        if ((strcmp(line, "FIM") == 0)) {
            printf("FIM\n");
            fclose(arq_saida);
            break;
        }
        if (strcmp(line, "PROCURA") == 0) {
            line = strtok(NULL, " ");
            // Adiciona o elemento.
            addElemento(avl, line); // arvore
            addFila(fila, line);  //fila
            insereHash(hash, atoi(line)); //hash

            //ordem(line, avl->raiz);
            // Imprime o caminho feito no arquivo.
            printFile(arq_saida);
            // Desaloca pra proxima chamada.
            free(vetOrdem);
            vetOrdem = NULL;
            length   = 0;
            fclose(arq_saida);
        }
        fgets(line, length_Buffer - 1, arq_entr);
    }
    return 0;
}

/*
  Procedimento que chama o procedimento de caminho, alocando no
    vetor vetOrdem a ordem chamada.
*/
void ordem(char *comand, No *no) {
    if (strcmp(comand, "PREORDEM") == 0) {  // PreOrdem
        printf("Imprime PREORDEM\n");
        preOrdem(no);
    } else if (strcmp(comand, "INORDEM") == 0) {  // InOrdem
        printf("imprime INORDEM\n");
        inOrdem(no);
    } else if (strcmp(comand, "POSORDEM") == 0) { // PosOrdem
        printf("imprime POSORDEM\n");
        posOrdem(no);
    }
}

/*
   Procedimento que escreve os caminho no arquivo no padrão estabelecido.
*/
void printFile(FILE *arq) {
    fprintf(arq, "<");
    // caso não exista nos na arvore
    if (length == 0) {
        fprintf(arq, ">\n");
        return;
    }
    for (int i = 0; i < length; i++) {
        if (i != (length - 1))
            fprintf(arq, "%d-", vetOrdem[i]);
        else
            fprintf(arq, "%d>\n", vetOrdem[i]);
    }
}