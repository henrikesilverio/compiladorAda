#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tabelaPreditiva.h"
#include "AnaliseLexica.h"

#ifndef AnaliseSintatica
#endif // AnaliseSintatica
#define T_STR 2048

typedef struct {
    char* Valor;
    struct No* Prox;
} No;

typedef struct {
    char* Topo;
    struct No* Cauda;
} Pilha;

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
        novoNo->Valor = (char*) malloc(strlen(pilha->Topo) + 1);
        strcpy(novoNo->Valor, pilha->Topo);
        novoNo->Prox = pilha->Cauda;
        pilha->Cauda = novoNo;
    }
    pilha->Topo = (char*) malloc(strlen(valor) + 1);
    strcpy(pilha->Topo, valor);
}

void Desempilhar(Pilha* pilha) {
    if(pilha->Topo == NULL) {
    } else if (pilha->Cauda == NULL) {
        pilha->Topo = NULL;
    } else {
        No* cauda = (No*) malloc(sizeof(No));
        cauda = pilha->Cauda;
        pilha->Topo = (char*) malloc(strlen(cauda->Valor) + 1);
        strcpy(pilha->Topo, cauda->Valor);
        pilha->Cauda = cauda->Prox;
        free(cauda);
    }
}

char* Tabela(char* topoPilha, char* entrada) {
    int linha = Busca(topoPilha)->posicao;
    int coluna = Busca(entrada)->posicao;
    return tabela[linha][coluna];
}

int EhTerminal(char* simbolo) {
    int i;
    for(i = 0; i < t_terminais; i++) {
        if (strstr(terminais[i], simbolo) != NULL) {
            return 1;
        } else {
            continue;
        }
    }
    return 0;
}

void EmpilharProducaoEmOrdem(char* producao, Pilha* pilha) {
    int i, j = 0, t = strlen(producao);
    char* buffer = (char*) malloc(strlen(producao) + 1);
    for(i = 0; i < t; i++) {
        if (producao[i] != '|') {
            buffer[j++] = producao[i];
            if (i == (t - 1)) {
                buffer[j] = '\0';
                Empilhar(pilha, buffer);
                j = 0;
            }
            continue;
        }
        buffer[j] = '\0';
        Empilhar(pilha, buffer);
        j = 0;
    }
}

void Erro(Token token, char* mensagem, int linha, int coluna) {
    printf("Sintaxe erro! %s, token: %s, linha: %d, coluna: %d\n", mensagem, token.palavra, linha, coluna);
}

void AnalisadorSintaticoPreditivo() {
    Pilha* pilha = Inicializar();

    ///Inicia a pilha com o simbolo inicial da gramatica
    Empilhar(pilha, "$");
    Empilhar(pilha, "compilation");

    InicializarTabelaHash();

    Token token = ObterToken(arquivo, &linha, &coluna);

    int t_producao, i;
    while(strstr(pilha->Topo, "$") == NULL) {
        if (strstr(pilha->Topo, token.simboloTerminal) != NULL) {
            Desempilhar(pilha);
            token = ObterToken(arquivo, &linha, &coluna);
        } else if (EhTerminal(pilha->Topo)) {
            Erro(token, "Topo terminal", token.linha, token.coluna);
            break;
        } else if (strstr(Tabela(pilha->Topo, token.simboloTerminal), "ERRO") != NULL) {
            Erro(token, "Sequência incompleta", token.linha, token.coluna);
            break;
        } else {
            Pilha* ordem = Inicializar();
            EmpilharProducaoEmOrdem(Tabela(pilha->Topo, token.simboloTerminal), ordem);

            Desempilhar(pilha);

            while(ordem->Topo != NULL) {
                if (strstr(ordem->Topo, "epsilon") == NULL) {
                    Empilhar(pilha, ordem->Topo);
                }
                Desempilhar(ordem);
            }
            free(ordem);
        }
    }
    if (strstr(pilha->Topo, "$") != NULL && strstr(ObterToken(arquivo, linha, coluna).palavra, "$") == NULL) {
        Erro(token, "Sequência invalida", token.linha, token.coluna);
    }
}
