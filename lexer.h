#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEYWORDS \
    K(BREAK, "break") \
    K(CONTINUE, "continue") \
    K(ELSE, "else") \
    K(IF, "if") \
    K(FLOAT, "float") \
    K(INT, "int") \
    K(WHILE, "while")
#define K(type, string) type,

#define SPECIAL_CHARACTERS \
    C(END, '\0', "END") \
    C(LROUND, '(', "(") \
    C(RROUND, ')', ")") \
    C(LSQUARE, '[', "[") \
    C(RSQUARE, ']', "]") \
    C(LCURLY, '{', "{") \
    C(RCURLY, '}', "}") \
    C(COMMA, ',', ",") \
    C(SEMI, ';', ";") \
    C(DOT, '.', ".") \
    C(PLUS, '+', "+") \
    C(MINUS, '-', "-") \
    C(STAR, '*', "*") \
    C(SLASH, '/', "/") \
    C(EXCLAMATION, '!', "!") \
    C(EQUAL, '=', "=") \
    C(LANGLE, '<', "<") \
    C(RANGLE, '>', ">")
#define C(type, value, string) type,

typedef enum {
    KEYWORDS
    NUM_KEYWORDS,
    IDENTIFIER,
    NUMBER,
    STRING_LITERAL,
    ERROR,
    SPECIAL_CHARACTERS
} token_type;
#undef K
#undef C
#define K(type, string) string,
#define C(type, value, string) \
    case value: \
        token = new_token(string, type, scanner->line);\
        break;

typedef struct {
    char *value;
    token_type type;
    int line;
} Token;

typedef struct {
    char *src;
    char *current;
    size_t line;
} Scanner;

Scanner new_scanner(char* src);
Token get_token(Scanner *scanner);
void clean_string(Token token);

#endif // LEXER_H
