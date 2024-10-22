%{
    #include<stdio.h>
    int valid=1;
%}
%token digit letter
%%
    start: letter s
    s: letter s
    | digit s
    |
    ;
%%
int yyerror()
{
    valid=0;
    printf("Invalid variable!\n");
    return 0;
}
int main()
{
    printf("Enter the variable:\n");
    yyparse();
    if(valid)
    {
        printf("Valid variable!\n");
    }
}