
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: resize scale infile outfile\n");
        return 1;
    }
    
    // remember scale
    int scale = atoi(argv[1]);
       
    // ensure n is is a positive int less than or equal to 100
    if(scale > 100 || scale < 1)
    {
        fprintf(stderr, "scale range is (0, 100]\n");
        return 1;
    }

    // remember filenames
    char* ifile = argv[2];
    char* ofile = argv[3];

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

    // store the width and height of the first image
    int width1 = bih.biWidth;
    int height1 = bih.biHeight;
   
    // update width and height
    bih.biWidth *= scale;
    bih.biHeight *= scale;
       
    // determine padding for scanlines
    // padding1 will be used to skip scaling on the 0 bytes
    int padding1 =  (4 - (width1 * sizeof(RGBTRIPLE)) % 4) % 4;
    // padding 2 will be used to apply padding 
    // to every scanline of the scaled image
    int padding2 =  (4 - (bih.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // update image size = total number of bytes used by the pixels
    bih.biSizeImage = abs(bih.biHeight) * ((bih.biWidth * sizeof (RGBTRIPLE)) + padding2);
    
    // update file size = total number of bytes including the header information
    bfh.bfSize = bih.biSizeImage + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER); 

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // allocate storage for buffer to hold scanline
    // it needs to be flexible to handle different widths of images
    // that is why we use pointer
    // the buffer will not contain the padding (if any)
    RGBTRIPLE *scanline = malloc(sizeof(RGBTRIPLE) * (bih.biWidth));

    // iterate over infile's scanlines
    for (int i = 0, height = abs(height1); i < height; i++)
    {
        // later, we will add it to the pointer in order to iterate over the pixels
        int iterator = 0;
        // iterator becomes 0 for the next scanline
        for (int j = 0; j < width1; j++)
        {
            // temporary storage
            RGBTRIPLE pixel;
            
            // read RGB triple from infile
            // we read 3 bytes(one pixel) at a time
            fread(&pixel, sizeof(RGBTRIPLE), 1, inptr);
            
            // write pixel to temporary scanline n times where n is the scale
            for(int count = 0; count < scale; count++)
            {
                // 3 bytes ( one pixel) are written at a time
                *(scanline+(iterator)) = pixel;
                iterator++;
            }
        }
            
        // skip over padding, if any
        // because we don't want to scale the padding
        // padding2(for the new image) is calculated by using formula
        fseek(inptr, padding1, SEEK_CUR);

           // put scanline n times according to the scale amount         
           for(int r = 0; r < scale; r++)
           {
                // scanline contains the the the first scanline of 
                // the scaled image 
                fwrite((scanline), sizeof(RGBTRIPLE) * bih.biWidth,1, outptr);
                //fwrite((scanline), sizeof(RGBTRIPLE), bih.biWidth, outptr);
                // write padding to the scaled image
                for (int k = 0; k < padding2; k++)
                    fputc(0, outptr);
           }        
    }
    //Your program, if it uses malloc, must not leak any memory.
    free(scanline);
    fclose(inptr);
    fclose(outptr);

    return 0;
}
