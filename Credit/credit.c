#include <cs50.h>
#include <stdio.h>

// Function to count the number of digits
bool luhnAlgorithm(long long cardNumber);
int countDigits(long long number)
{
    if (number == 0)
    {
        return 1;
    }

    int count = 0;

    while (number != 0)
    {
        number /= 10;
        count++;
    }

    return count;
}
// Function to check if the card number is a valid Visa card
bool isValidVisa(long long cardNumber)
{
    int count = countDigits(cardNumber);

    // Check if the card type is Visa (starts with 4), has either 13 or 16 digits, and passes Luhn's Algorithm
    return (cardNumber / 1000000000000 == 4) && (count == 13 || count == 16) && luhnAlgorithm(cardNumber);
}
// function to check if valid american express
bool isValidAmericanExpress(long long cardNumber)
{
    int count = countDigits(cardNumber);
    int firstTwoDigits = 0;

    if (count == 15)
    {
        while (cardNumber >= 100)
        {
            cardNumber /= 10;
        }
        firstTwoDigits = (int) cardNumber;
    }

    if (firstTwoDigits == 34 || firstTwoDigits == 37)
    {
        return true;
    }
    return false;
}

// function to check if its valid master card
bool isValidMasterCard(long long cardNumber)
{
    int count = countDigits(cardNumber);
    int firstTwoDigits = 0;

    if (count == 16)
    {
        while (cardNumber >= 100)
        {
            cardNumber /= 10;
        }
        firstTwoDigits = (int) cardNumber;
    }

    if (firstTwoDigits == 51 || firstTwoDigits == 52 || firstTwoDigits == 53 || firstTwoDigits == 54 || firstTwoDigits == 55)
    {
        return true;
    }
    return false;
}

bool luhnAlgorithm(long long cardNumber)
{
    int sum = 0;
    int position = 0;

    while (cardNumber > 0)
    {
        int digit = cardNumber % 10;

        if (position % 2 == 1)
        {
            digit *= 2;

            if (digit >= 10)
            {
                sum += digit % 10;
                digit /= 10;
            }
        }

        sum += digit;
        cardNumber /= 10;
        position++;
    }

    return (sum % 10 == 0);
}

int main(void)
{
    long long user_input_cardnumber = get_long("Please enter your Cardnumber: \n");

    if (isValidVisa(user_input_cardnumber) && luhnAlgorithm(user_input_cardnumber))
    {
        printf("Visa Card\n");
    }
    else if (isValidMasterCard(user_input_cardnumber) && luhnAlgorithm(user_input_cardnumber))
    {
        printf("Valid Master Card\n");
    }
    else if (isValidAmericanExpress(user_input_cardnumber) && luhnAlgorithm(user_input_cardnumber))
    {
        printf("American Express\n");
    }
    else
    {
        printf("Invalid Card\n");
    }
    return 0;
}
