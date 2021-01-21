#include <stdio.h>
#include <cs50.h>
#include <stdbool.h>

// Problem: Receive a number, and dependig on the rules, return which card is
// All AMEX start with 34 or 37;  15-digit numbers,
// MASTERCARD start with 51, 52, 53, 54, or 55, uses 16-digit numbers
// VISA numbers start with 4. uses 13- or 16-digit
// INVALID

// STEPS
// ask until I got a long number w/o symbols
// get a way to id the number long and position (% 10)

// get the checksum
// dependig on long check starting point, if fits in a category and checksum =0, print the category
// else, print INVALID

int get_first_digits(long number, int num_total_digits, int nums_from_left);


int main(void)
{
    long number;

    // step 1, get the CC number
    do
    {
        number = get_long("Number: ");
    }
    while (number <= 0);


    int digit;
    long reduced_number = number;
    int sum_not_every_other = 0, sum_every_other = 0, product = 0, num_positions = 0;
    bool in_every_other = false;
    int temp_sum = 0, total_sum = 0;

    // traverse the number to get each position
    do
    {
        digit = reduced_number % 10;

        if (in_every_other)
        {
            product = digit * 2;

            if (product > 9)
            {
                // I need to sum the unit with the decimals
                temp_sum = (product % 10) + ((product / 10) % 10);

                sum_every_other += temp_sum;
            }
            else
            {
                sum_every_other += product;
            }
        }
        else
        {
            sum_not_every_other += digit;
        }

        // change the every other way
        in_every_other = !in_every_other;

        // get the count of the number's lenght
        num_positions ++;

        // now, reduce the number for the next cycle
        reduced_number /= 10;


    }
    while (reduced_number > 0);


    // now, add both sums
    total_sum = sum_every_other + sum_not_every_other;

    int first_digits = 0;

    if ((num_positions == 16) || (num_positions == 15) || (num_positions == 13))
    {
        if (total_sum % 10 == 0)
        {
            first_digits = get_first_digits(number, num_positions, 2);
            // check AMEX
            if (num_positions == 15)
            {
                if ((first_digits == 34) || (first_digits == 37))
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }

            }

            // check MASTERCARD or visa with 16 digits
            if (num_positions == 16)
            {
                //  check if is a valid mastercar
                if ((first_digits >= 51) && (first_digits <= 55))
                {
                    printf("MASTERCARD\n");
                }
                // check if is a visa of 16 digits
                else if (first_digits / 10 == 4)
                {
                    printf("VISA\n");
                }
                else
                {
                    printf("INVALID\n");
                }

            }


            // check VISA with 13 digits
            if (num_positions == 13)
            {
                if (first_digits / 10 == 4)
                {
                    printf("VISA\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}


// function that returns n digits from left to right of a given number
int get_first_digits(long number, int num_total_digits, int nums_from_left)
{
    for (int i = 1; i <= (num_total_digits - nums_from_left); i++)
    {
        number = number / 10;
    }

    return number;

}