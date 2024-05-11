#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


bool is26CharsLong(string);
bool isAlphaString(string);
bool isNotRepeating(string);
void encipher(string, string);

int main(int argc, string argv[])
{

    string key = argv[1];

    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (!is26CharsLong(argv[1]))
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (!isAlphaString(argv[1]))
    {
        printf("Key must only contain alphabetic characters.\n");
        return 1;
    }

    else if (!isNotRepeating(argv[1]))
    {
        printf("Key must not contain repeated characters.\n");
        return 1;
    }

    string plaintext = get_string("plaintext: ");

    encipher(plaintext, key);

    printf("ciphertext: %s\n", plaintext);
}


bool is26CharsLong(string s)
{
    int keyCounter;
    for (keyCounter = 0; s[keyCounter] != '\0'; keyCounter++);

    if (keyCounter != 26)
    {
        return false;
    }

    return true;
}


bool isAlphaString(string s)
{
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (!isalpha(s[i]))
        {
            return false;
        }
    }

    return true;
}


bool isNotRepeating(string s)
{
    for (int target = 0; s[target] != '\0'; target++)
    {
        int repeated = 0;
        for (int compare = 0; s[compare] != '\0'; compare++)
        {
            if (s[compare] == s[target])
            {
                repeated++;
            }
        }
        if (repeated > 1)
        {
            return false;
        }
    }
    return true;
}


void encipher(string plaintext, string key)
{
    for (int i = 0; plaintext[i] != '\0'; i++)
    {
        int char_pos;
        char_pos = toupper(plaintext[i]) - 'A';

        if (isupper(plaintext[i]))
        {
            plaintext[i] = key[char_pos];
            plaintext[i] = toupper(plaintext[i]);
        }

        if (islower(plaintext[i]))
        {
            plaintext[i] = key[char_pos];
            plaintext[i] = tolower(plaintext[i]);
        }
    }
}
