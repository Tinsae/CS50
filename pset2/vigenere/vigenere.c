/* A program that uses a string key to encrypt a plaintext
   It will leave non-alphabetical characters as they are 
*/
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main(int argc, string argv [])
{
    string ikey, plaintext;
   // printf("Welcome to the Vigenere Encryption\n");
    if (argc == 2)
    {
        // step 1: get the key
        ikey = argv[1];
        for (int i = 0;i < strlen(ikey);i++)
        {
            if (!isalpha(ikey[i]))
            {
                printf("%s\n", " illegal characters inside key ");
                return 1;
            }
        }
      
        // step 2: get the plain text
        printf("%s", "plaintext: ");
        plaintext = get_string();
        // step 3: encipher
        int klength = (int) strlen(ikey);
        int plength = (int) strlen(plaintext);
        // the size of cipher text is equal to the plaintext   
        char ciphertext[plength + 1];
        // result will keep each encrypted character
        char result = '\0';
        // plain text counter
        int pcount;
        // key counter
        int kcount;
        for (pcount = 0,kcount = 0;pcount < plength;pcount++)
        {
            char letter = plaintext[pcount];
            // As Zamlya said; the modulo operator will group the 
            // alphabetical characters into available keys
            char key = ikey[(kcount) % klength]; 
            // No need for else block because we ensured that key is
            // alphabetical in Line 13 - Line 20
            if (isupper(key)) 
            {
                // shift the letter by 65
                key -= 65; 
            }
            else if (islower(key))
            {
                // shift the letter by 97
                key -= 97;
            }
            if (isupper(letter))
            {
                // shift the letter by 65
                result = letter - 65;
                // do the ciphering
                result = (result + key ) % 26 + 65;
                kcount++;
            }   
            else if (islower(letter))
            {
                // shift the letters by 97
                result = letter - 97;
                // do the ciphering
                result = (result + key ) % 26 + 97;
                kcount++;
            }
            // we don't go to the next key because the letter is not
            // alphabetical
            // ; And the letter remains as it is
            else
            {
                result = letter;
            }
            ciphertext[pcount] = result;
           // printf("%c", result);
        }
        // make sure the string is terminated
        ciphertext[pcount] = '\0';
        //"ciphertext:%s\n"
        printf("ciphertext:%s\n",ciphertext);
    }
    else
    {
        printf("%s \n", "insufficient arguments: ");
        return 1;
    }
    
}