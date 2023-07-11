#include "lexer.h"

Scanner new_scanner(char *src) {
    Scanner scanner;
    scanner.src = src;
    scanner.current = src;
    scanner.line = 1;
    return scanner;
}

Token new_token(char *value, token_type type, int line) {
    Token token;
    token.value = value;
    token.type = type;
    token.line = line;
    return token;
}

void clean_string(Token token) {
    if(token.type == IDENTIFIER ||
        token.type == NUMBER ||
        token.type == STRING_LITERAL)
        free(token.value);
}

char *extract_str(const char *start, size_t size) {
    char *literal = calloc(size, sizeof(char));
    memcpy(literal, start, size - 1);
    return literal;
}

char peek(const char *current, int offset) {
    return *(current + offset);
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

int is_string(char c) {
    return c == '"';
}

int is_alphanumeric(char c) {
    return (is_digit(c) || is_letter(c));
}

/*
 * Ignore whitespaces and comment lines
 */
void ignore(Scanner *scanner) {
    while ((*scanner->current < '!' && *scanner->current > '\0') || *scanner->current == '#') {
        if (*scanner->current == '#') {
            while(*scanner->current++ != '\n');
        }
        if (*scanner->current == '\n') {
            scanner->line++;
        }
        scanner->current++;
    }
}

/*
 * Word. Reserved words or identifiers.
 */
Token word(Scanner *scanner) {
    const char* keyword[] = {KEYWORDS};
    const char *start = scanner->current;
    size_t size = 1;
    while (is_alphanumeric(peek(start, size++)));
    scanner->current += size - 1;
    char *value = extract_str(start, size);
    int kw = 0;
    while(kw < NUM_KEYWORDS && strcmp(value, keyword[kw])) kw++;
    return new_token(value, kw, scanner->line);
}

/*
 * Number.
 */
Token number(Scanner *scanner) {
    const char *start = scanner->current;
    int size = 1;
    while (is_digit(peek(start, size++)));
    scanner->current += size - 1;
    char *value = extract_str(start, size);
    return new_token(value, NUMBER, scanner->line);
}

/*
 * String literal.
 */
Token string(Scanner *scanner) {
    const char* start = scanner->current + 1;
    int size = 1;
    char peeked = 0;
    do {
        peeked = peek(start, size);
        if (peeked == '\n') {
            scanner->line++;
        }
        if (peeked == '\0') {
            scanner->current += size + 1;
            printf("%s\n", scanner->current);
            return new_token("No end quote found", ERROR, scanner->line);
        }
        size++;
    } while (peeked != '"');
    scanner->current += size + 1;
    char *value = extract_str(start, size);
    return new_token(value, STRING_LITERAL, scanner->line);
}

/*
 * One character token
 */
Token character(Scanner *scanner) {
    Token token;
    switch (*scanner->current++) {
    SPECIAL_CHARACTERS
    default:
        token = new_token("Illegal character", ERROR, scanner->line);
    }
    return token;
}

Token get_token(Scanner *scanner) {
    Token (*fn[4]) (Scanner *scanner);
    fn[0] = character;
    fn[1] = word;
    fn[2] = number;
    fn[3] = string;
    ignore(scanner);
    int state = is_letter(*scanner->current) + 
                2 * is_digit(*scanner->current) +
                3 * is_string(*scanner->current);
    return fn[state] (scanner);
}

