%{
int COMMENT = 0;
%}

identifier [a-zA-Z][a-zA-Z0-9]*

%%
#.*               { printf("\n%s is a preprocessor directive", yytext); }
"int" |
"float" |
"char" |
"double" |
"while" |
"for" |
"struct" |
"typedef" |
"do" |
"if" |
"break" |
"continue" |
"void" |
"switch" |
"return" |
"else" |
"goto"         { if (!COMMENT) printf(" kwd"); }

"/*"            { COMMENT = 1; printf(" comment starts"); }
"*/"            { COMMENT = 0; printf(" comment ends"); }

"+"             { if (!COMMENT) printf(" op-plus"); }
"-"             { if (!COMMENT) printf(" op-sub"); }
"*"             { if (!COMMENT) printf(" op-mul"); }
"/"             { if (!COMMENT) printf(" op-div"); }

{identifier}\(  { if (!COMMENT) printf(" fun"); }
"{"             { if (!COMMENT) printf(" block begins"); }
"}"             { if (!COMMENT) printf(" block ends"); }
{identifier}(\[[0-9]*\])? { if (!COMMENT) printf(" id"); }

\".*\"          { if (!COMMENT) printf(" str"); }
[0-9]+          { if (!COMMENT) printf(" num"); }

"="             { if (!COMMENT) printf(" op-equ"); }
"<=" |
">=" |
"<" |
"==" |
">"             { if (!COMMENT) printf(" rel-op"); }

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
    return 0;
}

int yywrap() {
    return 1;
}
