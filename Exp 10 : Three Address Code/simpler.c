#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR 100
#define MAX_STACK 100

// Structure to store operands or temporary results
struct Operand {
    int value;
    char name[10];  // To store either number or temporary variable name
    int isTemp;     // Flag to indicate if it's a temporary variable
};

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int getPrecedence(char op) {
    switch(op) {
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

void generateThreeAddressCode(char *expr) {
    char operators[MAX_STACK];
    struct Operand operands[MAX_STACK];
    int top_op = -1;
    int top_val = -1;
    int temp_count = 1;
    
    printf("\nGenerated Three Address Code:\n");
    printf("----------------------------\n");
    
    for(int i = 0; expr[i] != '\0'; i++) {
        if(isdigit(expr[i])) {
            struct Operand op;
            op.value = expr[i] - '0';
            sprintf(op.name, "%d", op.value);
            op.isTemp = 0;
            operands[++top_val] = op;
        }
        else if(isOperator(expr[i])) {
            while(top_op >= 0 && getPrecedence(operators[top_op]) >= getPrecedence(expr[i])) {
                struct Operand op2 = operands[top_val--];
                struct Operand op1 = operands[top_val--];
                struct Operand result;
                
                result.isTemp = 1;
                sprintf(result.name, "t%d", temp_count++);
                
                printf("%s := %s %c %s\n", 
                       result.name, 
                       op1.name, 
                       operators[top_op], 
                       op2.name);
                
                operands[++top_val] = result;
                top_op--;
            }
            operators[++top_op] = expr[i];
        }
    }
    
    while(top_op >= 0) {
        struct Operand op2 = operands[top_val--];
        struct Operand op1 = operands[top_val--];
        struct Operand result;
        
        result.isTemp = 1;
        sprintf(result.name, "t%d", temp_count++);
        
        printf("%s := %s %c %s\n", 
               result.name, 
               op1.name, 
               operators[top_op], 
               op2.name);
        
        operands[++top_val] = result;
        top_op--;
    }
}

int main() {
    char expr[MAX_EXPR];
    
    printf("Enter an arithmetic expression: ");
    scanf("%s", expr);
    
    generateThreeAddressCode(expr);
    
    return 0;
}