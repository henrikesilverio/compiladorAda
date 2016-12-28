#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

#ifdef AnaliseLexica

#endif // AnaliseLexica

#define TOKEN_INVALIDO -1
#define PALAVRA_RESERVADA 0
#define SIMBOLO 1
#define CADEIA_CARACTERE 2
#define NUMERO 3
#define IDENTIFICADOR 4
#define SEPARADORES_COMPOSTO "*./:<=>"
#define SEPARADORES_SIMPLES "&'()+,-;|"
#define TAM_BUFFER 10000

typedef struct {
    char* palavra;
    char* simboloTerminal;
    int tipo;
    int linha;
    int coluna;
} Token;

FILE* arquivo;
int linha, coluna;

void CriarExpressaoRegularPalavrasReservadas(char* buffer){
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

void CriarExpressaoRegularSimbolos(char* buffer){
    strcpy(buffer, "^&$|");
    strcat(buffer, "^\'$|");
    strcat(buffer, "^\\*$|");
    strcat(buffer, "^\\)$|");
    strcat(buffer, "^\\($|");
    strcat(buffer, "^\\+$|");
    strcat(buffer, "^,$|");
    strcat(buffer, "^-$|");
    strcat(buffer, "^\\.$|");
    strcat(buffer, "^\\/$|");
    strcat(buffer, "^:$|");
    strcat(buffer, "^;$|");
    strcat(buffer, "^<$|");
    strcat(buffer, "^=$|");
    strcat(buffer, "^>$|");
    strcat(buffer, "^\\|$|");
    strcat(buffer, "^=>$|");
    strcat(buffer, "^\\.\\.$|");
    strcat(buffer, "^\\*\\*$|");
    strcat(buffer, "^:=$|");
    strcat(buffer, "^\\/=$|");
    strcat(buffer, "^>=$|");
    strcat(buffer, "^<=$|");
    strcat(buffer, "^>>$|");
    strcat(buffer, "^<<$|");
    strcat(buffer, "^<>$");
}

void ObterVetorDeExpressoesRegulares(char vetor[5][2256]){
    char expressaoRegularPalavrasReservadas[2256];
    char expressaoRegularSimbolos[512];

    CriarExpressaoRegularPalavrasReservadas(expressaoRegularPalavrasReservadas);
    CriarExpressaoRegularSimbolos(expressaoRegularSimbolos);

    strcpy(vetor[0], expressaoRegularPalavrasReservadas);
    strcpy(vetor[1], expressaoRegularSimbolos);
    strcpy(vetor[2], "^\".*\"$");
    strcpy(vetor[3], "(^[0-9]+$)|([0-9]+[.][0-9]+$)");
    strcpy(vetor[4], "^[a-zA-Z](_?[a-zA-Z0-9])*$");
}

void strupp(char* beg)
{
    while (*beg = toupper(*beg)) {
        beg++;
    }
}

char* ObterSimboloTerminal(Token* token) {
    int resultado;
    regex_t regex;
    if(token->tipo == PALAVRA_RESERVADA) {
        char* upp = malloc(sizeof(token->palavra) + 1);
        strcpy(upp,token->palavra);
        strupp(upp);
        return upp;
    } else if (token->tipo == NUMERO) {
        return "numeric_lit";
    } else if (token->tipo == CADEIA_CARACTERE) {
        resultado = regcomp(®ex, "\'.\'", REG_EXTENDED);
        resultado = regexec(®ex, token->palavra, 0, NULL, 0);
        if(resultado == REG_NOMATCH){
            return "char_lit";
        } else {
            return "char_string";
        }
    } else if (token->tipo == SIMBOLO) {
        if (strstr(token->palavra, "'") != NULL) {
            return "TIC";
        } else if (strstr(token->palavra, "..") != NULL) {
            return "DOT_DOT";
        } else if (strstr(token->palavra, "<<") != NULL) {
            return "LT_LT";
        } else if (strstr(token->palavra, "<>") != NULL) {
            return "BOX";
        } else if (strstr(token->palavra, "<=") != NULL) {
            return "LT_EQ";
        } else if (strstr(token->palavra, "**") != NULL) {
            return "EXPON";
        } else if (strstr(token->palavra, "/=") != NULL) {
            return "NE";
        } else if (strstr(token->palavra, ">>") != NULL) {
            return "GT_GT";
        } else if (strstr(token->palavra, ">=") != NULL) {
            return "GE";
        } else if (strstr(token->palavra, ":=") != NULL) {
            return "IS_ASSIGNED";
        } else if (strstr(token->palavra, "=>") != NULL) {
            return "RIGHT_SHAFT";
        } else {
            return token->palavra;
        }
    } else if (token->tipo == IDENTIFICADOR) {
        return "identifier";
    }
}

void ClassificarToken(Token* token){
    char expressoes[5][2256];
    int tipo;
    regex_t regex;
    int resultado;
    token->tipo = TOKEN_INVALIDO;
    token->simboloTerminal = "$";
    ObterVetorDeExpressoesRegulares(expressoes);
    for(tipo = 0; tipo < 5; tipo++){
        resultado = regcomp(®ex, expressoes[tipo], REG_EXTENDED);
        resultado = regexec(®ex, token->palavra, 0, NULL, 0);
        if(resultado == REG_NOMATCH){
            continue;
        }
        token->tipo = tipo;
        token->simboloTerminal = ObterSimboloTerminal(token);
        break;
    }
}

Token ObterToken(FILE* arquivo, int* linha, int* coluna) {
    Token token;
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

        if(naoEncontrouString && strchr(SEPARADORES_SIMPLES, ch)) {
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
        } else if (naoEncontrouString && strchr(SEPARADORES_COMPOSTO, ch)) {
            if(tamanho_palavra < 2) {
                palavra[tam++] = (char)ch;
                token.coluna = *coluna;
                token.linha = *linha;
                (*coluna)++;
                continue;
            } else {
                ///Volta uma posição do arquivo
                fseek(arquivo, -1, SEEK_CUR);
            }
            break;
        } else if (tamanho_palavra == 1 && strchr(SEPARADORES_COMPOSTO, palavra[0])) {
            ///Volta uma posição do arquivo
            fseek(arquivo, -1, SEEK_CUR);
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
    if (!tam) {
        palavra[0] = '$';
        palavra[1] = '\0';
    } else {
        palavra[tam] = '\0';
    }
    token.palavra = palavra;
    ClassificarToken(&token);
    return token;
}
