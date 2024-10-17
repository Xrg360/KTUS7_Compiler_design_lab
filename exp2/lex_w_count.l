%{
int COMMENT = 0;
int kwd_count = 0;
int id_count = 0;
int op_plus_count = 0;
int op_sub_count = 0;
int op_mul_count = 0;
int op_div_count = 0;
int block_begins_count = 0;
int block_ends_count = 0;
int str_count = 0;
int num_count = 0;
int rel_op_count = 0;
%}

%%
#.*             { printf("\n%s is a preprocessor directive", yytext); }

"int"|"float"|"char"|"double"|"while"|"for"|"struct"|"typedef"|"do"|"if"|"break"|"continue"|"void"|"switch"|"return"|"else"|"goto"
                { if (!COMMENT) { printf(" kwd"); kwd_count++; } }

"/*"            { COMMENT = 1; printf(" comment starts"); }
"*/"            { COMMENT = 0; printf(" comment ends"); }

"+"             { if (!COMMENT) { printf(" op-plus"); op_plus_count++; } }
"-"             { if (!COMMENT) { printf(" op-sub"); op_sub_count++; } }
"*"             { if (!COMMENT) { printf(" op-mul"); op_mul_count++; } }
"/"             { if (!COMMENT) { printf(" op-div"); op_div_count++; } }

[a-zA-Z][a-zA-Z0-9]*\(  { if (!COMMENT) { printf(" fun"); } }

"{"             { if (!COMMENT) { printf(" block begins"); block_begins_count++; } }
"}"             { if (!COMMENT) { printf(" block ends"); block_ends_count++; } }

[a-zA-Z][a-zA-Z0-9]*(\[[0-9]*\])?  { if (!COMMENT) { printf(" id"); id_count++; } }

\".*\"          { if (!COMMENT) { printf(" str"); str_count++; } }
[0-9]+          { if (!COMMENT) { printf(" num"); num_count++; } }

"="             { if (!COMMENT) { printf(" op-equ"); } }

"<="|">="|"<"|"=="|">"  { if (!COMMENT) { printf(" rel-op"); rel_op_count++; } }

.               { if (!COMMENT) ECHO; }
%%

int main(int argc, char **argv) {
    FILE *file = fopen("input.c", "r");
    if (!file) {
        printf("Could not open the file\n");
        return 1;
    }
    yyin = file;
    yylex();
    fclose(file);

    // Print counts after analysis
    printf("\nSummary:\n");
    printf("Keywords: %d\n", kwd_count);
    printf("Identifiers: %d\n", id_count);
    printf("Plus operators: %d\n", op_plus_count);
    printf("Subtraction operators: %d\n", op_sub_count);
    printf("Multiplication operators: %d\n", op_mul_count);
    printf("Division operators: %d\n", op_div_count);
    printf("Block begins: %d\n", block_begins_count);
    printf("Block ends: %d\n", block_ends_count);
    printf("String literals: %d\n", str_count);
    printf("Numbers: %d\n", num_count);
    printf("Relational operators: %d\n", rel_op_count);
    
    return 0;
}

int yywrap() {
    return 1;
}
