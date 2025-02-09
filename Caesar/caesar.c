#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // Check if the correct number of command-line arguments is provided
    if (argc != 2 || strlen(argv[1]) != 1)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Check if the key is a single digit argv[1][0] checks 2nd "string" in argv and after that access the first charackter from argv[1] so ./ceasar[0] key[1] and k[0]ey 
    if (argv[1][0] < '0' || argv[1][0] > '9')
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert the key to an integer
    int key = atoi(argv[1]);

    // Get the plaintext from the user
    string plaintext = get_string("Plain text:  ");

    // Encrypt the plaintext using the Caesar cipher
    printf("Ciphertext: ");
    for (int i = 0; i < strlen(plaintext); i++)
    {
        char original_char = plaintext[i];
        char cipher_char;

        if (original_char >= 'A' && original_char <= 'Z')
        {
            cipher_char = ((original_char - 'A' + key) % 26) + 'A';
        }
        else if (original_char >= 'a' && original_char <= 'z')
        {
            cipher_char = ((original_char - 'a' + key) % 26) + 'a';
        }
        else
        {
            cipher_char = original_char;
        }

        printf("%c", cipher_char);
    }

    printf("\n");

    return 0;
}
