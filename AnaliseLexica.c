#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

#define TOKEN_INVALIDO -1
#define PALAVRA_RESERVADA 0
#define SIMBOLO 1
#define CADEIA_CARACTERE 2
#define NUMERO 3
#define IDENTIFICADOR 4
#define SEPARADORES "&'()*+,-./:;<=>|"
#define TAM_BUFFER 10000

struct Token {
    char* palavra;
    int tipo;
    int linha;
    int coluna;
} token;

void criarExpressaoRegularPalavrasReservadas(char* buffer){
    strcpy(buffer, "(");
    strcat(buffer, "^abort$|");
    strcat(buffer, "^abs$|");
    strcat(buffer, "^abstract$|");
    strcat(buffer, "^accept$|");
    strcat(buffer, "^access$|");
    strcat(buffer, "^aliased$|");
    strcat(buffer, "^all$|");
    strcat(buffer, "^and$|");
    strcat(buffer, "^array$|");
    strcat(buffer, "^at$|");
    strcat(buffer, "^begin$|");
    strcat(buffer, "^body$|");
    strcat(buffer, "^case$|");
    strcat(buffer, "^constant$|");
    strcat(buffer, "^declare$|");
    strcat(buffer, "^delay$|");
    strcat(buffer, "^delta$|");
    strcat(buffer, "^digits$|");
    strcat(buffer, "^do$|");
    strcat(buffer, "^else$|");
    strcat(buffer, "^elsif$|");
    strcat(buffer, "^end$|");
    strcat(buffer, "^entry$|");
    strcat(buffer, "^exception$|");
    strcat(buffer, "^exit$|");
    strcat(buffer, "^for$|");
    strcat(buffer, "^function$|");
    strcat(buffer, "^generic$|");
    strcat(buffer, "^goto$|");
    strcat(buffer, "^if$|");
    strcat(buffer, "^in$|");
    strcat(buffer, "^interface$|");
    strcat(buffer, "^is$|");
    strcat(buffer, "^limited$|");
    strcat(buffer, "^loop$|");
    strcat(buffer, "^mod$|");
    strcat(buffer, "^new$|");
    strcat(buffer, "^not$|");
    strcat(buffer, "^null$|");
    strcat(buffer, "^of$|");
    strcat(buffer, "^or$|");
    strcat(buffer, "^others$|");
    strcat(buffer, "^out$|");
    strcat(buffer, "^overrinding$|");
    strcat(buffer, "^package$|");
    strcat(buffer, "^pragma$|");
    strcat(buffer, "^private$|");
    strcat(buffer, "^procedure$|");
    strcat(buffer, "^protected$|");
    strcat(buffer, "^raise$|");
    strcat(buffer, "^range$|");
    strcat(buffer, "^record$|");
    strcat(buffer, "^rem$|");
    strcat(buffer, "^renames$|");
    strcat(buffer, "^requeue$|");
    strcat(buffer, "^return$|");
    strcat(buffer, "^reverse$|");
    strcat(buffer, "^select$|");
    strcat(buffer, "^separate$|");
    strcat(buffer, "^some$|");
    strcat(buffer, "^subtype$|");
    strcat(buffer, "^synchronized$|");
    strcat(buffer, "^tagged$|");
    strcat(buffer, "^task$|");
    strcat(buffer, "^terminate$|");
    strcat(buffer, "^then$|");
    strcat(buffer, "^type$|");
    strcat(buffer, "^until$|");
    strcat(buffer, "^use$|");
    strcat(buffer, "^when$|");
    strcat(buffer, "^while$|");
    strcat(buffer, "^with$|");
    strcat(buffer, "^xor$");
    strcat(buffer, ")");
}

