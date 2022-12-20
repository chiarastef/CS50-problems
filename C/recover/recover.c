// Implement a program that recovers JPEGs from a forensic image
// Problem link: https://cs50.harvard.edu/x/2022/psets/4/recover/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Create a new type to store a byte of data
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open file (memory card)
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE buffer[512];
    int img_counter = 0;
    FILE *output;
    char filename[8];
    bool found = false;
    bool first_img = false;

    // Go through memory card and read 512 bytes into a buffer
    while (fread(buffer, sizeof(BYTE), 512, input))
    {
        // Check if it's the start of a new jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Check if it's first jpg
            if (!first_img)
            {
                first_img = true;
            }
            else
            {
                fclose(output); // Close current file
            }
            sprintf(filename, "%03i.jpg", img_counter);
            output = fopen(filename, "w");
            fwrite(buffer, sizeof(BYTE), 512, output);
            found = true;
            img_counter++;
        }
        // If jpg was alredy found, keep writing
        else if (found)
        {
            fwrite(buffer, sizeof(BYTE), 512, output);
        }
    }

    // Close files
    fclose(input);
    fclose(output);
    return 0;
}