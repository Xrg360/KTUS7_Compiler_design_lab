%{
int line_count = 0;
int word_count = 0;
%}

identifier [a-zA-Z][a-zA-Z0-9]*

%%
\n              { line_count++; }   

{identifier}    { word_count++; }   
[0-9]+          { word_count++; }   

[ \t]+          { /* Ignore whitespace */ }
.               { /* Ignore everything else */ }

%%

int main(int argc, char **argv) {
    FILE *file = fopen("input.txt", "r");  // Open text file
    if (!file) {
        printf("Could not open the file\n");
        return 1;
    }
    yyin = file;
    yylex();  
    fclose(file);

    // Print the final counts
    printf("Number of lines: %d\n", line_count);
    printf("Number of words: %d\n", word_count);

    return 0;
}

int yywrap() {
    return 1;
}
