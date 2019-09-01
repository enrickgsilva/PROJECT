#include "estruturas.h"
#include "compactar.h"
#include "descompactar.h"

int main()
{
    int choice;
    system("clear");
    printf("Seja bem vindo ao WME™ Compress!\n");

    while (1)
    {
        printf("\nEscolha uma das opções abaixo:\n[1] - Compactar\n[2] - Descompactar\n[3] - Sair\n");
        scanf("%d", &choice);

        if (choice == 1)
        {
            compactar();
        }
        else if (choice == 2)
        {
            descompactar();
        }
        else if (choice == 3)
        {
            printf("\nAté mais!\n");
            break;
        }
        else
        {
            printf("Opção não encontrada, digite uma opção válida\n");
        }
        printf("\nDeseja continuar no programa?\n"); printf("[1] - SIM | NÂO - [2]\n");
        scanf("%d", &choice);
        system("clear");
        if(choice == 2) break;
   }
    
    return 0;
}