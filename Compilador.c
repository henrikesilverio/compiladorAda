#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "AnaliseSintatica.h"

void PrintPilha(Pilha* p) {
    No *aux = p->Cauda;
    printf("Topo: %s\n", p->Topo);
    while(aux != NULL) {
        printf("Cauda: %s\n", aux->Valor);
        aux = aux->Prox;
    }
    free(aux);
}

int main(int argc, char **argv) {
    int opcao;
    int index;
    linha = 1;
    coluna = 1;
    extern char *optarg;
    opterr = 0;
    while((opcao = getopt(argc, argv, "hc:")) -1) {
        switch(opcao) {
            case 'h':
                printf("--------------------------------------------------\n");
                printf("\tCompilador para liguaguem Ada\n");
                printf("--------------------------------------------------\n");
                printf("Utilizaçao: ./compilandor [opção] caminho do arquivo\n");
                printf("Opções:\n");
                printf(" -Opção -h: ajuda\n");
                printf(" -Opção -c: compilar Ex: ./compilandor -c helloworld.adb\n");
                printf("--------------------------------------------------\n");
                break;
            case 'c':
                printf("\n");
                arquivo = fopen(optarg, "r");
                if(arquivo == NULL) {
                    printf("Nao foi possivel abrir o arquivo.\n");
                } else {
                    printf("\tCompilando\n");
                    int index;
//                    InicializarTabelaHash();
//                    printf("%s\n", Tabela("TIC", ")"));
                    AnalisadorSintaticoPreditivo();
//                    InicializarTabelaHash();
//                    Pilha *p = Inicializar();
//                    EmpilharProducaoEmOrdem(Tabela("c_with_clause_use_clause", "WITH"), p);
//                    PrintPilha(p);
//                    for(index = 0; index < 20; index++) {
//                        Token token;
//                        token = ObterToken(arquivo, &linha, &coluna);
//                        printf("Simbolo Terminal %s\n", token.simboloTerminal);
//                        printf("Tipo %d\n", token.tipo);
//                        if (token.tipo == TOKEN_INVALIDO){
//                            printf("Token %s invalido linha: %d coluna: %d\n", token.palavra, token.linha, token.coluna);
//                        }
//                    }
                    fclose(arquivo);
                }
                break;
            case '?':
                if (optopt == 'c') {
                    fprintf (stderr, "Opção -%c requer um argumento.\n", optopt);
                } else if (isprint (optopt)) {
                    fprintf (stderr, "Opção desconhecida '-%c'.\n", optopt);
                } else {
                    fprintf(stderr, "Caractere de opção desconhecido '\\x%x'.\n", optopt);
                }
                return 1;
            default:
                abort();
        }
    }
    return 0;
}
