#include <stdio.h>
#include <cs50.h>
#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <crypt.h>

int main(int argc, string argv[])
{
    // Program will exit if given anything other than one command line argument
    if (argc != 2)
    {
        printf("Enter one command line argument.\n");
        return 1;
    }
    //creates array to reference in for loops
    char letters[52] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
                       };
    // Gets salt from hashed password
    char salt[3];
    strncpy(salt, argv[1], 2);
    // Initialize array to hold iterations of password
    char key[6];
    // Initialize boolean that shows whether or not key has been found
    bool found = false;
    string hash;
    int counter = 0;
    // Generate plaintext to be hashed
    // If a loop completes, the counter is increased, and the key will now be generated with an extra letter
    // If the hashcode is equal to argv[1], the given hashcode, the loops will break and it will print the key.
    for (int i = 0; i < 52; i++)
    {
        key[4] = (counter >= 4) ? letters[i] : '\0';
        for (int j = 0; j < 52; j++)
        {
            key[3] = (counter >= 3) ? letters[j] : '\0';
            for (int k = 0; k < 52; k++)
            {
                key[2] = (counter >= 2) ? letters[k] : '\0';
                for (int l = 0; l < 52; l++)
                {
                    key[1] = (counter >= 1) ? letters[l] : '\0';
                    for (int m = 0; m < 52; m++)
                    {
                        key[0] = letters[m];
                        hash = crypt(key, salt);
                        if (strcmp(hash, argv[1]) == 0)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found)
                    {
                        break;
                    }
                    counter++;
                }
                if (found)
                {
                    break;
                }
                counter++;
            }
            if (found)
            {
                break;
            }
            counter++;
        }
        if (found)
        {
            break;
        }
        counter++;
    }
    printf("%s\n", key);
    return 0;
}
