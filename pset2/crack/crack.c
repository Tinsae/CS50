/* A program that cracks password by brute force apporach
   Given input is hashed password(encrypted), 
   The output is the plaintext
*/
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <crypt.h>

int main(int argc, string argv [])
{
    // an array to put all letters and the null character
    char letters[52];
    bool found=false;
    int counter=0;
    long long big_counter=0;
    FILE *permutations;
    permutations = fopen("/home/ubuntu/workspace/pset2/crack/test.txt", "w");
    // fill the array with all possible letters
    for(char pass = 65; (pass <= 122) ;pass++)
    {
        // skip the non-alphabetic characters
        if (!(pass > 90 && pass < 97))
        {
            letters[counter] = pass;
            counter++;
            
        }
    }
    // make the last character a null

    // we need exactly two arguments: the program name and the hashed password
    if (argc == 2)
    {
        // extract the salt from the 1st argument
        char salt[3] = {argv[1][0], argv[1][1], '\0'};
        // the maximum crackable password is four
        char password[5] = {'\0'};
        // length 1 password
        for (int icount = 0; icount < 52 && found == false; icount++)
        {
            big_counter++;
            password[0] = letters[icount];
            password[1] = '\0';
            fputs(password,permutations);
            fputs("\n",permutations);
            if (strcmp(crypt(password, salt), argv[1]) == 0)
            {
                found = 1;
            }
            // length 2
            for (int jcount = 0; jcount < 52 && found == false; jcount++)
            {
                big_counter++;
                password[1] = letters[jcount];
                password[2] = '\0';

                fputs(password,permutations);
                fputs("\n",permutations);
                if (strcmp(crypt(password, salt), argv[1]) == 0)
                {
                    found = 1;
                }
        
                fputs(password,permutations);
                fputs("\n",permutations);
                // length 3
                for (int kcount = 0; kcount < 52 && found == false; kcount++)
                {
                    big_counter++;
                    password[2] = letters[kcount];
                    password[3] = '\0';
                    password[4] = '\0';
                    fputs(password,permutations);
                    fputs("\n",permutations);
        
                    if (strcmp(crypt(password, salt), argv[1]) == 0)
                    {
                        found = 1;
                    }
                    // length 4;
                    for (int mcount = 0; 
                    mcount < 52 && found == false ; mcount++)
                    {
                        password[3] = letters[mcount];
                        password[4] = '\0';
                        fputs(password,permutations);
                        fputs("\n",permutations);
                        if (strcmp(crypt(password, salt), argv[1]) == 0)
                        {
                            found = 1;
                        }
                        big_counter++;
                    }         
                }
            }
        }
    
        
        if (found == 1)
        {
            // printf("Password: %s\n", password);
        
            // in clear form
            printf("%s\n",password); 
        } 
        else
        {
            printf("Not found\n");
        }
        // No of comparisions
        // Worst Case= 52*52*52*52 + 52*52*52 + 52*52 + 52 =7454980
        // printf("No of Comparisons: %lld\n", big_counter);
    }

    else
    {
        return 1;
    }
    
}