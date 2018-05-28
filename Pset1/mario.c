#include <stdio.h>
#include <cs50.h>

// Prints a pyramid up to 23 hashes large

int getHeight(void);
void printSpaces(int s);
void printHashes(int h);

int main(void)
{
    int h = getHeight();
    int s = h - 1;
    for (int i = 1; i <= h; i++)
    {
        printSpaces(s);
        s--;
        printHashes(i);
        printf("  ");
        printHashes(i);
        printf("\n");
    }

}

// Gets height of pyramid
int getHeight(void)
{
    int h = get_int("Height: ");
    while (h < 0 || h > 23)
    {
        h = get_int("Height: ");
    }
    return h;
}

// Prints spaces on left side of pyramid
void printSpaces(int s)
{
    for (int j = 0; j < s; j++)
    {
        printf(" ");
    }
}

// Prints i amount of hashes
void printHashes(int i)
{
    for (int k = 0; k < i; k++)
    {
        printf("#");
    }
}
