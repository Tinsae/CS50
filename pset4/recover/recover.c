

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cs50.h>
int main(void)
{
    typedef uint8_t  BYTE;
    FILE* inptr = fopen("card.raw", "r");
    FILE* outptr = NULL;
    BYTE buffer[512];
    BYTE initials[4];
    BYTE sign[4] = {0xff, 0xd8, 0xff, 0xe0};
    int number = 0;
    char filename[8];

    if (inptr == NULL)
    {
        fprintf(stderr,"Could not open %s.\n", "card.raw");
        return 1;
    }
    // read a buffer from card.raw until EOF , on EOF fread returns 0
    // we read 512 blocks; i expect to find 50 blocks
    // equivalent to 50 images
    // The fourth byte’s first four bits are 1110.
    // modify the fourth byte accordingly
    // filter out the four starting bits of the fourth byte
    while (fread(&buffer, sizeof(buffer), 1, inptr) > 0)
    {
        int i=0;
        // get the inital bytes for a buffer
        while(i < 4)
        {
            initials[i] = buffer[i];
            i++;
            
        }
        initials[3] = (initials[3] >> 4) << 4;
        // to check whether the image is a jpg; we compare the two
        // if fourth byte is 0xe0 it is a jpg
        bool sign_found=memcmp(initials, sign, sizeof(sign)) == 0;
        // open a new file for a new signature
        if ( sign_found && outptr == NULL)
        {
            sprintf(filename, "%03d.jpg", number);
            outptr = fopen(filename, "a");
            fwrite(&buffer, sizeof(buffer), 1, outptr);
        }
        // close previous file because a new sign is found
        else if(sign_found && outptr != NULL)
        {
            fclose(outptr);
            number++;
            sprintf(filename, "%03d.jpg", number);
            outptr = fopen(filename, "a");
            fwrite(&buffer, sizeof(buffer), 1, outptr);
        }
        // A single jpeg may use one or more blocks
        // write bytes to opened jpg file because 
        // a new signature is not recieved in the buffer
        
        else if (!sign_found && 
        outptr != NULL)
        {
            fwrite(&buffer, sizeof(buffer), 1, outptr);
        }
        
    }
    
    fclose(inptr);
    fclose(outptr);
    return 0;
}
