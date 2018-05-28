#include <stdio.h>
#include <cs50.h>

int getCardDigits(long long card);
int getCheckSum(long long card, int digits);
bool validCheckSum(int sum);
void printCardType(long long card, int digits);

int main(void)
{
    long long card = get_long_long("Card Number: \n");
    int digits = getCardDigits(card);
    int sum = getCheckSum(card, digits);
    if (validCheckSum(sum))
    {
        printCardType(card, digits);
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}

// Multiplies every other digit by 2, starting with the number’s second-to-last digit, and then adds those products' digits together.
int getCheckSum(long long card, int digits)
{
    int i = 0;
    int A[digits];
    int B[digits];
    // Populates array A with digits from card number.
    while (card != 0)
    {
        A[i] = card % 10;
        card /= 10;
        i++;
    }
    // Populates array B with digits from array A in the correct order
    i = 0;
    for (int l = digits - 1; l >= 0; l--)
    {
        B[i++] = A[l];
    }
    int sum = 0;
    // Multiplies and adds every other digit from the card, starting from second to last digit.
    for (int j = digits - 2; j >= 0; j -= 2)
    {
        B[j] *= 2;
        if (B[j] >= 10)
        {
            while (B[j] != 0)
            {
                sum += B[j] % 10;
                B[j] /= 10;
            }
        }
        else
        {
            sum +=B[j];
        }
    }
    // Adds digits to sum that were not multiplied
    for (int k = digits - 1; k >= 0; k -= 2)
    {
        sum += B[k];
    }

    return sum;
}

//Gets the number of digits in card number
int getCardDigits(long long card)
{
    int digits = 0;
    while (card != 0)
    {
        card /= 10;
        digits++;
    }
    return digits;
}

//Checks that the sum is divisible by 10 evenly
bool validCheckSum(int sum)
{
    bool valid = false;
    if (sum % 10 == 0)
    {
        valid = true;
    }
    return valid;
}

//Prints the card type
void printCardType(long long card, int digits)
{
    int i = 0;
    int A[digits];
    // Populates array A with digits from card number.
    while (card != 0)
    {
        A[i] = card % 10;
        card /= 10;
        i++;
    }
    if ((A[digits - 1] == 4 && digits == 13) || (A[digits - 1] == 4 && digits == 16))
    {
        printf("VISA\n");
    }
    else if ((A[digits - 1] == 3 && A[digits - 2] == 4 && digits == 15) ||
             (A[digits - 1] == 3 && A[digits - 2] == 7 && digits == 15))
    {
        printf("AMEX\n");
    }
    else if ((A[digits - 1] == 5 && A[digits - 2] == 1 && digits == 16) ||
             (A[digits - 1] == 5 && A[digits - 2] == 2 && digits == 16) ||
             (A[digits - 1] == 5 && A[digits - 2] == 3 && digits == 16) ||
             (A[digits - 1] == 5 && A[digits - 2] == 4 && digits == 16) ||
             (A[digits - 1] == 5 && A[digits - 2] == 5 && digits == 16))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
