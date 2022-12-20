// From a credit card number find if the credit card is valid and if it's American Express, Visa or MasterCard
// Problem link: https://cs50.harvard.edu/x/2022/psets/1/credit/

#include <cs50.h>
#include <stdio.h>

int calculateChecksum(long number);
int calculateCardLength(long number);
int findStartingDigits(long number);

int main(void)
{
    // Propmt user for credit card number
    long cardNumber = get_long("Number: ");

    // Calculate checksum (Luhn’s Algorithm)
    int checksum = calculateChecksum(cardNumber);

    // Check Luhn's Algorithm
    if (checksum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Calculate card number length
    int length = calculateCardLength(cardNumber);

    // Find starting digits of card number
    int startingDigits = findStartingDigits(cardNumber);

    // Print type of card
    if (length == 15 && (startingDigits == 34 || startingDigits == 37))
    {
        printf("AMEX\n");
    }
    else if ((length == 13 || length == 16) && startingDigits / 10 == 4)
    {
        printf("VISA\n");
    }
    else if (length == 16 && startingDigits > 50 && startingDigits < 56)
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

int calculateChecksum(long number)
{
    long cardNumberCopy1 = number;
    long cardNumberCopy2 = number;
    int sum1 = 0; // sum of every other digit starting with the number’s second-to-last digit multiplied by 2
    int sum2 = 0; // sum of the digits that weren’t multiplied by 2 (the rest of the digits)

    cardNumberCopy1 /= 10; // remove last digit from card number
    sum2 = cardNumberCopy2 % 10; // add last digit to sum 2

    while (cardNumberCopy1 > 0)
    {
        int digit = (cardNumberCopy1 % 10) * 2; // get every other digit and multiply by 2
        // Add the products’ digits (not the products themselves) together
        if (digit < 10)
        {
            sum1 += digit;
        }
        else
        {
            int sumSingleDigits = (digit % 10) + (digit / 10); // get single digits and add them up
            sum1 += sumSingleDigits;
        }
        cardNumberCopy1 /= 100; // remove last two digits from card number

        cardNumberCopy2 /= 100; // remove last two digits from card number
        sum2 += cardNumberCopy2 % 10; // calculate sum of the digits that weren’t multiplied by 2
    }

    return sum1 + sum2;
}

int calculateCardLength(long number)
{
    int length = 0;
    while (number > 0)
    {
        number /= 10;
        length++;
    }
    return length;
}

int findStartingDigits(long number)
{
    while (number > 100)
    {
        number /= 10;
    }

    return number;
}