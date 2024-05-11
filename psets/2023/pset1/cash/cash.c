#include <cs50.h>
#include <math.h>
#include <stdio.h>

float askPositiveFloat(void)
{
    float positiveFloat;
    bool negative = true;

    while (negative)
    {
        positiveFloat = get_float("Change owed: ");

        if (positiveFloat > 0)
        {
            negative = false;
        }
    }
    return positiveFloat;
}

int coinNum()
{
    int totalCents = round(askPositiveFloat() * 100);
    int numOfCoins = 0;
    int remainder;
    int tempNum;
    int quarter = 25;
    int dime = 10;
    int nickel = 5;
    int penny = 1;

    if (totalCents >= quarter)
    {
        remainder = totalCents % quarter;
        tempNum = totalCents - remainder;
        numOfCoins = numOfCoins + (tempNum / quarter);
        totalCents = remainder;
    }
    if (totalCents >= dime)
    {
        remainder = totalCents % dime;
        tempNum = totalCents - remainder;
        numOfCoins = numOfCoins + (tempNum / dime);
        totalCents = remainder;
    }
    if (totalCents >= nickel)
    {
        remainder = totalCents % nickel;
        tempNum = totalCents - remainder;
        numOfCoins = numOfCoins + (tempNum / nickel);
        totalCents = remainder;
    }
    if (totalCents >= penny)
    {
        remainder = totalCents % penny;
        tempNum = totalCents - remainder;
        numOfCoins = numOfCoins + (tempNum / penny);
        totalCents = remainder;
    }
    return numOfCoins;
}

int main(void)
{
    printf("%i\n", coinNum());
}
