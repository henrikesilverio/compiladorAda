#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int Tamanho;
    char** Vetor;
} VetorString;

typedef struct {
    int TamanhoDeAlpha;
    char** Alpha;
    int TamanhoDeBeta;
    char** Beta;
} AlphaBeta;

typedef struct {
    char* Simbolo;
    char** Producoes;
    int numeroDeProducoes;
    struct No* Proximo;
} No;

typedef struct {
    struct No* Primeiro;
} Gramatica;

char* simboloVazio;
VetorString naoTerminais;

void InserirNaoTerminal(char* simbolo) {
    if (naoTerminais.Tamanho == 0) {
        naoTerminais.Vetor = (char**) malloc(sizeof(char*));
    }
    naoTerminais.Vetor[naoTerminais.Tamanho] = (char**) malloc(sizeof(simbolo) + 1);
    strcpy(naoTerminais.Vetor[naoTerminais.Tamanho], simbolo);
    naoTerminais.Tamanho++;
}

Gramatica* Inicializar() {
    Gramatica* g = (Gramatica*) malloc(1 * sizeof(Gramatica));
    g->Primeiro = NULL;
    return g;
}

char* Concatenar(char* destino, char caracter) {
    int tamanho = strlen(destino);
    char* buffer = malloc(tamanho + 2);
    strcpy(buffer, destino);
    buffer[tamanho] = caracter;
    destino = malloc(strlen(buffer) + 1);
    strcpy(destino, buffer);
    return destino;
}

void InserirProducao(Gramatica* gramatica, No* novaProducao) {
    if (gramatica->Primeiro == NULL) {
        gramatica->Primeiro = novaProducao;
    } else {
        No* iterador = gramatica->Primeiro;
        while(iterador->Proximo != NULL) {
            iterador = iterador->Proximo;
        }
        iterador->Proximo = malloc(sizeof(novaProducao));
        iterador->Proximo = novaProducao;
    }
}

void InserirCorposProducao(No* producao, char** corposProducao, int numeroCorposProducao) {
    int i;
    for (i = 0; i < numeroCorposProducao; i++) {
        int posicao = i + producao->numeroDeProducoes;
        producao->Producoes[posicao] = (char*) malloc(strlen(corposProducao[i]) + 1);
        strcpy(producao->Producoes[posicao], corposProducao[i]);
    }
    producao->numeroDeProducoes += i;
}

void AtualizarCorposProducao(No* producao, char** corposProducao, int numeroCorposProducao) {
    int i;
    producao->Producoes = (char**) malloc(1 * numeroCorposProducao);
    for (i = 0; i < numeroCorposProducao; i++) {
        producao->Producoes[i] = (char*) malloc(strlen(corposProducao[i]) + 1);
        strcpy(producao->Producoes[i], corposProducao[i]);
    }
    producao->numeroDeProducoes = numeroCorposProducao;
}

void InserirOuAtualizarProducao(Gramatica* gramatica, char* simbolo, char** corposProducao, int numeroCorposProducao) {
    No* producao = gramatica->Primeiro;
    if (gramatica->Primeiro == NULL) {
        No* novaProducao = (No*) malloc(1 * sizeof(No));
        novaProducao->Producoes = (char**) malloc(1 * sizeof(char*));
        novaProducao->Producoes = corposProducao;
        novaProducao->Simbolo = (char*) malloc(strlen(simbolo) + 1);
        strcpy(novaProducao->Simbolo, simbolo);
        novaProducao->numeroDeProducoes = numeroCorposProducao;
        InserirProducao(gramatica, novaProducao);
    } else {
        while(producao != NULL) {
            if (strcmp(producao->Simbolo, simbolo) == 0) {
                InserirCorposProducao(producao, corposProducao, numeroCorposProducao);
                break;
            }
            producao = producao->Proximo;
        }
        if (producao == NULL) {
            No* novaProducao = (No*) malloc(1 * sizeof(No));
            novaProducao->Producoes = (char**) malloc(1 * sizeof(char*));
            novaProducao->Producoes = corposProducao;
            novaProducao->Simbolo = (char*) malloc(strlen(simbolo) + 1);
            strcpy(novaProducao->Simbolo, simbolo);
            novaProducao->numeroDeProducoes = numeroCorposProducao;
            InserirProducao(gramatica, novaProducao);
        }
    }
}

No* ObterProducoesPorNaoTerminal(Gramatica* gramatica, char* simbolo) {
    No* iterador = gramatica->Primeiro;
    No* producoes = (No*) malloc(1 * sizeof(No));
    while(iterador != NULL) {
        if (strcmp(iterador->Simbolo, simbolo) == 0) {
            return iterador;
        }
        iterador = iterador->Proximo;
    }
    return NULL;
}

