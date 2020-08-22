#include "helpers.h"
#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

// Function prototypes
RGBTRIPLE central_blur(int row, int col, int height, int width, RGBTRIPLE image[height][width]);
RGBTRIPLE first_row_blur(int col, int height, int width, RGBTRIPLE image[height][width]);
RGBTRIPLE last_row_blur(int col, int height, int width, RGBTRIPLE image[height][width]);
RGBTRIPLE first_col_blur(int row, int height, int width, RGBTRIPLE image[height][width]);
RGBTRIPLE last_col_blur(int row, int height, int width, RGBTRIPLE image[height][width]);

// Convert image to grayscale
// RGBTRIPLE is a struct defined in bmp.h ie
/*
typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE; */
// Function does not return anything as it changes the value directly via the array / pointers
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // Extract current pixel
            RGBTRIPLE current_pixel = image[i][j];
            BYTE current_blue = current_pixel.rgbtBlue;
            BYTE current_green = current_pixel.rgbtGreen;
            BYTE current_red = current_pixel.rgbtRed;

            // Calculate the average BYTE
            BYTE rgb_average = round(((float)(current_blue) + (float)(current_green) + (float)(current_red))  / 3.0);

            // Alter the current_pixel values
            current_pixel.rgbtBlue = rgb_average;
            current_pixel.rgbtGreen = rgb_average;
            current_pixel.rgbtRed = rgb_average;

            // Store the value back in the array
            image[i][j] = current_pixel;

        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // Extract current pixel
            RGBTRIPLE current_pixel = image[i][j];
            BYTE current_blue = current_pixel.rgbtBlue;
            BYTE current_green = current_pixel.rgbtGreen;
            BYTE current_red = current_pixel.rgbtRed;

            // Calculate the sepia values
            int sepiaRed = round(0.393 * (float)current_red + 0.769 * (float)current_green + 0.189 * (float)current_blue);
            int sepiaGreen = round(0.349 * (float) current_red + 0.686 * (float) current_green + 0.168 * (float) current_blue);
            int sepiaBlue = round(0.272 * (float) current_red + 0.534 * (float) current_green + 0.131 * (float) current_blue);

            // Reset any values more than 255
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

            // Reset any values less than 0
            if (sepiaRed < 0)
            {
                sepiaRed = 0;
            }
            if (sepiaGreen < 0)
            {
                sepiaGreen = 0;
            }
            if (sepiaBlue < 0)
            {
                sepiaBlue = 0;
            }

            // Cast back to BYTE
            BYTE sepia_blue = sepiaBlue;
            BYTE sepia_green = sepiaGreen;
            BYTE sepia_red = sepiaRed;

            // Alter the current_pixel values
            current_pixel.rgbtBlue = sepia_blue;
            current_pixel.rgbtGreen = sepia_green;
            current_pixel.rgbtRed = sepia_red;


            // Store the value back in the array
            image[i][j] = current_pixel;

        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary array for the swapping
    int temp_array[3];

    // Iterate over rows
    for (int i = 0; i < height; i++)
    {

        // Iterate over columns
        for (int j = 0; j < width / 2; j++)
        {

            // Extract left pixel and store inside temp
            RGBTRIPLE left_pixel = image[i][j];
            temp_array[0] = left_pixel.rgbtBlue;
            temp_array[1] = left_pixel.rgbtGreen;
            temp_array[2] = left_pixel.rgbtRed;

            // Move right pixel into left pixel
            RGBTRIPLE right_pixel = image[i][width - 1 - j];
            left_pixel.rgbtBlue = right_pixel.rgbtBlue;
            left_pixel.rgbtGreen = right_pixel.rgbtGreen;
            left_pixel.rgbtRed = right_pixel.rgbtRed;
            image[i][j] = left_pixel;

            // Assign temp to the right pixel
            right_pixel = image[i][width - 1 - j];
            right_pixel.rgbtBlue = temp_array[0];
            right_pixel.rgbtGreen = temp_array[1];
            right_pixel.rgbtRed = temp_array[2];
            image[i][width - 1 - j] = right_pixel;

        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare new image array to store the new pixel values
    RGBTRIPLE new_image[height][width];

    // Blur those that can be blurred with a 3x3 box
    for (int row = 1; row < height - 1; row++)
    {
        for (int col = 1; col < width - 1; col++)
        {

            // Generate new pixel
            RGBTRIPLE new_pixel = central_blur(row, col, height, width, image);

            // Store new pixel
            new_image[row][col] = new_pixel;

        }
    }

    // Blur pixels in the first row
    for (int col = 0; col < width; col++)
    {
        RGBTRIPLE first_row_pixel = first_row_blur(col, height, width, image);
        new_image[0][col] = first_row_pixel;
    }


    // Blur pixels in the last row
    for (int col = 0; col < width; col++)
    {

        RGBTRIPLE last_row_pixel = last_row_blur(col, height, width, image);
        new_image[height - 1][col] = last_row_pixel;
    }

    // Blur the remaining pixels in the first column
    for (int row = 1; row <= height - 2; row++)
    {
        RGBTRIPLE first_col_pixel = first_col_blur(row, height, width, image);
        new_image[row][0] = first_col_pixel;
    }

    // Blur the remaining pixels in the last column
    for (int row = 1; row <= height - 2; row++)
    {
        RGBTRIPLE last_col_pixel = last_col_blur(row, height, width, image);
        new_image[row][width - 1] = last_col_pixel;
    }

    // Copy over the values to replace the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }

    return;

}

// Get the blurred pixel RGB values for the last col
RGBTRIPLE last_col_blur(int row, int height, int width, RGBTRIPLE image[height][width])
{
    // Top and bottom pixels are handled by the first/last row methods
    // Each pixel is a 3x2 grid

    // Extract pixel data
    RGBTRIPLE top_left = image[row - 1][width - 2];
    RGBTRIPLE top_right = image[row - 1][width - 1];
    RGBTRIPLE main_pixel = image[row][width - 1]; // main is centre right
    RGBTRIPLE centre_left = image[row][width - 2];
    RGBTRIPLE bottom_left = image[row + 1][width - 2];
    RGBTRIPLE bottom_right = image[row + 1][width - 1];

    // Create a pixel with the average of all 6 pixels
    RGBTRIPLE new_pixel;
    new_pixel.rgbtBlue = round(((float)main_pixel.rgbtBlue + (float)centre_left.rgbtBlue
                                + (float)top_left.rgbtBlue + (float)top_right.rgbtBlue + (float)bottom_left.rgbtBlue
                                + (float)bottom_right.rgbtBlue) / 6.0);

    new_pixel.rgbtGreen = round(((float)main_pixel.rgbtGreen + (float)centre_left.rgbtGreen
                                 + (float)top_left.rgbtGreen + (float)top_right.rgbtGreen + (float)bottom_left.rgbtGreen
                                 + (float)bottom_right.rgbtGreen) / 6.0);

    new_pixel.rgbtRed = round(((float)main_pixel.rgbtRed + (float)centre_left.rgbtRed
                               + (float)top_left.rgbtRed + (float)top_right.rgbtRed + (float)bottom_left.rgbtRed
                               + (float)bottom_right.rgbtRed) / 6.0);

    return new_pixel;

}

// Get the blurred pixel RGB values for the first col
RGBTRIPLE first_col_blur(int row, int height, int width, RGBTRIPLE image[height][width])
{
    // Top and bottom pixels are handled by the first/last row methods
    // Each pixel is a 3x2 grid

    // Extract pixel data
    RGBTRIPLE top_left = image[row - 1][0];
    RGBTRIPLE top_right = image[row - 1][1];
    RGBTRIPLE main_pixel = image[row][0];
    RGBTRIPLE centre_right = image[row][1];
    RGBTRIPLE bottom_left = image[row + 1][0];
    RGBTRIPLE bottom_right = image[row + 1][1];

    // Create a pixel with the average of all 6 pixels
    RGBTRIPLE new_pixel;
    new_pixel.rgbtBlue = round(((float)main_pixel.rgbtBlue + (float)centre_right.rgbtBlue
                                + (float)top_left.rgbtBlue + (float)top_right.rgbtBlue + (float)bottom_left.rgbtBlue
                                + (float)bottom_right.rgbtBlue) / 6.0);

    new_pixel.rgbtGreen = round(((float)main_pixel.rgbtGreen + (float)centre_right.rgbtGreen
                                 + (float)top_left.rgbtGreen + (float)top_right.rgbtGreen + (float)bottom_left.rgbtGreen
                                 + (float)bottom_right.rgbtGreen) / 6.0);

    new_pixel.rgbtRed = round(((float)main_pixel.rgbtRed + (float)centre_right.rgbtRed
                               + (float)top_left.rgbtRed + (float)top_right.rgbtRed + (float)bottom_left.rgbtRed
                               + (float)bottom_right.rgbtRed) / 6.0);

    return new_pixel;

}

// Get the blurred pixel RGB values for the last row
RGBTRIPLE last_row_blur(int col, int height, int width, RGBTRIPLE image[height][width])
{

    // Bottom left is only a 2x2 grid
    if (col == 0)
    {

        // Extract pixel data. NTS too lazy to change var names hence they do not make sense contex-wise
        RGBTRIPLE centre_pixel = image[height - 1][0];
        RGBTRIPLE top_right = image[height - 2][1];
        RGBTRIPLE top_left = image[height - 2][0];
        RGBTRIPLE bottom_right = image[height - 1][1];

        // Create a pixel with the average of all 4 pixels
        RGBTRIPLE new_pixel;
        new_pixel.rgbtBlue = round(((float)centre_pixel.rgbtBlue + (float)top_right.rgbtBlue
                                    + (float)top_left.rgbtBlue + (float)bottom_right.rgbtBlue) / 4.0);
        new_pixel.rgbtGreen = round(((float)centre_pixel.rgbtGreen + (float)top_right.rgbtGreen
                                     + (float)top_left.rgbtGreen + (float)bottom_right.rgbtGreen) / 4.0);
        new_pixel.rgbtRed = round(((float)centre_pixel.rgbtRed + (float)top_right.rgbtRed + (float)top_left.rgbtRed
                                   + (float)bottom_right.rgbtRed) / 4.0);

        return new_pixel;

    }
    else if (col == (width - 1))
    {
        // Bottom right is 2x2 grid

        // Extract pixel data
        RGBTRIPLE centre_pixel = image[height - 1][width - 1];
        RGBTRIPLE bottom_left = image[height - 1][width - 2];
        RGBTRIPLE top_right = image[height - 2][width - 1];
        RGBTRIPLE top_left = image[height - 2][width - 2];

        // Create a pixel with the average of all 4 pixels
        RGBTRIPLE new_pixel;
        new_pixel.rgbtBlue = round(((float)centre_pixel.rgbtBlue + (float)top_left.rgbtBlue
                                    + (float)bottom_left.rgbtBlue + (float)top_right.rgbtBlue) / 4.0);
        new_pixel.rgbtGreen = round(((float)centre_pixel.rgbtGreen + (float)top_left.rgbtGreen
                                     + (float)bottom_left.rgbtGreen + (float)top_right.rgbtGreen) / 4.0);
        new_pixel.rgbtRed = round(((float)centre_pixel.rgbtRed + (float)top_left.rgbtRed + (float)bottom_left.rgbtRed
                                   + (float)top_right.rgbtRed) / 4.0);

        return new_pixel;
    }
    else
    {
        // For all other last row pixels, extract a 2x3 grid

        // Extract pixel data
        RGBTRIPLE centre_pixel = image[height - 1][col];
        RGBTRIPLE bottom_left = image[height - 1][col - 1];
        RGBTRIPLE bottom_right = image[height - 1][col + 1];
        RGBTRIPLE top_left = image[height - 2][col - 1];
        RGBTRIPLE top_right = image[height - 2][col + 1];
        RGBTRIPLE above = image[height - 2][col];


        // Create a pixel with the average of all 4 pixels
        RGBTRIPLE new_pixel;
        new_pixel.rgbtBlue = round(((float)centre_pixel.rgbtBlue + (float)top_left.rgbtBlue
                                    + (float)top_right.rgbtBlue + (float)above.rgbtBlue + (float)bottom_left.rgbtBlue
                                    + (float)bottom_right.rgbtBlue) / 6.0);

        new_pixel.rgbtGreen = round(((float)centre_pixel.rgbtGreen + (float)top_left.rgbtGreen
                                     + (float)top_right.rgbtGreen + (float)above.rgbtGreen + (float)bottom_left.rgbtGreen
                                     + (float)bottom_right.rgbtGreen) / 6.0);

        new_pixel.rgbtRed = round(((float)centre_pixel.rgbtRed + (float)top_left.rgbtRed
                                   + (float)top_right.rgbtRed + (float)above.rgbtRed + (float)bottom_left.rgbtRed
                                   + (float)bottom_right.rgbtRed) / 6.0);

        return new_pixel;

    }
}


// Get the blurred pixel RGB values for the first row
RGBTRIPLE first_row_blur(int col, int height, int width, RGBTRIPLE image[height][width])
{

    // Top left is only a 2x2 grid
    if (col == 0)
    {

        // Extract pixel data
        RGBTRIPLE centre_pixel = image[0][0];
        RGBTRIPLE top_right = image[0][1];
        RGBTRIPLE bottom_left = image[1][0];
        RGBTRIPLE bottom_right = image[1][1];

        // Create a pixel with the average of all 4 pixels
        RGBTRIPLE new_pixel;
        new_pixel.rgbtBlue = round(((float)centre_pixel.rgbtBlue + (float)top_right.rgbtBlue
                                    + (float)bottom_left.rgbtBlue + (float)bottom_right.rgbtBlue) / 4.0);
        new_pixel.rgbtGreen = round(((float)centre_pixel.rgbtGreen + (float)top_right.rgbtGreen
                                     + (float)bottom_left.rgbtGreen + (float)bottom_right.rgbtGreen) / 4.0);
        new_pixel.rgbtRed = round(((float)centre_pixel.rgbtRed + (float)top_right.rgbtRed
                                   + (float)bottom_left.rgbtRed + (float)bottom_right.rgbtRed) / 4.0);

        return new_pixel;

    }
    else if (col == (width - 1))
    {
        // Top right is 2x2 grid

        // Extract pixel data
        RGBTRIPLE centre_pixel = image[0][width - 1];
        RGBTRIPLE top_left = image[0][width - 2];
        RGBTRIPLE bottom_left = image[1][width - 2];
        RGBTRIPLE bottom_right = image[1][width - 1];

        // Create a pixel with the average of all 4 pixels
        RGBTRIPLE new_pixel;
        new_pixel.rgbtBlue = round(((float)centre_pixel.rgbtBlue + (float)top_left.rgbtBlue
                                    + (float)bottom_left.rgbtBlue + (float)bottom_right.rgbtBlue) / 4.0);
        new_pixel.rgbtGreen = round(((float)centre_pixel.rgbtGreen + (float)top_left.rgbtGreen
                                     + (float)bottom_left.rgbtGreen + (float)bottom_right.rgbtGreen) / 4.0);
        new_pixel.rgbtRed = round(((float)centre_pixel.rgbtRed + (float)top_left.rgbtRed + (float)bottom_left.rgbtRed +
                                   (float)bottom_right.rgbtRed) / 4.0);

        return new_pixel;
    }
    else
    {
        // For all other first row pixels, extract a 2x3 grid

        // Extract pixel data
        RGBTRIPLE centre_pixel = image[0][col];
        RGBTRIPLE top_left = image[0][col - 1];
        RGBTRIPLE top_right = image[0][col + 1];
        RGBTRIPLE bottom = image[1][col];
        RGBTRIPLE bottom_left = image[1][col - 1];
        RGBTRIPLE bottom_right = image[1][col + 1];

        // Create a pixel with the average of all 6 pixels
        RGBTRIPLE new_pixel;
        new_pixel.rgbtBlue = round(((float)centre_pixel.rgbtBlue + (float)top_left.rgbtBlue
                                    + (float)top_right.rgbtBlue + (float)bottom.rgbtBlue + (float)bottom_left.rgbtBlue
                                    + (float)bottom_right.rgbtBlue) / 6.0);

        new_pixel.rgbtGreen = round(((float)centre_pixel.rgbtGreen + (float)top_left.rgbtGreen
                                     + (float)top_right.rgbtGreen + (float)bottom.rgbtGreen + (float)bottom_left.rgbtGreen
                                     + (float)bottom_right.rgbtGreen) / 6.0);

        new_pixel.rgbtRed = round(((float)centre_pixel.rgbtRed + (float)top_left.rgbtRed
                                   + (float)top_right.rgbtRed + (float)bottom.rgbtRed + (float)bottom_left.rgbtRed
                                   + (float)bottom_right.rgbtRed) / 6.0);

        return new_pixel;

    }
}

// Get the blurred pixel RGB values for a 3x3 box
RGBTRIPLE central_blur(int row, int col, int height, int width, RGBTRIPLE image[height][width])
{

    // Extract pixels
    RGBTRIPLE centre_pixel = image[row][col];
    RGBTRIPLE upper_left = image[row - 1][col - 1];
    RGBTRIPLE above = image[row - 1][col];
    RGBTRIPLE upper_right = image[row - 1][col + 1];
    RGBTRIPLE left = image[row][col - 1];
    RGBTRIPLE right = image[row][col + 1];
    RGBTRIPLE bottom_left = image[row + 1][col - 1];
    RGBTRIPLE below = image[row + 1][col];
    RGBTRIPLE bottom_right = image[row + 1][col + 1];

    // Create a pixel with the average of all 9 pixels
    RGBTRIPLE new_pixel;
    new_pixel.rgbtBlue = round(((float)centre_pixel.rgbtBlue + (float)upper_left.rgbtBlue + (float)above.rgbtBlue
                                + (float)upper_right.rgbtBlue + (float)left.rgbtBlue
                                + (float)right.rgbtBlue + (float)bottom_left.rgbtBlue + (float)below.rgbtBlue + (float)bottom_right.rgbtBlue) / 9.0);

    new_pixel.rgbtGreen = round(((float)centre_pixel.rgbtGreen + (float)upper_left.rgbtGreen + (float)above.rgbtGreen
                                 + (float)upper_right.rgbtGreen + (float)left.rgbtGreen
                                 + (float)right.rgbtGreen + (float)bottom_left.rgbtGreen + (float)below.rgbtGreen + (float)bottom_right.rgbtGreen) / 9.0);

    new_pixel.rgbtRed = round(((float)centre_pixel.rgbtRed + (float)upper_left.rgbtRed + (float)above.rgbtRed
                               + (float)upper_right.rgbtRed + (float)left.rgbtRed
                               + (float)right.rgbtRed + (float)bottom_left.rgbtRed + (float)below.rgbtRed + (float)bottom_right.rgbtRed) / 9.0);

    return new_pixel;
}
