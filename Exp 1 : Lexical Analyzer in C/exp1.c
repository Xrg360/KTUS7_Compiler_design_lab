#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int is_keyword(const char* buffer);
int is_operator(char ch);

int main() {
    char ch, buffer[15], operators[] = "+-*/%=,;()";
    FILE *fp;
    int i = 0, j = 0;
    
    // Counters
    int keyword_count = 0, identifier_count = 0, operator_count = 0;

    fp = fopen("input.txt", "r");

    if (fp == NULL) {
        printf("Error opening the file\n");
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF) {
        if (isalnum(ch)) {
            // Collect alphanumeric characters into buffer
            buffer[j++] = ch;
        } else {
            // Process the buffer if it's non-empty
            if (j != 0) {
                buffer[j] = '\0';  // Terminate the string
                j = 0;

                if (is_keyword(buffer)) {
                    printf("kwd ");
                    keyword_count++;  // Increment keyword counter
                } else {
                    printf("id ");
                    identifier_count++;  // Increment identifier counter
                }
            }

            // Handle operators
            if (is_operator(ch)) {
                operator_count++;  // Increment operator counter
                switch (ch) {
                    case '+': printf("op-plus "); break;
                    case '-': printf("op-sub "); break;
                    case '*': printf("op-mul "); break;
                    case '/': printf("op-div "); break;
                    case '%': printf("op-mod "); break;
                    case '=': printf("op-equ "); break;
                    case ';': printf("; "); break;
                    case ',': printf(", "); break;
                    case '(': printf("( "); break;
                    case ')': printf(") "); break;
                }
            }
        }
    }

    fclose(fp);

    // Output the counts
    printf("\nCounts:\n");
    printf("Keywords: %d\n", keyword_count);
    printf("Identifiers: %d\n", identifier_count);
    printf("Operators: %d\n", operator_count);

    return 0;
}

int is_keyword(const char* buffer) {
    const char *keywords[] = {"auto", "break", "case", "char", "const", "continue", "default",
                              "do", "double", "else", "enum", "extern", "float", "for", "goto",
                              "if", "int", "long", "register", "return", "short", "signed",
                              "sizeof", "static", "struct", "switch", "typedef", "union",
                              "unsigned", "void", "volatile", "while"};
    
    for (int i = 0; i < 32; ++i) {
        if (strcmp(keywords[i], buffer) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_operator(char ch) {
    return strchr("+-*/%=,;()", ch) != NULL;
}
