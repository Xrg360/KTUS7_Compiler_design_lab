#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_PROD 100
#define MAX_LEN 50

// Structure to store variable information
struct Variable {
    char name;
    int value;
    int is_constant;
};

// Function to check if a character is an operator
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Function to evaluate expression with constants
int evaluateExpression(char* expr, struct Variable* vars, int var_count) {
    char op1 = expr[0];
    char operator = expr[1];
    char op2 = expr[2];
    int val1, val2;
    
    // Get value of first operand
    if (isdigit(op1)) {
        val1 = op1 - '0';
    } else {
        for (int i = 0; i < var_count; i++) {
            if (vars[i].name == op1 && vars[i].is_constant) {
                val1 = vars[i].value;
                break;
            }
        }
    }
    
    // Get value of second operand
    if (isdigit(op2)) {
        val2 = op2 - '0';
    } else {
        for (int i = 0; i < var_count; i++) {
            if (vars[i].name == op2 && vars[i].is_constant) {
                val2 = vars[i].value;
                break;
            }
        }
    }
    
    // Perform operation
    switch(operator) {
        case '+': return val1 + val2;
        case '-': return val1 - val2;
        case '*': return val1 * val2;
        case '/': return val1 / val2;
        default: return 0;
    }
}

int main() {
    int n;
    char productions[MAX_PROD][MAX_LEN];
    struct Variable vars[MAX_PROD];
    int var_count = 0;
    
    printf("Enter the number of productions: ");
    scanf("%d", &n);
    getchar(); // Consume newline
    
    printf("Enter %d productions:\n", n);
    for (int i = 0; i < n; i++) {
        fgets(productions[i], MAX_LEN, stdin);
        productions[i][strcspn(productions[i], "\n")] = 0; // Remove newline
    }
    
    printf("\nOptimized code:\n");
    
    // Process each production
    for (int i = 0; i < n; i++) {
        char lhs = productions[i][0];
        char rhs[MAX_LEN];
        strcpy(rhs, productions[i] + 2); // Skip '=' character
        
        // Case 1: Direct assignment of constant
        if (strlen(rhs) == 1 && isdigit(rhs[0])) {
            vars[var_count].name = lhs;
            vars[var_count].value = rhs[0] - '0';
            vars[var_count].is_constant = 1;
            var_count++;
            continue;
        }
        
        // Case 2: Assignment of another variable
        if (strlen(rhs) == 1 && isalpha(rhs[0])) {
            for (int j = 0; j < var_count; j++) {
                if (vars[j].name == rhs[0] && vars[j].is_constant) {
                    vars[var_count].name = lhs;
                    vars[var_count].value = vars[j].value;
                    vars[var_count].is_constant = 1;
                    var_count++;
                    // Note: We don't print this assignment as it's optimized out
                    goto next_production;
                }
            }
            // If we get here, the RHS variable is not a constant
            printf("%s\n", productions[i]);
        }
        
        // Case 3: Expression
        if (strlen(rhs) == 3 && isOperator(rhs[1])) {
            char op1 = rhs[0];
            char op2 = rhs[2];
            int can_propagate = 1;
            int val1 = 0, val2 = 0;
            
            // Check if operands are constants or known constant variables
            if (isdigit(op1)) {
                val1 = op1 - '0';
            } else {
                int found = 0;
                for (int j = 0; j < var_count; j++) {
                    if (vars[j].name == op1 && vars[j].is_constant) {
                        val1 = vars[j].value;
                        found = 1;
                        break;
                    }
                }
                if (!found) can_propagate = 0;
            }
            
            if (isdigit(op2)) {
                val2 = op2 - '0';
            } else {
                int found = 0;
                for (int j = 0; j < var_count; j++) {
                    if (vars[j].name == op2 && vars[j].is_constant) {
                        val2 = vars[j].value;
                        found = 1;
                        break;
                    }
                }
                if (!found) can_propagate = 0;
            }
            
            if (can_propagate) {
                int result;
                switch(rhs[1]) {
                    case '+': result = val1 + val2; break;
                    case '-': result = val1 - val2; break;
                    case '*': result = val1 * val2; break;
                    case '/': result = val1 / val2; break;
                    default: result = 0;
                }
                printf("%c=%d\n", lhs, result);
                vars[var_count].name = lhs;
                vars[var_count].value = result;
                vars[var_count].is_constant = 1;
                var_count++;
                continue;
            }
        }
        
        // If no optimization possible, print original production
        printf("%s\n", productions[i]);
        
        next_production:
            continue;
    }
    
    return 0;
}