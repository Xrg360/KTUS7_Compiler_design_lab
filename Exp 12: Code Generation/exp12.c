#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE 100
#define MAX_CODE 1000
#define MAX_TOKEN 20

// Structure to hold three address code components
typedef struct {
    char result[MAX_TOKEN];
    char op;
    char arg1[MAX_TOKEN];
    char arg2[MAX_TOKEN];
} ThreeAddressCode;

// Function declarations
void trim(char *str);
void parse_line(char *line, ThreeAddressCode *code);
void generate_assembly(ThreeAddressCode *code, FILE *output);
char get_operator(char *expr);
void get_operands(char *expr, char op, char *arg1, char *arg2);

// Trim whitespace from a string
void trim(char *str) {
    char *start = str;
    char *end = str + strlen(str) - 1;
    
    while(isspace(*start)) start++;
    while(end > start && isspace(*end)) end--;
    
    *(end + 1) = '\0';
    memmove(str, start, end - start + 2);
}

// Get the operator from expression
char get_operator(char *expr) {
    if (strchr(expr, '+')) return '+';
    if (strchr(expr, '-')) return '-';
    if (strchr(expr, '*')) return '*';
    if (strchr(expr, '/')) return '/';
    return '=';
}

// Extract operands from expression
void get_operands(char *expr, char op, char *arg1, char *arg2) {
    char *pos = strchr(expr, op);
    if (pos == NULL) {
        strcpy(arg1, expr);
        arg2[0] = '\0';
        return;
    }
    
    int len = pos - expr;
    strncpy(arg1, expr, len);
    arg1[len] = '\0';
    strcpy(arg2, pos + 1);
    
    trim(arg1);
    trim(arg2);
}

// Parse a line of three address code
void parse_line(char *line, ThreeAddressCode *code) {
    char *equals = strchr(line, '=');
    if (equals == NULL) return;
    
    // Get result (left of =)
    int result_len = equals - line;
    strncpy(code->result, line, result_len);
    code->result[result_len] = '\0';
    trim(code->result);
    
    // Get expression (right of =)
    char expr[MAX_LINE];
    strcpy(expr, equals + 1);
    trim(expr);
    
    // Get operator and operands
    code->op = get_operator(expr);
    get_operands(expr, code->op, code->arg1, code->arg2);
}

// Generate assembly code for the three address code
void generate_assembly(ThreeAddressCode *code, FILE *output) {
    switch(code->op) {
        case '+':
            fprintf(output, "    MOV AX, %s\n", code->arg1);
            fprintf(output, "    ADD AX, %s\n", code->arg2);
            fprintf(output, "    MOV %s, AX\n", code->result);
            break;
            
        case '-':
            fprintf(output, "    MOV AX, %s\n", code->arg1);
            fprintf(output, "    SUB AX, %s\n", code->arg2);
            fprintf(output, "    MOV %s, AX\n", code->result);
            break;
            
        case '*':
            fprintf(output, "    MOV AX, %s\n", code->arg1);
            fprintf(output, "    MOV BX, %s\n", code->arg2);
            fprintf(output, "    MUL BX\n");
            fprintf(output, "    MOV %s, AX\n", code->result);
            break;
            
        case '/':
            fprintf(output, "    MOV AX, %s\n", code->arg1);
            fprintf(output, "    MOV BX, %s\n", code->arg2);
            fprintf(output, "    XOR DX, DX\n");
            fprintf(output, "    DIV BX\n");
            fprintf(output, "    MOV %s, AX\n", code->result);
            break;
            
        case '=':
            fprintf(output, "    MOV %s, %s\n", code->result, code->arg1);
            break;
    }
    fprintf(output, "\n");
}

int main() {
    char input_line[MAX_LINE];
    ThreeAddressCode code;
    FILE *output;
    
    // Open output file
    output = fopen("output.asm", "w");
    if (output == NULL) {
        printf("Error opening output file!\n");
        return 1;
    }
    
    // Write assembly header
    fprintf(output, "section .data\n\n");
    fprintf(output, "section .text\n");
    fprintf(output, "global _start\n\n");
    fprintf(output, "Start:\n");
    
    printf("Enter three address code (empty line to end):\n");
    
    // Read and process input lines
    while (1) {
        if (fgets(input_line, MAX_LINE, stdin) == NULL || input_line[0] == '\n') {
            break;
        }
        
        // Remove newline
        input_line[strcspn(input_line, "\n")] = 0;
        
        // Parse and generate code
        parse_line(input_line, &code);
        generate_assembly(&code, output);
    }
    
    // Write assembly footer
    fprintf(output, "Exit program;\n");
    fprintf(output, "    MOV AX, 4C00h\n");
    fprintf(output, "    INT 21h\n");
    
    fclose(output);
    printf("\nAssembly code has been written to output.asm\n");
    
    return 0;
}
