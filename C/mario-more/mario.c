// Make pyramids using hashes
// The user decides how tall the pyramids should be (between 1 and 8 inclusive)
// Problem link: https://cs50.harvard.edu/x/2022/psets/1/mario/more/

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for height
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8); //Check if input is between 1 and 8 inclusive

    // Print pyramid
    for (int i = 0; i < height; i++) // rows
    {
        // Space before pyramid
        for (int k = height - 1; k > i; k--)
        {
            printf(" ");
        }

        // First pyramid
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // Space between pyramids
        printf("  ");

        // Second pyramid
        for (int l = 0; l <= i; l++)
        {
            printf("#");
        }

        printf("\n");
    }
}