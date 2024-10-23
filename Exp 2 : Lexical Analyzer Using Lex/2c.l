%{
int vowel_count = 0;
int consonant_count = 0;
%}

vowel [aeiouAEIOU]
consonant [bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ]

%%

{vowel}        { vowel_count++; }    // Increment vowel count for each vowel
{consonant}    { consonant_count++; } // Increment consonant count for each consonant

[ \t\n]+       { /* Ignore whitespace and newlines */ }
.              { /* Ignore other characters */ }

%%

int main(int argc, char **argv) {
    FILE *file = fopen("input.txt", "r");  // Open text file
    if (!file) {
        printf("Could not open the file\n");
        return 1;
    }
    yyin = file;
    yylex();  // Start lexical analysis
    fclose(file);

    // Print the final counts
    printf("Number of vowels: %d\n", vowel_count);
    printf("Number of consonants: %d\n", consonant_count);

    return 0;
}

int yywrap() {
    return 1;
}
