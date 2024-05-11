#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t byte;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage Error: ./program-name <filename>\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Usage Error: Cant open < %s > file\n", argv[1]);
        return 1;
    }

    byte buffer_pic[512];

    int counter = 0;

    int jpg_cc = 0;
    int jpg_one = 1;
    int flag = 0;

    FILE *pic_G = NULL;

    while (fread(buffer_pic, sizeof(byte), 512, input))
    {
        if (buffer_pic[0] == 0xff && buffer_pic[1] == 0xd8 && buffer_pic[2] == 0xff && (buffer_pic[3] & 0xf0) == 0xe0)
        {

            if (jpg_one == 0)
            {
                fclose(pic_G);
            }
            flag = 1;
            jpg_cc = 1;
        }
        else
        {
            jpg_cc = 0;
        }

        if (jpg_cc == 1)
        {
            jpg_one = 0;

            char filename[255];
            sprintf(filename, "%03i.jpg", counter);
            counter++;

            pic_G = fopen(filename, "w");
            if (pic_G == NULL)
            {
                return 1;
            }
        }

        if (flag == 1)
        {
            fwrite(buffer_pic, sizeof(byte), 512, pic);
        }

    }

    fclose(input);
    fclose(pic_G);
    return 0;
