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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
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
            // Colours sums for each grid
            float sumRedGx = 0;
            float sumGreenGx = 0;
            float sumBlueGx = 0;
            float sumRedGy = 0;
            float sumGreenGy = 0;
            float sumBlueGy = 0;
            // Coordinates of pixel and neighboring pixels
            int rowCoords[] = {i - 1, i, i + 1};
            int colCoords[] = {j - 1, j, j + 1};
            // Gx grid and Gy grid
            int gx[3][3] =
            {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}
            };
            int gy[3][3] =
            {
                {-1, -2, -1},
                {0, 0, 0},
                {1, 2, 1}
            };

            // Iterate through coordinates and Gx and Gy grids
            for (int k = 0; k < 3; k++)
            {
                for (int q = 0; q < 3; q++)
                {
                    // Check if pixel is inside image
                    if (rowCoords[k] >= 0 && colCoords[q] >= 0 && rowCoords[k] < width && colCoords[q] < height)
                    {
                        RGBTRIPLE pixel = copy[rowCoords[k]][colCoords[q]];

                        // Compute gx and gy for each colour channel
                        sumRedGx += gx[k][q] * pixel.rgbtRed;
                        sumGreenGx += gx[k][q] * pixel.rgbtGreen;
                        sumBlueGx += gx[k][q] * pixel.rgbtBlue;

                        sumRedGy += gy[k][q] * pixel.rgbtRed;
                        sumGreenGy += gy[k][q] * pixel.rgbtGreen;
                        sumBlueGy += gy[k][q] * pixel.rgbtBlue;
                    }
                }
            }

            // Compute each new colour channel final value
            int red = round(sqrt(pow(sumRedGx, 2) + pow(sumRedGy, 2)));
            int green = round(sqrt(pow(sumGreenGx, 2) + pow(sumGreenGy, 2)));
            int blue = round(sqrt(pow(sumBlueGx, 2) + pow(sumBlueGy, 2)));

            // Check if the values are greater than 255, if so set value to 255
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }

            // Change image value to final value
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}
