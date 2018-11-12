#include <stdio.h>
#include <cs50.h>
#define MAX_HEIGHT 23
/* A program to display a symmetric pyramids which look like
   Nintendos classic mario game.
*/
int main(void)
{
    int height=-1;
    printf("Enter the height of the pyramid: ");
    // validate the height
    while(true)
    {
        height = get_int();
        if(height > MAX_HEIGHT || height < 0)
        {
            printf("Invalid input");
        }
        else
        {
            break;
        }
    }

    int space = height - 1;
    int hash = 1;
    // An outer loop to manage the every row of the pyramid
    for(int i = 0;i < height;i++,space--,hash++)
    {
        // display spaces on the left side
        for(int j = 0;j < space;j++)
        {
            printf(" ");
        }
        // display hashes on the left side
        for(int k = 0;k < hash;k++)
        {
            printf("#");
        }
        // create gap
        printf("  ");
        // display hashes on the right side
        for(int k = 0;k < hash;k++)
        {
            printf("#");
        }
        printf("\n");
   }
}   