#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate average between blue, green and red of each pixel
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            // Assign average value to blue, green and red of each pixel
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate sepia values
            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);

            // Check if the values are greater than 255
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            // Assign sepia values to blue, green and red of each pixel
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++) // Only need to iterate through the first half of each row
        {
            // Calculate last element of row
            int last = width - 1;
            // Switch left elements with right elements
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][last - j];
            image[i][last - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy of image array
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Iterate through pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int counter = 0;
            float sumRed = 0;
            float sumGreen = 0;
            float sumBlue = 0;
            // Coordinates of pixel and neighboring pixels
            int rowCoords[] = {i - 1, i, i + 1};
            int colCoords[] = {j - 1, j, j + 1};

            // Iterate through coordinates
            for (int k = 0; k < 3; k++)
            {
                for (int q = 0; q < 3; q++)
                {
                    // Check if pixel is valid
                    if (rowCoords[k] >= 0 && colCoords[q] >= 0 && rowCoords[k] < width && colCoords[q] < height)
                    {
                        RGBTRIPLE pixel = copy[rowCoords[k]][colCoords[q]];

                        // Add up colours
                        sumRed += pixel.rgbtRed;
                        sumGreen += pixel.rgbtGreen;
                        sumBlue += pixel.rgbtBlue;
                        //Update counter
                        counter++;
                    }
                }
            }

            // Change image value to rounded average of neighboring pixels
            image[i][j].rgbtRed = round(sumRed / counter);
            image[i][j].rgbtGreen = round(sumGreen / counter);
            image[i][j].rgbtBlue = round(sumBlue / counter);
        }
    }
    return;
}