void CriarGramatica(FILE* arquivo, Gramatica* gramatica) {
    int ch, i = 0;
    char* simbolo = (char*) malloc(sizeof(char) + 1);
    char** producoes = (char**) malloc(1 * sizeof(char*));
    producoes[i] = (char*) malloc(sizeof(char) + 1);
    while ((ch = fgetc(arquivo)) != EOF) {
        if ((char)ch != '-' && strlen(simbolo) == 0) {
            simbolo = Concatenar(simbolo, (char)ch);
            continue;
        }
        if ((char)ch == ' ' || (char)ch  =='\t' || (char)ch == '-') {
            continue;
        } else if ((char)ch == '\n') {
            InserirOuAtualizarProducao(gramatica, simbolo, producoes, (i + 1));
            InserirNaoTerminal(simbolo);
            i = 0;
            producoes = (char**) malloc(1 * sizeof(char*));
            producoes[i] = (char*) malloc(sizeof(char) + 1);
            simbolo = (char*) malloc(sizeof(char) + 1);
        } else if ((char)ch != '|') {
            producoes[i] = Concatenar(producoes[i], (char)ch);
        } else if ((char)ch == '|') {
            i++;
            producoes[i] = (char*) malloc(sizeof(char) + 1);
        }
    }
}

void SubstituirTodasAsProducoesDeAjEmAi(Gramatica* gramatica, char* aj, char* ai) {
    char** producoes = (char**) malloc(1 * sizeof(char*));
    No* producoesAi = ObterProducoesPorNaoTerminal(gramatica, ai);
    No* producoesAj = ObterProducoesPorNaoTerminal(gramatica, aj);
    int i, j, k = 0;
    for (i = 0; i < producoesAi->numeroDeProducoes; i++) {
        if (strncmp(producoesAi->Producoes[i], aj, strlen(aj)) == 0) {
            for (j = 0; j < producoesAj->numeroDeProducoes; j++) {
                int tAjPj = strlen(producoesAj->Producoes[j]);
                int tAiPi = strlen(producoesAi->Producoes[i]);
                char* resto = (char*) malloc(tAiPi);
                if (tAiPi > 1) {
                    ///Pega o valor da posição 1 ate o fim da producao[i]
                    memcpy(resto, &producoesAi->Producoes[i][1], tAiPi);
                }
                producoes[k] = (char*) malloc(tAjPj + strlen(resto) + 1);
                strcpy(producoes[k], producoesAj->Producoes[j]);
                strcat(producoes[k], resto);
                k++;
            }
        } else {
            producoes[k] = (char*) malloc(strlen(producoesAi->Producoes[i]) + 1);
            producoes[k] = producoesAi->Producoes[i];
            k++;
        }
    }
    producoesAi->numeroDeProducoes = k;
    producoesAi->Producoes = (char**) malloc(1 * sizeof(producoes));
    producoesAi->Producoes = producoes;
}

AlphaBeta ObterCorposProducaoAlphasEBetasDeAi(Gramatica* gramatica, char* ai) {
    int i, a = 0, b = 0, tAi = strlen(ai);
    AlphaBeta alphaBeta;
    No* producoesAi = ObterProducoesPorNaoTerminal(gramatica, ai);
    char** alpha = (char**) malloc(sizeof(char*) * producoesAi->numeroDeProducoes);
    char** beta = (char**) malloc(sizeof(char*) * producoesAi->numeroDeProducoes);
    for (i = 0; i < producoesAi->numeroDeProducoes; i++) {
        int tPi = strlen(producoesAi->Producoes[i]);
        if (strncmp(producoesAi->Producoes[i], ai, tAi) == 0) {
            alpha[a] = (char*) malloc(tPi + 1);
            strcpy(alpha[a], producoesAi->Producoes[i]);
            a++;
        } else {
            beta[b] = (char*) malloc(tPi + 1);
            strcpy(beta[b], producoesAi->Producoes[i]);
            b++;
        }
    }
    alphaBeta.TamanhoDeAlpha = a;
    alphaBeta.Alpha = alpha;
    alphaBeta.TamanhoDeBeta = b;
    alphaBeta.Beta = beta;
    return alphaBeta;
}

