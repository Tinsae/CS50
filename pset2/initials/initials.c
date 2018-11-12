#include <cs50.h>
#include <stdio.h>
#include <string.h>
/*
The program prints a person’s initials given a person’s name
*/
int main(void)
{
    printf("Enter your name ? ");
    string name=get_string();
    
    int i = 0;
    while(i < strlen(name))
    {
        if (name[i - 1] == ' ' || i == 0 )
        {
            printf("%c",name[i]);
        }
        i++;
    }
    printf("\n");
}
    