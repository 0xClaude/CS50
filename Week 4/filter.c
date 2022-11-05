#include "helpers.h"
#include <math.h>

// Convert image to grayscale
// OK
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

// Convert image to sepia
// OK
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // first, backup the color codes to avoid overwriting
            int originalBlue = image[i][j].rgbtBlue;
            int originalGreen = image[i][j].rgbtGreen;
            int originalRed = image[i][j].rgbtRed;

            // transform color codes according to algorithm
            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            // check if no value exceeds 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            // apply filter to the pixel

            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
// OK
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
