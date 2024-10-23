#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR_LENGTH 100

typedef struct Quadruple {
    char operator[10];
    char arg1[10];
    char arg2[10];
    char result[10];
} Quadruple;

typedef struct QuadrupleTable {
    Quadruple* quads;
    int count;
    int capacity;
    int temp_count;
} QuadrupleTable;

// Initialize quadruple table
QuadrupleTable* initQuadrupleTable() {
    QuadrupleTable* table = (QuadrupleTable*)malloc(sizeof(QuadrupleTable));
    table->capacity = 10;
    table->count = 0;
    table->temp_count = 0;
    table->quads = (Quadruple*)malloc(table->capacity * sizeof(Quadruple));
    return table;
}

// Generate a new temporary variable name
char* generateTemp(QuadrupleTable* table) {
    char* temp = (char*)malloc(10);
    sprintf(temp, "t%d", ++table->temp_count);
    return temp;
}

// Add a new quadruple to the table
void addQuadruple(QuadrupleTable* table, const char* op, const char* arg1, 
                  const char* arg2, const char* result) {
    if (table->count >= table->capacity) {
        table->capacity *= 2;
        table->quads = (Quadruple*)realloc(table->quads, 
                                          table->capacity * sizeof(Quadruple));
    }
    
    Quadruple* quad = &table->quads[table->count++];
    strcpy(quad->operator, op);
    strcpy(quad->arg1, arg1);
    strcpy(quad->arg2, arg2);
    strcpy(quad->result, result);
}

// Print the quadruple table
void printQuadrupleTable(QuadrupleTable* table) {
    printf("\nQuadruple Address Code:\n");
    printf("%-4s %-8s %-8s %-8s %-8s\n", "No.", "Op", "Arg1", "Arg2", "Result");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < table->count; i++) {
        printf("%-4d %-8s %-8s %-8s %-8s\n", 
               i,
               table->quads[i].operator,
               table->quads[i].arg1,
               table->quads[i].arg2,
               table->quads[i].result);
    }
}

// Check if character is an operator
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '^');
}

// Get operator precedence
int getPrecedence(char op) {
    switch (op) {
        case '^': return 3;
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
        default: return 0;
    }
}

// Parse expression and generate quadruples
void parseExpression(QuadrupleTable* table, const char* expr) {
    char operators[MAX_EXPR_LENGTH];
    char* operands[MAX_EXPR_LENGTH];
    int op_top = -1;
    int operand_top = -1;
    
    char token[10] = "";
    int token_index = 0;
    
    // Process each character in the expression
    for (int i = 0; expr[i] != '\0'; i++) {
        if (isspace(expr[i])) continue;
        
        if (isalnum(expr[i])) {
            token[token_index++] = expr[i];
        } else if (isOperator(expr[i])) {
            if (token_index > 0) {
                token[token_index] = '\0';
                operands[++operand_top] = strdup(token);
                token_index = 0;
            }
            
            while (op_top >= 0 && 
                   getPrecedence(operators[op_top]) >= getPrecedence(expr[i])) {
                char* temp = generateTemp(table);
                char* arg2 = operands[operand_top--];
                char* arg1 = operands[operand_top--];
                char op[2] = {operators[op_top--], '\0'};
                
                addQuadruple(table, op, arg1, arg2, temp);
                operands[++operand_top] = temp;
            }
            operators[++op_top] = expr[i];
        }
    }
    
    // Handle the last token
    if (token_index > 0) {
        token[token_index] = '\0';
        operands[++operand_top] = strdup(token);
    }
    
    // Process remaining operators
    while (op_top >= 0) {
        char* temp = generateTemp(table);
        char* arg2 = operands[operand_top--];
        char* arg1 = operands[operand_top--];
        char op[2] = {operators[op_top--], '\0'};
        
        addQuadruple(table, op, arg1, arg2, temp);
        operands[++operand_top] = temp;
    }
}

// Free the quadruple table
void freeQuadrupleTable(QuadrupleTable* table) {
    free(table->quads);
    free(table);
}

int main() {
    QuadrupleTable* table = initQuadrupleTable();
    char expr[MAX_EXPR_LENGTH];
    
    while (1) {
        printf("\nEnter an arithmetic expression (or 'quit' to exit):\n");
        printf("(Supported operators: +, -, *, /, ^)\n> ");
        
        if (fgets(expr, MAX_EXPR_LENGTH, stdin) == NULL) {
            break;
        }
        
        // Remove newline
        expr[strcspn(expr, "\n")] = 0;
        
        // Check for quit command
        if (strcmp(expr, "quit") == 0) {
            break;
        }
        
        // Reset the table for new expression
        table->count = 0;
        table->temp_count = 0;
        
        // Parse and generate quadruples
        parseExpression(table, expr);
        
        // Print results
        printf("\nExpression: %s\n", expr);
        printQuadrupleTable(table);
    }
    
    // Clean up
    freeQuadrupleTable(table);
    
    return 0;
}
