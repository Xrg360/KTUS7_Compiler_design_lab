#include<stdio.h>
#include<string.h>
#include<ctype.h>
char input[10];
int i=0,error=0;
void E();
void T();
void Eprime();
void Tprime();
void F();
void main()
{
        printf("Recursive Descend Parser for arithmetic expression containing + and * (eg:a+a*a)\n");
        printf("Enter an arithmetic expression : ");
        scanf("%s",input);
        E();
        if(strlen(input)==i&&error==0)
                printf("\nAccepted..!!!\n");
        else
                printf("\nRejected..!!!\n");
}
void E()
{
        T();
        Eprime();
}
void Eprime()
{
        if(input[i]=='+')
        {
                i++;
                E();
        }
}
void T()
{
        F();
        Tprime();
}
void Tprime()
{
        if(input[i]=='*')
        {
                i++;
                T();
        }
}
void F()
{
        if(isalnum(input[i]))       i++;
        else if(input[i]=='(')
        {
                i++;
                E();
                if(input[i]==')')
                        i++;
                else
                        error=1;
        }
        else
                error=1;
}
