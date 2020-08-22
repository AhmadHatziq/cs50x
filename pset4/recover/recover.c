#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <cs50.h>

typedef uint8_t BYTE;

bool is_header(BYTE bytes_buffer[512]);

int main(int argc, char *argv[])
{
    // Check for no. or args supplied
    if (argc != 2)
    {
        printf("Usage: ./recover image \n");
        return 1;
    }

    // Open file
    char *file_name = argv[1];
    FILE *source_file = fopen(file_name, "r");

    // Check if file name is valid
    if (!source_file)
    {
        printf("%s cannot be opened \n", file_name);
        return 1;
    }

    // Declare variables
    int image_count = 0;
    BYTE current_buffer[512];
    int byte_count = 0;
    bool first_jpeg = true;
    FILE *output_file = NULL;
    char output_filename[512];

    // Test with iterating 3 blocks
    while (true)
    {

        // Read in the bytes in 512 chunks
        int bytes_read = fread(current_buffer, sizeof(BYTE), 512, source_file);
        byte_count = byte_count + 1;

        // Exit and write previous chunk if 0 bytes are read
        if (bytes_read == 0)
        {
            // printf("Byte count on exit: %i \n", byte_count);
            // printf("Total headers: %i \n", image_count);
            break;
        }

        // Check if the current block contains a JPEG header
        if (is_header(current_buffer) == true)
        {
            image_count = image_count + 1;

            // First jpeg file. Open a new file for saving
            if (first_jpeg == true)
            {

                first_jpeg = false;
                sprintf(output_filename, "%03i.jpg", image_count - 1);
                output_file = fopen(output_filename, "w");
                fwrite(current_buffer, sizeof(BYTE), bytes_read, output_file);

            }
            else
            {

                // Subsequent jpegs. Close previous and create a new file
                fclose(output_file);
                sprintf(output_filename, "%03i.jpg", image_count - 1);
                output_file = fopen(output_filename, "w");
                fwrite(current_buffer, sizeof(BYTE), bytes_read, output_file);

            }

        }
        else if (output_file != NULL)
        {
            // Only save when we have an output ptr active
            fwrite(current_buffer, sizeof(BYTE), bytes_read, output_file);
        }

    }

    // Close files
    fclose(source_file);
    fclose(output_file);

    return 0;
}

// Checks if the first 4 bytes follow the JPEG standard
bool is_header(BYTE bytes_buffer[512])
{
    if ((bytes_buffer[0] == 0xff) && (bytes_buffer[1] == 0xd8) && (bytes_buffer[2] == 0xff) && ((bytes_buffer[3] & 0xf0) == 0xe0))
    {
        return true;
    }
    else
    {
        return false;
    }
}