#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg;
    float temp_avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // get the average
            temp_avg = ((float) image[i][j].rgbtBlue + (float) image[i][j].rgbtGreen + (float) image[i][j].rgbtRed) / 3;
            avg = round(temp_avg);

            // validate that the average doesn't is greater than 255
            if (avg > 255)
            {
                avg = 255;
            }

            // assign the new gray scale to the pixel
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed  = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sepiaRed, sepiaGreen, sepiaBlue;
    float floatRed, floatGreen, floatBlue;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // get the sepia colors
            floatRed =
                (.393 * (float) image[i][j].rgbtRed) +
                (.769 * (float) image[i][j].rgbtGreen) +
                (.189 * (float) image[i][j].rgbtBlue);
            sepiaRed = round(floatRed);

            floatGreen =
                (.349 * (float) image[i][j].rgbtRed) +
                (.686 * (float) image[i][j].rgbtGreen) +
                (.168 * (float) image[i][j].rgbtBlue);
            sepiaGreen = round(floatGreen);

            floatBlue =
                (.272 * (float) image[i][j].rgbtRed) +
                (.534 * (float) image[i][j].rgbtGreen) +
                (.131 * (float) image[i][j].rgbtBlue);
            sepiaBlue = round(floatBlue);


            // sepiaRed = round(
            //     (.393 * image[i][j].rgbtBlue) +
            //     (.769 * image[i][j].rgbtGreen) +
            //     (.189 * image[i][j].rgbtRed));

            // sepiaGreen = round(
            //     (.349 * image[i][j].rgbtBlue) +
            //     (.686 * image[i][j].rgbtGreen) +
            //     (.168 * image[i][j].rgbtRed));

            // sepiaBlue = round(
            //     (.272 * image[i][j].rgbtBlue) +
            //     (.534 * image[i][j].rgbtGreen) +
            //     (.131 * image[i][j].rgbtRed));

            // validate that the sepia colors doesn't are greater than 255
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


            // assign the new gray scale to the pixel
            image[i][j].rgbtRed  = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int tempRed, tempGreen, tempBlue, middle;
    // float temp_middle;
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        middle = round(width / 2);
        for (int j = 0; j < middle; j++)
        {
            // do swap
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    int sumRed, sumGreen, sumBlue, counter;
    float floatRed, floatGreen, floatBlue;
    int avgRed, avgGreen, avgBlue;


    RGBTRIPLE blur_image[height][width];
    
    // copy image to blur_image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blur_image[i][j] = image[i][j];
        }
    }



    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sumRed = 0;
            sumGreen = 0;
            sumBlue = 0;
            counter = 0;

            // 1 ROW ABOVE
            if (i > 0)
            {
                // top left
                if (j > 0)
                {
                    sumRed += image[i - 1][j - 1].rgbtRed;
                    sumGreen += image[i - 1][j - 1].rgbtGreen;
                    sumBlue += image[i - 1][j - 1].rgbtBlue;
                    counter += 1;
                }

                // top middle
                sumRed += image[i - 1][j].rgbtRed;
                sumGreen += image[i - 1][j].rgbtGreen;
                sumBlue += image[i - 1][j].rgbtBlue;
                counter += 1;

                // top right
                if (j < width - 1)
                {
                    sumRed += image[i - 1][j + 1].rgbtRed;
                    sumGreen += image[i - 1][j + 1].rgbtGreen;
                    sumBlue += image[i - 1][j + 1].rgbtBlue;
                    counter += 1;
                }
            }

            // CURRENT ROW
            // current row, left
            if (j > 0)
            {
                sumRed += image[i][j - 1].rgbtRed;
                sumGreen += image[i][j - 1].rgbtGreen;
                sumBlue += image[i][j - 1].rgbtBlue;
                counter += 1;
            }

            //  current possition
            sumRed += image[i][j].rgbtRed;
            sumGreen += image[i][j].rgbtGreen;
            sumBlue += image[i][j].rgbtBlue;
            counter += 1;

            //  current row, right
            if (j < width - 1)
            {
                sumRed += image[i][j + 1].rgbtRed;
                sumGreen += image[i][j + 1].rgbtGreen;
                sumBlue += image[i][j + 1].rgbtBlue;
                counter += 1;
            }


            // 1 ROW BELOW
            if (i < height - 1)
            {
                //  bottom left
                if (j > 0)
                {
                    sumRed += image[i + 1][j - 1].rgbtRed;
                    sumGreen += image[i + 1][j - 1].rgbtGreen;
                    sumBlue += image[i + 1][j - 1].rgbtBlue;
                    counter += 1;
                }

                // bottom middle
                sumRed += image[i + 1][j].rgbtRed;
                sumGreen += image[i + 1][j].rgbtGreen;
                sumBlue += image[i + 1][j].rgbtBlue;
                counter += 1;

                //  bottom left
                if (j < width - 1)
                {
                    sumRed += image[i + 1][j + 1].rgbtRed;
                    sumGreen += image[i + 1][j + 1].rgbtGreen;
                    sumBlue += image[i + 1][j + 1].rgbtBlue;
                    counter += 1;
                }
            }


            // GET AVERAGES AND NEW VALUES
            floatRed = (float) sumRed / (float) counter;
            floatGreen = (float) sumGreen / (float) counter;
            floatBlue = (float) sumBlue / (float) counter;


            avgRed = round(floatRed);
            avgGreen = round(floatGreen);
            avgBlue = round(floatBlue);



            if (avgRed > 255)
            {
                avgRed = 255;
            }
            if (avgGreen > 255)
            {
                avgGreen = 255;
            }
            if (avgBlue > 255)
            {
                avgBlue = 255;
            }

            // SET THE NEW VALUES
            blur_image[i][j].rgbtRed = avgRed;
            blur_image[i][j].rgbtGreen = avgGreen;
            blur_image[i][j].rgbtBlue = avgBlue;
        }
    }

    // copy blur_image to image
    // for some reason, I can do this: (I need to traverse all the matrix)
    // image = blur_image;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blur_image[i][j];
        }
    }
            
    return;
}
