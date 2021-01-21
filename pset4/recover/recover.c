#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;
    const int FAT_SIZE = 512;

    // check for invalid usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // OPEN THE INPUT FILE
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");

    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }


    // custom data types
    BYTE bf_fat[FAT_SIZE];
    int counter = 0;
    char filename[50];
    FILE *img;

    // READ FILE, while I don't have else to read
    while (fread(&bf_fat, sizeof(BYTE), FAT_SIZE, inptr) >= FAT_SIZE)
    {
        // check if I have a new header
        if ((bf_fat[0] == 0xff) &&
            (bf_fat[1] == 0xd8) &&
            (bf_fat[2] == 0xff) &&
            ((bf_fat[3] & 0xf0) == 0xe0))
        {
            // printf("new header!\n");
            // check if I already have a file opened, if so, close it
            if (img != NULL)
            {
                // printf("\nclosing %s \n\n\n", filename);
                fclose(img);
            }

            // open new file to write
            // set the name of the newfile_name. This function returns to filename the formatted
            // counter with 3 positons, plus zeros to left: 001, 050
            sprintf(filename, "%03i.jpg", counter);
            // printf("opening: %s\n", filename);

            img = fopen(filename, "w");
            // write the header in the new file
            fwrite(&bf_fat, sizeof(BYTE), FAT_SIZE, img);

            counter ++;
        }
        else
        {
            // I have data to write
            // check that I have an open file, to avoid segmentation, BC the file can (and in this case does) have garbage ar the beggining
            // that shouldn't be write to the file.
            if (img != NULL)
            {
                fwrite(&bf_fat, sizeof(BYTE), FAT_SIZE, img);
            }
            // printf("0");
        }
    }

    // closing files
    if (inptr != NULL)
    {
        fclose(inptr);
    }

    if (img != NULL)
    {
        printf("\nclosing final file %s \n", filename);
        fclose(img);
    }

    return 0;
}