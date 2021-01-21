#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>


int validate_key(string key);
string encrypt(string key, string plaintext);
int get_alpha_position(char letter);


int main(int argc, string argv[])
{
    string key =  argv[1];
    string plaintext;


    // validate comma-line input
    if (argc != 2)
    {
        printf("Error on command-line input.\n");
        return 1;
    }

    // validate comma-line input
    if (validate_key(key) == 1)
    {
        printf("Invalid key.\n");
        return 1;
    }


    // ask plaintext
    do
    {
        plaintext = get_string("plaintext: ");
    }
    while (strlen(plaintext) == 0);

    // string encrypted_text = encrypt(key, plaintext);
    encrypt(key, plaintext);
}



// function that validates if the key is valid key
int validate_key(string key)
{
    int len_key = strlen(key);
    char upper_i;

    // check that contains only 26 characters
    if (strlen(key) != 26)
    {
        return 1;
    }

    // check that cointains only alphabetic characters
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (!isalpha(key[i]))
        {
            return 1;
        }
    }

    // check for duplicated values on key
    for (int i = 0; i < len_key; i++)
    {
        // for eficiency, I will only upper i once.
        upper_i = toupper(key[i]);
        for (int j = i + 1; j < len_key; j++)
        {
            if (upper_i == toupper(key[j]))
            {
                return 1;
            }
        }
    }

    return 0;
}


// function that translate plaintext to a encrypt text using the key
string encrypt(string key, string plaintext)
{

    int alpha_position;
    char encrypted[strlen(plaintext)];

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        // Only encrypt characters.
        if (isalpha(plaintext[i]))
        {
            alpha_position = get_alpha_position(plaintext[i]);
            // return the character in the rigth case, upper or lower
            if (islower(plaintext[i]))
            {
                encrypted[i] = tolower(key[alpha_position]);
            }
            else
            {
                encrypted[i] = toupper(key[alpha_position]);
            }
        }
        else
        {
            encrypted[i] = plaintext[i];
        }

    }

    printf("ciphertext: %s\n", encrypted);

    return 0;
}


// funcion that receives an alphabetic letter and returns its position -1 in the alphabet
int get_alpha_position(char letter)
{
    char alphabeth[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    char upper_letter = toupper(letter);

    for (int i = 0; i < 26; i++)
    {
        if (alphabeth[i] == upper_letter)
        {
            return i;
        }
    }

    return 1;

}