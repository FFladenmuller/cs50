#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

int main (int argc, char* argv[])
{
    // Ensure 3 arguments
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize infile outfile\n");
        return 1;
    }
    // Remember f, filenames
    float f = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];
    // Open files
    FILE *inptr = fopen(infile, "r");
    FILE *outptr = fopen(outfile, "w");
    // Ensure f is (0.0, 100], files are proper files
    if ((f <= 0 || f > 100) || inptr == NULL || outptr == NULL)
    {
        fprintf(stderr, "Usage: resize infile outfile\n");
        fclose(inptr);
        fclose(outptr);
        return 1;
    }
    // Get old/new BITMAPFILEHEADER/BITMAPINFOHEADER
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;BITMAPINFOHEADER oldbi;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    fseek(inptr, sizeof(BITMAPFILEHEADER), SEEK_SET);
    fread(&oldbi, sizeof(BITMAPINFOHEADER), 1, inptr);
    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 1;
    }
    // Get old padding
    int oldpadding = (4 - (oldbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // Update old BITMAPFILEHEADER/BITMAPINFOHEADER
    bi.biHeight = f * bi.biHeight;
    bi.biWidth = f * bi.biWidth;
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + 54;
    // Write updated BITMAPFILEHEADER/BITMAPINFOHEADER to new file
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    // If upscaling image, repeat pixels. Only works for whole number input
    if (f > 1)
    {
        // Iterate over y
        for (int i = 0; i < abs(oldbi.biHeight); i++)
        {
             // line will hold copy of multiplied x pixel line to multiply each line f times
             int linesize = (oldbi.biWidth * sizeof(RGBTRIPLE) * f) + (padding * sizeof(char));
             BYTE *line = malloc(linesize);
             BYTE *lineptr = malloc(sizeof(BYTE*));
             lineptr = line;
            // Iterate over x
            for (int j = 0; j < oldbi.biWidth; j++)
            {
                // Temporary storage
                RGBTRIPLE triple;
                // Read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                for (int k = 0; k < f; k++)
                {
                    // Write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    // Add each pixel to line
                    *line = triple.rgbtBlue;
                    line += sizeof(BYTE);
                    *line = triple.rgbtGreen;
                    line += sizeof(BYTE);
                    *line = triple.rgbtRed;
                    line += sizeof(BYTE);
                }
            }
            // Skip over padding, if any
            fseek(inptr, oldpadding, SEEK_CUR);
            // If padding is needed, add it
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
                *line = 0x00;
                line += sizeof(BYTE);
            }
            // Write scanned line f - 1 times, since it was already written once
            for (int l = 0; l < f - 1; l++)
            {
                fwrite(lineptr, linesize, 1, outptr);
            }
            free(line);
            free(lineptr);
        }
    }
    // For inputs 1 or lower, use nearest neighbor interpolation
    else if (f <= 1)
    {
        RGBTRIPLE pixels[oldbi.biWidth][abs(oldbi.biHeight)];
        // Populate 2d RGBtriple array with old pixels
        for (int y = 0; y < abs(oldbi.biHeight); y++)
        {
            for (int x = 0; x < oldbi.biWidth; x++)
            {
                // Temporary storage
                RGBTRIPLE triple;
                // Read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                pixels[x][y] = triple;
            }
            fseek(inptr, oldpadding, SEEK_CUR);
        }
        // Write new picture using nearest neighbor interpolation
        // In essence, you imagine the width and height on x and y axis,
        //with each pixel having its own percentage on the line. For the
        //new image that is say, 10% down both the X and Y axis, such as
        // 1,1 in a 10 x 10 picture, you would pick the pixel that is closest
        //to 10% down both its X and Y axis in the old picture to be resized.
        //srcX and srcY calculate which pixel number you need.
        // Iterate over new pictures height
        int srcX; int srcY;
        for (int i = 0; i < abs(bi.biHeight); i++)
        {
            // Iterate over new pictures width
            for (int j = 0; j < bi.biWidth; j++)
            {
                // Temporary storage
                RGBTRIPLE triple;
                // Find old pixel XY coordinates
                srcX = round ( ((double)j / (double)bi.biWidth) * (double)oldbi.biWidth);
                srcY = round ( ((double)i / (double)abs(bi.biHeight)) * (double)abs(oldbi.biHeight));
                srcX = (srcX >= oldbi.biWidth) ? oldbi.biWidth - 1 : srcX;
                srcY = (srcY >= abs(oldbi.biHeight)) ? abs(oldbi.biHeight) - 1 : srcY;
                triple.rgbtBlue = pixels[srcX][srcY].rgbtBlue;
                triple.rgbtRed = pixels[srcX][srcY].rgbtRed;
                triple.rgbtGreen = pixels[srcX][srcY].rgbtGreen;
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);\
                printf("srcX: %i  srcY: %i\n", srcX, srcY);
            }
            // If padding is needed, add it
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }
     fclose(inptr);
     fclose(outptr);
     return 0;
}
