#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // for each pixel, calculate the average
            int average = (int)round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // set RGB to the average
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        //  create an axis, so  only iterate until half of the width
        for (int j = 0; j < width / 2; j++)
        {
            // swap the pixels
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copying the image pixel by pixel
    // We need to do this so we
    // OK
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // bluring the image
    // OK
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // first, set up temporary variables
            int tempBlue = 0, tempGreen = 0, tempRed = 0;
            float counter = 0.0;

            // for each pixel, check if adjacant pixels exist
            // if the pixel exists, let's add the RGB values and increase the counter

            // XOO
            // OOO
            // OOO
            if (i - 1 >= 0 && j - 1 >= 0)
            {
                tempBlue += copy[i - 1][j - 1].rgbtBlue;
                tempGreen += copy[i - 1][j - 1].rgbtGreen;
                tempRed += copy[i - 1][j - 1].rgbtRed;
                counter += 1.0;
            }

            // OXO
            // OOO
            // OOO
            if (i - 1 >= 0)
            {
                tempBlue += copy[i - 1][j].rgbtBlue;
                tempGreen += copy[i - 1][j].rgbtGreen;
                tempRed += copy[i - 1][j].rgbtRed;
                counter += 1.0;
            }

            // OOX
            // OOO
            // OOO
            if (i - 1 >= 0 && j + 1 < width)
            {
                tempBlue += copy[i - 1][j + 1].rgbtBlue;
                tempGreen += copy[i - 1][j + 1].rgbtGreen;
                tempRed += copy[i - 1][j + 1].rgbtRed;
                counter += 1.0;
            }

            // OOO
            // XOO
            // OOO
            if (j - 1 >= 0)
            {
                tempBlue += copy[i][j - 1].rgbtBlue;
                tempGreen += copy[i][j - 1].rgbtGreen;
                tempRed += copy[i][j - 1].rgbtRed;
                counter += 1.0;
            }

            // OOO
            // OXO
            // OOO
            // default case is always added
            tempBlue += copy[i][j].rgbtBlue;
            tempGreen += copy[i][j].rgbtGreen;
            tempRed += copy[i][j].rgbtRed;
            counter += 1.0;

            // OOO
            // OOX
            // OOO
            if (j + 1 < width)
            {
                tempBlue += copy[i][j + 1].rgbtBlue;
                tempGreen += copy[i][j + 1].rgbtGreen;
                tempRed += copy[i][j + 1].rgbtRed;
                counter += 1.0;
            }

            // OOO
            // OOO
            // XOO
            if (i + 1 < height && j - 1 >= 0)
            {
                tempBlue += copy[i + 1][j - 1].rgbtBlue;
                tempGreen += copy[i + 1][j - 1].rgbtGreen;
                tempRed += copy[i + 1][j - 1].rgbtRed;
                counter += 1.0;
            }

            // OOO
            // OOO
            // OXO
            if (i + 1 < height)
            {
                tempBlue += copy[i + 1][j].rgbtBlue;
                tempGreen += copy[i + 1][j].rgbtGreen;
                tempRed += copy[i + 1][j].rgbtRed;
                counter += 1.0;
            }

            // OOO
            // OOO
            // OOX
            if (i + 1 < height && j + 1 < width)
            {
                tempBlue += copy[i + 1][j + 1].rgbtBlue;
                tempGreen += copy[i + 1][j + 1].rgbtGreen;
                tempRed += copy[i + 1][j + 1].rgbtRed;
                counter += 1.0;
            }

            // After we checked for all the adjacant piixels, let's get the average, round it, and convert it to an integer
            tempBlue = (int)round(tempBlue / counter);
            tempGreen = (int)round(tempGreen / counter);
            tempRed = (int)round(tempRed / counter);

            // Apply filter to pixel
            image[i][j].rgbtBlue = tempBlue;
            image[i][j].rgbtGreen = tempGreen;
            image[i][j].rgbtRed = tempRed;
        }
    }
    return;
}

// detect edges

void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Set up matrixes.
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Set up copy of image.
    RGBTRIPLE copy[height][width];

    // Make a copy of the picture so  manipulation doesn't interfere.
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j ++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Loop through every pixel first
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j ++)
        {
            // Initialize  temporary values for RGB
            int GxRed = 0, GxGreen = 0, GxBlue = 0, GyRed = 0, GyGreen = 0, GyBlue = 0;
            // Initialize Gx and Gy
            int GRed = 0, GBlue = 0, GGreen = 0;
            // Initialize

            // Loop through the adjacent files
            for (int k = -1; k < 2; k ++)
            {
                for (int l = -1; l < 2; l ++)
                {
                    // Check if it's out of bounds
                    if (i + k < 0 || i + k >= height || j + l < 0 || j + l >= width)
                    {
                        // If it's out of bounds, add 0
                        // Continue in this case
                        continue;
                    }
                    else
                    {
                        // Add values for Gx
                        GxRed += copy[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                        GxGreen += copy[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];
                        GxBlue += copy[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];

                        // Add values for Gy
                        GyRed += copy[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                        GyGreen += copy[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];
                        GyBlue += copy[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];

                    }
                }
            }
            // calculcate the values of sobel
            GRed = round(sqrt(GxRed * GxRed + GyRed * GyRed));
            GGreen = round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen));
            GBlue = round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue));

            // Check if no value exceeded the limits
            if (GRed > 255)
            {
                GRed = 255;
            }
            if (GGreen > 255)
            {
                GGreen = 255;
            }
            if (GBlue > 255)
            {
                GBlue = 255;
            }

            // modify the pixel
            image[i][j].rgbtRed = GRed;
            image[i][j].rgbtGreen = GGreen;
            image[i][j].rgbtBlue = GBlue;
        }
    }

    return;
}
