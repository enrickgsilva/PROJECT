#include "estruturas.h"

void criar_arquivo(char *nomedoarquivo) {
    nomedoarquivo = strtok(nomedoarquivo, ".");
    nomedoarquivo = strcat(nomedoarquivo, ".huff");
}

void compactando_arquivo(FILE *arquivo, FILE *arquivo_final, HASH *hash)
{
    int contador = 0;
    unsigned char byte_atual, compactando = 0;

    printf("\nCompactando arquivo...\n");
    printf("\nIsto pode demorar um pouco...\n");
    while (!feof(arquivo))
    {
        byte_atual = fgetc(arquivo);
        int i;
        for (i = 0; i < strlen(hash->tabela[(int)byte_atual]->caminho); i++)
        {
            if (hash->tabela[(int)byte_atual]->caminho[i] == '1')
            {
                compactando = setar_bit(compactando, 7 - contador); 
            }
            contador++;

            if (contador == 8)
            {
                fprintf(arquivo_final, "%c", compactando);
                compactando = 0;
                contador = 0;
            }
        }
    }
    if (compactando != 0)
    {
        fprintf(arquivo_final, "%c", compactando);
    }
    printf("Arquivo compactado com sucesso!\n");
    fclose(arquivo);
    fclose(arquivo_final);
}

void compactar(){
    char nomedoarquivo[50];
    char caminho_da_arvore[TAMANHO_DA_TABELA];
    short lixo, tam_arvore, final;
    unsigned char byte;

    NODE *raiz = NULL;
    HASH *nova_hash = criar_hash();
    QUEUE *nova_fila = criar_fila();
    FILE *arquivo_final;

    printf("Entre com o nome do arquivo que deseja compactar: ");
    scanf("%s", nomedoarquivo);
    FILE *arquivo = fopen(nomedoarquivo, "rb");
    if (arquivo == NULL)
    {
        printf("File not found...\n");
        exit(1);
    }

    printf("\nIniciando a compactação!\n");
    nova_hash = inicializar_hash(nova_hash); 
    nova_hash = gerar_frequencias(nova_hash, arquivo); 
    rewind(arquivo);

    nova_fila = gerar_fila_de_prioridade(nova_hash, nova_fila); 

    raiz = criar_arvore_de_huffman(nova_fila);
    criar_caminho(raiz, nova_hash, caminho_da_arvore, 0);

    lixo = tamanho_do_lixo(nova_hash);
    tam_arvore = tamanho_da_arvore(raiz);

    criar_arquivo(nomedoarquivo);
    arquivo_final = fopen(nomedoarquivo, "wb");

    lixo = lixo << 13;
    final = lixo | tam_arvore;


    byte = final >> 8;
    fprintf(arquivo_final, "%c", byte);

    byte = final; 
    fprintf(arquivo_final, "%c", byte);

    imprimir_arvore_no_arquivo(raiz, arquivo_final);

    fseek(arquivo_final, (2 + tam_arvore), SEEK_SET);

    compactando_arquivo(arquivo, arquivo_final, nova_hash);

    return;   
}