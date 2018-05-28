#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

bool allalpha(string k);

int main(int argc, string argv[])
{
    // If the program doesnt recieve one command line argument that is alphabetical, it will exit
    if (argc != 2 || !allalpha(argv[1]))
    {
        printf("Please enter an alphabetical keyword to encode with.\n");
        return 1;
    }
    // Gets key to encode with from argument
    string k = argv[1];
    // Creates alphabet
    char a[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
               };
    // Prompts user for plaintext to encode
    string p = get_string("plaintext: ");
    // Encodes plaintext by iterating over each character with key
    for (int i = 0, n = strlen(p), c = 0, j = 0; i < n; i++)
    {
        // Using modulo so that j, the key for the plaintext character will be between 0-25
        j = (k[c] < 97) ? k[c] % 65 : k[c] % 97;
        if (!isalpha(p[i]))
        {
            continue;
        }
        else if (isupper(p[i]))
        {
            p[i] = toupper(a[((p[i] + j) % 65) % 26]);
            c++;
        }
        else
        {
            p[i] = a[((p[i] + j) % 97) % 26];
            c++;
        }
        c = (c == strlen(k)) ? 0 : c;
    }
    // Outputs ciphertext
    printf("ciphertext: %s\n", p);
    return 0;
}

// Checks to see if all characters in k are alphabetical
bool allalpha(string k)
{
    bool a = true;
    for (int i = 0, n = strlen(k); i < n; i++)
    {
        if (!isalpha(k[i]))
        {
            a = false;
            break;
        }
    }
    return a;
}