void criarExpressaoRegularSimbolos(char* buffer){
    strcpy(buffer, "^&$|");
    strcat(buffer, "^\'$|");
    strcat(buffer, "^\\*$|");
    strcat(buffer, "^\\)$|");
    strcat(buffer, "^\\($|");
    strcat(buffer, "^\\*$|");
    strcat(buffer, "^\\+$|");
    strcat(buffer, "^\\,$|");
    strcat(buffer, "^\\-$|");
    strcat(buffer, "^\\.$|");
    strcat(buffer, "^\\/$|");
    strcat(buffer, "^\\:$|");
    strcat(buffer, "^\\;$|");
    strcat(buffer, "^\\;$|");
    strcat(buffer, "^\\<$|");
    strcat(buffer, "^\\=$|");
    strcat(buffer, "^\\>$|");
    strcat(buffer, "^\\=\\>$|");
    strcat(buffer, "^\\.\\.$|");
    strcat(buffer, "^\\*\\*$|");
    strcat(buffer, "^\\:\\=$|");
    strcat(buffer, "^\\/\\=$|");
    strcat(buffer, "^\\>\\=$|");
    strcat(buffer, "^\\<\\=$|");
    strcat(buffer, "^\\>\\>$|");
    strcat(buffer, "^\\<\\<$|");
    strcat(buffer, "^\\<\\>$");
}

void obterVetorDeExpressoesRegulares(char vetor[5][2256]){
    char expressaoRegularPalavrasReservadas[2256];
    char expressaoRegularSimbolos[512];

    criarExpressaoRegularPalavrasReservadas(expressaoRegularPalavrasReservadas);
    criarExpressaoRegularSimbolos(expressaoRegularSimbolos);

    strcpy(vetor[0], expressaoRegularPalavrasReservadas);
    strcpy(vetor[1], expressaoRegularSimbolos);
    strcpy(vetor[2], "^\".*\"$");
    strcpy(vetor[3], "(^[0-9]+$)|([0-9]+[.][0-9]+$)");
    strcpy(vetor[4], "^[a-zA-Z](_?[a-zA-Z0-9])*$");
}

int classificarToken(char* palavra){
    char expressoes[5][2256];
    int i;
    regex_t regex;
    int resultado;
    obterVetorDeExpressoesRegulares(expressoes);
    token.palavra = palavra;
    for(i = 0; i < 5; i++){
        resultado = regcomp(®ex, expressoes[i], REG_EXTENDED);
        resultado = regexec(®ex, palavra, 0, NULL, 0);
        if(resultado == REG_NOMATCH){
            continue;
        }
        return i;
    }
    return TOKEN_INVALIDO;
}

struct Token getToken(FILE* arquivo, int* linha, int* coluna) {
    struct Token token;
    int ch;
    int tam = 0;
    int naoEncontrouString = 1;
    int resultado;
    char palavra[TAM_BUFFER] = "";
    char index;
    while ((ch = fgetc(arquivo)) != EOF) {
        int tamanho_palavra = strlen(palavra);
        if (naoEncontrouString && (char)ch == ' ' || (char)ch  =='\t') {
            if(tamanho_palavra == 0) {
                (*coluna)++;
                continue;
            }
            (*coluna)++;
            break;
        }

        if (naoEncontrouString && (char)ch == '\n') {
            if(tamanho_palavra == 0) {
                (*coluna)++;
                continue;
            }
            (*linha)++;
            break;
        }

        if ((char)ch == '"') {
            naoEncontrouString = 0;
        }

        if (naoEncontrouString && strchr(SEPARADORES, ch)) {
            if(tamanho_palavra == 0) {
                palavra[tam++] = (char)ch;
                token.coluna = *coluna;
                token.linha = *linha;
                (*coluna)++;
            } else {
                ///Volta uma posição do arquivo
                fseek(arquivo, -1, SEEK_CUR);
            }
            break;
        }

        palavra[tam++] = (char)ch;
        if (tamanho_palavra == 0) {
            token.coluna = *coluna;
            token.linha = *linha;
        } else if((char)ch == '"'){
            naoEncontrouString = 1;
        }
        (*coluna)++;
    }
    palavra[tam] = '\0';
    token.palavra = palavra;
    return token;
}

int main(){
    int i;
    int linha = 1, coluna = 1;
    FILE *arquivo = fopen("helloworld.adb", "r");
	if(arquivo == NULL) {
        printf("Nao foi possivel abrir o arquivo.\n");
	} else {
        for(i = 0; i < 29; i++){
            struct Token token;
            token = getToken(arquivo, &linha, &coluna);
            printf("Palavra %s\n", token.palavra);
            token.tipo = classificarToken(token.palavra);
            if (token.tipo == TOKEN_INVALIDO){
                printf("Token %s invalido linha: %d coluna: %d\n", token.palavra, token.linha, token.coluna);
            }
        }
	}

    fclose(arquivo);
	return 0;
}
