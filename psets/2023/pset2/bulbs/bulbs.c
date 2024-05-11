#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);
int numobit(int num);
void snttoprt(int bit);

int main(void)
{
    string msg = get_string("Message: ");

    int msglen = strlen(msg);

    for (int k = 0; k < msglen; k++)
    {

        snttoprt(numobit(msg[k]));
        printf("\n");
    }

    return 0;
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        printf("\U0001F7E1");
    }
}

int numobit(int num)
{
    int bin = 100000000;
    int temp = 100000000;
    int n = num;
    for (int i = 0; i <= 7; i++)
    {
        if ((n % 2) == 0)
        {
            // bin += 0;
            n /= 2;
        }
        else if (n % 2 != 0)
        {
            bin += (1 * pow(10, 7 - i));
            if (n > 0)
            {
                n--;
            }
            n /= 2;
        }
    }
    return bin - temp;
}

void snttoprt(int bit)
{
    for (int t = 0; t <= 7; t++)
    {
        print_bulb(bit % 10);
        bit /= 10;
    }
}
