#include <stdio.h>

int main(void)
{
    int valor_inicial, valor_final, anos;

    // Solicite o valor inicial ao usuário
    printf("Digite o valor inicial: ");
    scanf("%d", &valor_inicial);

    // Solicite o valor final ao usuário
    printf("Digite o valor final: ");
    scanf("%d", &valor_final);

    // Calcule o número de anos até o limite
    anos = valor_final - valor_inicial;

    // Imprima o número de anos
    printf("O número de anos até o limite é: %d\n", anos);

    return 0;
}
