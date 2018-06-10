#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t  BYTE;

#define BUFFER 512

char *makeImageName(int counter);

int main(int argc, char *argv[])
{
    // Ensure 2 arguments
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: ./recover filetorecover");
        return 1;
    }
    FILE *inptr = fopen(argv[1], "r");
    // Ensure the file can be opened
    if (inptr == NULL)
    {
        fprintf(stderr, "File cannot be opened for reading");
        return 2;
    }
    BYTE *buffer = malloc(BUFFER);
    FILE *outptr = NULL;
    int jpegFound = 0; int counter = 0;
    char *outfile = NULL;
    // Go through file until end is reached
    while (fread(buffer, BUFFER, 1, inptr) == 1)
    {
        // If start of a new JPEG
        if (buffer[0] == 0xff
        && buffer[1] == 0xd8
        && buffer[2] == 0xff
        && ((buffer[3] & 224) == 224))
        {
            // If a file is open, close it
            if (jpegFound > 0)
            {
                counter++;
                free(outfile);
                fclose(outptr);
            }
            // Open new file
             outfile = makeImageName(counter);
             outptr = fopen(outfile, "w");
             jpegFound++;
        }
        // If a jpeg has been found yet, write to the open one
        if (jpegFound > 0)
        {
            fwrite(buffer, BUFFER, 1, outptr);
        }

    }
    // Free memory and close files
    free(buffer);
    fclose(inptr);
    fclose(outptr);
    return 0;
}
// Make image name for outfile
char *makeImageName(int counter)
{
    char *name = malloc(7);
    if (counter < 10)
    {
        sprintf(name, "00%i.jpg", counter);
    }
    else if (counter < 100)
    {
        sprintf(name, "0%i.jpg", counter);
    }
    else
    {
        sprintf(name, "%i.jpg", counter);
    }
    return name;
}
