#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tabelaPreditiva.h"
#include "analisadorLexico.h"

#define TERMINAIS "id+*()"
#define T_STR 512

typedef struct {
    char* Valor;
    struct No* Prox;
} No;

typedef struct {
    char* Topo;
    struct No* Cauda;
} Pilha;

char* entrada[1][3] = {{"id", "+", "id"}};
int colunaAtual = 0;

Pilha* Inicializar() {
    Pilha* p = (Pilha*) malloc(1 * sizeof(Pilha));
    p->Cauda = NULL;
    p->Topo = NULL;
    return p;
}

void Empilhar(Pilha* pilha, char* valor) {
    if(pilha->Topo == NULL) {
        pilha->Cauda = NULL;
    } else {
        No* novoNo = (No*) malloc(1 * sizeof(No));
        novoNo->Valor = (char*) malloc(sizeof(pilha->Topo) + 1);
        strcpy(novoNo->Valor, pilha->Topo);
        novoNo->Prox = pilha->Cauda;
        pilha->Cauda = novoNo;
    }
    pilha->Topo = (char*) malloc(sizeof(valor) + 1);
    strcpy(pilha->Topo, valor);
}

void Desempilhar(Pilha* pilha) {
    if(pilha->Topo == NULL) {
    } else if (pilha->Cauda == NULL) {
        pilha->Topo = NULL;
    } else {
        No* cauda = (No*) malloc(sizeof(No));
        cauda = pilha->Cauda;
        pilha->Topo = (char*) malloc(sizeof(cauda->Valor) + 1);
        strcpy(pilha->Topo, cauda->Valor);
        pilha->Cauda = cauda->Prox;
        free(cauda);
    }
}

void PrintPilha(Pilha* p) {
    No *aux = p->Cauda;
    printf("Topo: %s\n", p->Topo);
    while(aux != NULL) {
        printf("Cauda: %s\n", aux->Valor);
        aux = aux->Prox;
    }
    free(aux);
}

char* Tabela(char* topoPilha, char* entrada) {
    int linha = Busca(topoPilha)->posicao;
    int coluna = Busca(entrada)->posicao;
    return tabela[linha][coluna];
}

void Erro(char* token) {
    printf("Sintaxe erro: %s\n", token);
}

void AnalisadorSintaticoPreditivo(Pilha* pilha) {
    Token token = ObterToken();
    int t_producao, i;
    while(strstr(pilha->Topo, "$") == NULL) {
        if (strstr(pilha->Topo, token) != NULL) {
            Desempilhar(pilha);
            token = ObterToken();
        } else if (strstr(TERMINAIS, pilha->Topo) != NULL) {
            Erro(token);
            break;
        } else if (strstr(Tabela(pilha->Topo, token), "0") != NULL) {
            printf("ERRO\n");
            Erro(token);
            break;
        } else {
            char producao[T_STR];
            char* simbolo = (char*) malloc(1 * sizeof(producao));
            Pilha* ordem = Inicializar();
            strcpy(producao, Tabela(pilha->Topo, token));

            Desempilhar(pilha);

            simbolo = strtok(producao, "|");
            while(simbolo != NULL) {
                Empilhar(ordem, simbolo);
                simbolo = strtok(NULL, "|");
            }

            while(ordem->Topo != NULL) {
                if (strstr(ordem->Topo, "e") == NULL) {
                    Empilhar(pilha, ordem->Topo);
                }
                Desempilhar(ordem);
            }
            free(ordem);
            free(simbolo);
        }
    }
    if (strstr(pilha->Topo, "$") != NULL && strstr(ObterToken(), "$") == NULL) {
        Erro(token);
    }
}

void main(){
    Pilha* pilha = Inicializar();

    ///Inicia a pilha com o simbolo inicial da gramatica
    Empilhar(pilha, "$");
    Empilhar(pilha, "goal_symbol");

    InicializarTabelaHash();
//    AnalisadorSintaticoPreditivo(pilha);
}
