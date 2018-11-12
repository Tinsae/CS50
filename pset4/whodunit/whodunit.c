/**
 * whodunit.c
 *
 * It copies a bitmap image:the only 
 * evidence I have is a 24-bit BMP file called clue.bmp used in 
 * this program. While copying the file pixel by pixel,
 * I changed the pure red pixels to pure white so that the message can be seen
 * in the new image 
 * any other pixels which are not pure white are converted to yellow
 * 
 */
       
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
    // two files need to be supplied
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
    }

    // char* are like strings
    char* ifile = argv[1];
    char* ofile = argv[2];

    // open input file for reading
    FILE* inptr = fopen(ifile, "r");
    // open output file for writing
    FILE* outptr = fopen(ofile, "w");
    if (inptr == NULL || outptr == NULL)
    {
        fprintf(stderr,"file operation error");
        fclose(inptr);
        fclose(outptr);
        return 1;
    }
    
    // read input file's BITMAPFILEHEADER
    // this header is found in the first 14 bytes
    BITMAPFILEHEADER bfh;
    fread(&bfh, sizeof(BITMAPFILEHEADER), 1, inptr);
    // printf("%lu", sizeof(BITMAPFILEHEADER));
    
    // read input file's BITMAPINFOHEADER
    // this header is in the next 40 bytes
    BITMAPINFOHEADER bih;
    fread(&bih, sizeof(BITMAPINFOHEADER), 1, inptr);

    // The image needs to be 24-bit uncompressed BMP
    if (bfh.bfType != 0x4d42 || bfh.bfOffBits != 54 || bih.biSize != 40 || 
        bih.biBitCount != 24 || bih.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Invalid file format.\n");
        return 1;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    // for example for 3 x 3 image  and sizeof(RGBTRIPLE)=3
    // The amount of padding 
    // on each scanline 3 pixels * 3 bytes/pixel=9 bytes
    // 9 bytes is not divisble by four;
    // in order to make it divisible we add 3 bytes
    // so we add 3 bytes on every row
    

    int padding =  (4 - (bih.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // the width of club.bmp is 640 pixels which is divisble by 4
    // so padding is 0
    //printf("%i", padding);

    // iterate over infile's scanlines
    for (int i = 0, height = abs(bih.biHeight); i < height; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bih.biWidth; j++)
        {
            RGBTRIPLE pixel;

            // read RGB triple from infile
            // it reads 3 bytes on every iteration of the loop
            fread(&pixel, sizeof(RGBTRIPLE), 1, inptr);
            
            // change all pure red pixels to white pixels
            if ((pixel.rgbtBlue == 0 && 
            pixel.rgbtGreen == 0 && 
            pixel.rgbtRed == 255)) 
            {
                pixel.rgbtBlue = 255;
                pixel.rgbtGreen = 255;
                pixel.rgbtRed = 255;
            }
            // convert pixels which are not pure whites to yellow
            else if(pixel.rgbtBlue != 255 || 
            pixel.rgbtGreen != 255 || 
            pixel.rgbtRed != 255)
            {
                pixel.rgbtBlue = 0;
                pixel.rgbtGreen = 255;
                pixel.rgbtRed = 255;
            }
        
            // write RGB triple to the new file
            fwrite(&pixel, sizeof(RGBTRIPLE), 1, outptr);
        }

        // before going to the next scanline skip over padding 
        // padding variable contains the number of bytes to skip
        // for example for a 3 pix x 3 pix image; we need to add 3 bytes to
        // each scanline
        fseek(inptr, padding, SEEK_CUR);

        // add same padding to the new file
        for (int k = 0; k < padding; k++)
        {
            //writes one byte
            fputc(0, outptr);
        }
    }
    fclose(inptr);
    fclose(outptr);
    return 0;
}
