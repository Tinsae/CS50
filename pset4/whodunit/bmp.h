/**
 * BMP-related data types based on Microsoft's own.
 */

#include <stdint.h>

/**
 * Common Data Types 
 *
 * The data types in this section are essentially aliases for C/C++ 
 * primitive data types.
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/dd183374(v=vs.85).aspx.
 */
 
// 3 * 2 bytes + 2 * 4 bytes = 14 bytes
typedef struct 
{ 
    WORD bfType; 
    DWORD bfSize; 
    WORD bfReserved1; 
    WORD bfReserved2; 
    DWORD bfOffBits; 
} __attribute__((__packed__)) 
BITMAPFILEHEADER; 

/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the 
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx.
 */
 
// The struct below takes 40 bytes
// 9 * 4 bytes + 2 * 2 bytes = 40
typedef struct
{
    DWORD biSize;     
    LONG biWidth; 
    LONG biHeight; 
    WORD biPlanes; 
    WORD biBitCount; 
    DWORD biCompression; 
    DWORD biSizeImage; 
    LONG biXPelsPerMeter; 
    LONG biYPelsPerMeter; 
    DWORD biClrUsed; 
    DWORD biClrImportant; 
} __attribute__((__packed__))
BITMAPINFOHEADER; 

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from https://msdn.microsoft.com/en-us/library/dd162939(v=vs.85).aspx.
 */
 
 // 1 + 1 + 1 = 3 bytes = 24 bits
typedef struct
{
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;
