#include <stdio.h>
#include "lexer.h"

char *read_line() {
    char buffer[512];
    char *line = NULL;
    memset(buffer, 0, sizeof(buffer));
    fgets(buffer, 512, stdin);
    buffer[strcspn(buffer, "\r\n")] = '\0';
    line = calloc(strlen(buffer) + 1, sizeof(char));
    strcpy(line, buffer);
    return line;
}

void clean_line(char *line) {
    free(line);
}

int main(int argc, char *argv[]) {
    while(1) {
        printf(">");
        char *line = read_line();
        if (!strcmp(line, "quit")) break;
        Scanner scanner = new_scanner(line);
        Token tkn;
        do {
            tkn = get_token(&scanner);
            printf("[%s, %d] ", tkn.value, tkn.type);
            clean_string(tkn);
        } while (tkn.type != END);
        printf("\n");
        clean_line(line);
    }
    return 0;    
}
