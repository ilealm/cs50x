// Week 2
// Readability
// Implement a program that computes the approximate grade level needed to comprehend some text, per the below.

// Text: Congratulations! Today is your day. You're off to Great Places! You're off and away!
// Grade 3

#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int get_index(int letters, int words, int sentences);
int print_grade(int index);


int main(void)
{
    int letters = 0;
    int words = 1;
    int sentences = 0;

    string text = get_string("Text: ");


    // traverse the string to get the count of letters, sentences and words.
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // check is is a letter.
        if (isalpha(text[i]))
        {
            letters++;
        }
        // check if the word ended.
        if (text[i] == ' ')
        {
            words ++;
        }
        // check if the sentence ended.
        if ((text[i] == '.') || (text[i] == '!') || (text[i] == '?'))
        {
            sentences ++;
        }
    }

    int index = get_index(letters, words, sentences);
    print_grade(index);

}


// Function that returns the rouded index of the lecture.
int get_index(int letters, int words, int sentences)
{
    float l, s, index;

    l = ((float) letters / (float) words) * 100;
    s = ((float) sentences / (float) words) * 100;

    index = 0.0588 * l - 0.296 * s - 15.8;

    return round(index);
}


// function thar prints the lecture's grade.
int print_grade(int index)
{
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index <= 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

    return 0;
}