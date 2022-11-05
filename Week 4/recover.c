#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// define BYTE
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for exactly one argument
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // open file and check for errors
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // set up variables for control
    int counter = 0;
    FILE *tmp = NULL;
    char filename[8];

    // set up buffer
    BYTE buffer[512];

    // Ready file in 512 Byte chucks
    while (fread(buffer, 1, 512, file) == 512)
    {
        // Check for JPG header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Check if we are already writing into the file
            // if so, close the file first
            if (tmp != NULL)
            {
                fclose(tmp);
            }

            // let's create a new filename
            sprintf(filename, "%03i.jpg", counter);

            // increase the counter
            counter++;

            // open the file
            tmp = fopen(filename, "a");

            // write to the file
            fwrite(buffer, 1, 512, tmp);
        }
        else
        {
            // Are we already writing? If so, continue writing
            if (tmp != NULL)
            {
                fwrite(buffer, 1, 512, tmp);
            }
        }
    }

    // Is the temp file open? If so, close it.
    if (tmp != NULL)
    {
        fclose(tmp);
    }
    // Close the memory card file
    fclose(file);
}