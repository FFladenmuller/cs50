#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Prints error message and returns 1 if the program is not started with 1 argument
    if (argc != 2)
    {
        printf("The program needs 2 arguments,the program name, and an integer for K.\n");
        return 1;
    }
    // Gets number of places for plaintext to be shifted
    int k = atoi(argv[1]);
    // Gets plaintext to encrypt
    string p = get_string("Plaintext: ");
    // Creates alphabet
    char a[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
               };
    // Now convert plaintext to ciphertext
    for (int i = 0, n = strlen(p); i < n; i++)
    {
        if (!isalpha(p[i]))
        {
            continue;
        }
        else if (isupper(p[i]))
        {
            p[i] = toupper(a[((p[i] + k) % 65) % 26]);
        }
        else
        {
            p[i] = a[((p[i] + k) % 97) % 26];
        }
    }
    // Print ciphertext
    printf("ciphertext: %s\n", p);
    return 0;
}