void RemoverRecursaoDiretaAi(Gramatica* gramatica, char* ai) {
    AlphaBeta alphaBeta = ObterCorposProducaoAlphasEBetasDeAi(gramatica, ai);
    char** producoes;
    /// se não existir corpo de produção alpha,
    /// quer dizer que não tem recursão a esquerda
    if (alphaBeta.TamanhoDeAlpha) {
        char* aiLinha = (char*) malloc(strlen(ai) + 2);
        strcpy(aiLinha, ai);
        strcat(aiLinha, "'");
        int i, tAiLinha = strlen(aiLinha);
        producoes = (char**) malloc(sizeof(char*) * alphaBeta.TamanhoDeAlpha);
        ///Aplicando alpha
        for (i = 0; i < alphaBeta.TamanhoDeAlpha; i++) {
            int tAlphaI = strlen(alphaBeta.Alpha[i]);
            char* resto = (char*) malloc(tAlphaI + 1);
            if (tAlphaI > 1) {
                ///Pega o valor da posição 1 ate o fim da producao[i]
                memcpy(resto, &alphaBeta.Alpha[i][1], tAlphaI);
            }
            producoes[i] = (char*) malloc(tAiLinha + strlen(resto) + 1);
            strcpy(producoes[i], resto);
            strcat(producoes[i], aiLinha);
        }
        producoes[i] = (char*) malloc(strlen(simboloVazio) + 1);
        strcpy(producoes[i], simboloVazio);
        InserirOuAtualizarProducao(gramatica, aiLinha, producoes, (i + 1));

        ///Aplicando beta
        producoes = (char**) malloc(sizeof(char*) * alphaBeta.TamanhoDeBeta);
        for (i = 0; i < alphaBeta.TamanhoDeBeta; i++) {
            int tBetaI = strlen(alphaBeta.Beta[i]);
            if (strcmp(simboloVazio, alphaBeta.Beta[i]) == 0) {
                producoes[i] = (char*) malloc(tAiLinha + 1);
                strcpy(producoes[i], aiLinha);
            } else {
                producoes[i] = (char*) malloc(tAiLinha + strlen(alphaBeta.Beta[i]) + 1);
                strcpy(producoes[i], alphaBeta.Beta[i]);
                strcat(producoes[i], aiLinha);
            }
        }
        No* producoesAi = ObterProducoesPorNaoTerminal(gramatica, ai);
        AtualizarCorposProducao(producoesAi, producoes, i);
    }
}

int main(int argc, char **argv) {
    int opcao;
    int index;
    FILE* arquivo;
    extern char *optarg;
    opterr = 0;
    naoTerminais.Tamanho = 0;
    naoTerminais.Vetor = NULL;
    Gramatica* gramatica = Inicializar();
    while((opcao = getopt(argc, argv, "hv:e:s:")) -1) {
        switch(opcao) {
            case 'h':
                printf("\n");
                printf("\tAlgoritmo para remover recursão direta e indireta a esquerda\n");
                printf("\n");
                printf("Utilizaçao: ./RemoveRecursaoEsquerda -v epsilon -e caminho do arquivo de entrada.txt -s caminho do arquivo de saida.txt\n");
                printf("Opções:\n");
                printf(" -Opção -h: ajuda\n");
                printf(" -Opção -v: simbolo vazio Ex: ./RemoveRecursaoEsquerda -v epsilon\n");
                printf(" -Opção -e: arquivo de entrada Ex: ./RemoveRecursaoEsquerda -e entrada.txt\n");
                printf(" -Opção -s: arquivo de saida Ex: ./RemoveRecursaoEsquerda -s saida.txt\n");
                printf("\n");
                break;
            case 'v':
                simboloVazio = (char*) malloc(strlen(optarg) + 1);
                strcpy(simboloVazio, optarg);
                break;
            case 'e':
                printf("Lendo arquivo de entrada\n");
                arquivo = fopen(optarg, "r");
                if(arquivo == NULL) {
                    printf("Nao foi possivel abrir o arquivo.\n");
                } else {
                    CriarGramatica(arquivo, gramatica);
                    int i, j;
                    for (i = 0; i < naoTerminais.Tamanho; i++) {
                        for (j = 0; j < i; j++) {
                            SubstituirTodasAsProducoesDeAjEmAi(gramatica, naoTerminais.Vetor[j], naoTerminais.Vetor[i]);
                        }
                        RemoverRecursaoDiretaAi(gramatica, naoTerminais.Vetor[i]);
                    }
                    fclose(arquivo);
                }
                printf("Remoção realizado com sucesso\n");
                break;
            case 's':
                printf("Gravando no arquivo de saida\n");
                arquivo = fopen(optarg, "w");
                if(arquivo == NULL) {
                    printf("Nao foi possivel abrir o arquivo.\n");
                } else {
                    No* iterador = gramatica->Primeiro;
                    while(iterador != NULL) {
                        int i, j;
                        for (i = 0; i < strlen(iterador->Simbolo); i++) {
                            fputc(iterador->Simbolo[i], arquivo);
                        }
                        fputc('-', arquivo);
                        for (i = 0; i < iterador->numeroDeProducoes; i++) {
                            int tPi = strlen(iterador->Producoes[i]);
                            for (j = 0; j < tPi; j++) {
                                fputc(iterador->Producoes[i][j], arquivo);
                            }
                            if (i < (tPi - 1)) {
                                fputc('|', arquivo);
                            }
                        }
                        fputc('\n', arquivo);
                        iterador = iterador->Proximo;
                    }
                    fclose(arquivo);
                }
                printf("Gravação realizado com sucesso\n");
                break;
            case '?':
                if (optopt == 'i') {
                    fprintf (stderr, "Opção -%c requer um argumento.\n", optopt);
                } else if (isprint (optopt)) {
                    fprintf (stderr, "Opção desconhecida '-%c'.\n", optopt);
                } else {
                    fprintf(stderr, "Caractere de opção desconhecido '\\x%x'.\n", optopt);
                }
                return 1;
            default:
                abort();
                break;
        }
    }
    return 0;
}
