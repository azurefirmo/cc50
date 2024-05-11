#import <cs50.h>
#import <stdio.h>

int main(void)
{

    int answer;

    do
    {

        answer = get_int("Put an integer:");
    }
    while (answer < 0);

    if (answer > 8)
    {
        answer = get_int("Put an integer:");
    }

    printf("Height: %i\n", answer);

    // for(int i = answer; i>0; i--){
    //  for (int j=1; j<i; j++){
    //  printf(".");

    //  for(int k=0; k<j; k++){
    //   printf("#");
    //   }
    //     printf("\n");
    //   }
    // }

    //  printf("\n");

    // for(int i=0; i<answer; i++){
    //    for(int j=0; j<=i; j++){

    //     printf("#");
    //     }

    //     printf("\n");
    //    }

    for (int i = answer; i > 0; i--)
    {
        for (int j = i; j > 1; j--)
        {
            printf(" ");
        }
        for (int m = 0; m <= (answer - i); m++)
        {

            printf("#");
        }

        printf("\n");
    }
}
