#include <stdio.h>
#include <cs50.h>


int main(void)
{
    int height;
    // first, I will get a hight with a positive value value between 1 and 8
    do
    {
        height = get_int("Height: ");
    }
    while ((height < 1) || (height > 8)) ;

    // now, use a for for print the pyramid
    string symbol = "#";


    for (int row = 1; row <= height; row++)
    {

        // print the left side
        for (int col = 0; col < height; col++)
        {
            if (col < height - row)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }

        // print the middle gap
        printf("  ");


        // print the right side
        for (int col = 0; col < height; col++)
        {
            if (col < row)
            {
                printf("#");
            }
        }

        printf("\n");
    }
}

