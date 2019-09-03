#include "compactar.h"

int bit_esta_setado(unsigned char c, int i)
{
    unsigned char mascara = 1 << i;
    return mascara & c;
}

unsigned char setar_bit(unsigned char c, int i)
{
    unsigned char mascara = 1 << i;
    return mascara | c;
}

int verificar_no(void *item1, unsigned char item2)
{
    return (*((unsigned char *)item1) == item2);
}

NODE *criar_no(void *item, int frequencia)
{
    NODE *novo_no = (NODE *)malloc(sizeof(NODE));
    novo_no->item = item;
    novo_no->frequencia = frequencia;
    novo_no->proximo = NULL;
    novo_no->esquerda = NULL;
    novo_no->direita = NULL;
    return novo_no;
}

ELEMENT *criar_elemento()
{
    ELEMENT *novo_elemento = (ELEMENT *)malloc(sizeof(ELEMENT));
    novo_elemento->freq = 0;
    return novo_elemento;
}

HASH *criar_hash()
{
    HASH *nova_hash = (HASH *)malloc(sizeof(HASH));
    int i;
    for (i = 0; i <= TAMANHO_DA_TABELA; i++)
    {
        nova_hash->tabela[i] = NULL;
    }
    return nova_hash;
}

HASH *inicializar_hash(HASH *hash)
{
    for (int i = 0; i < TAMANHO_DA_TABELA; i++)
    {
        hash->tabela[i] = criar_elemento();
    }
    return hash;
}

HASH *gerar_frequencias(HASH *hash, FILE *arquivo)
{
    int indice;
    while ((indice = fgetc(arquivo)) != EOF)
    {
        hash->tabela[indice]->freq++;
    }
    return hash;
}

void adicionar_caminho(HASH *hash, void *item, char *bits)
{
    int aux = *(unsigned char *)item;
    int i = 0;
    while (bits[i] != '\0')
    {
        hash->tabela[aux]->caminho[i] = bits[i];
        i++;
    }
    hash->tabela[aux]->caminho[i] = '\0';
}

short fila_vazia(QUEUE *fila)
{
    if (fila->head == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

QUEUE *criar_fila()
{
    QUEUE *nova_fila = (QUEUE *)malloc(sizeof(QUEUE));
    nova_fila->head = NULL;
    return nova_fila;
}

void enfileirar(QUEUE *fila, void *item, int frequencia)
{
    NODE *novo_no = criar_no(item, frequencia);
    if (fila_vazia(fila) || frequencia <= fila->head->frequencia)
    {
        novo_no->proximo = fila->head;
        fila->head = novo_no;
    }
    else
    {
        NODE *atual = fila->head;
        while (atual->proximo != NULL && atual->proximo->frequencia < frequencia)
        {
            atual = atual->proximo;
        }
        novo_no->proximo = atual->proximo;
        atual->proximo = novo_no;
    }
}

void enfileirar_nos(QUEUE *fila, NODE *head)
{
    if (fila_vazia(fila) || head->frequencia <= fila->head->frequencia)
    {
        head->proximo = fila->head;
        fila->head = head;
    }
    else
    {
        NODE *atual = fila->head;
        while (atual->proximo != NULL && atual->proximo->frequencia < head->frequencia)
        {
            atual = atual->proximo;
        }
        head->proximo = atual->proximo;
        atual->proximo = head;
    }
}

NODE *desenfileirar(QUEUE *fila)
{
    if (fila_vazia(fila))
    {
        printf("Priority Queue Underflow\n");
        return NULL;
    }
    else
    {
        NODE *novo_no = fila->head;
        fila->head = fila->head->proximo;
        novo_no->proximo = NULL;
        return novo_no;
    }
}

QUEUE *gerar_fila_de_prioridade(HASH *hash, QUEUE *fila)
{
    for (int i = 0; i < TAMANHO_DA_TABELA; i++)
    {
        if (hash->tabela[i]->freq > 0)
        {
            unsigned char *aux = (unsigned char *)malloc(sizeof(unsigned char));
            *aux = i;
            enfileirar(fila, aux, hash->tabela[i]->freq);
        }
    }
    return fila;
}

short folha_da_arvore(NODE *raiz)
{
    if (raiz->esquerda == NULL && raiz->direita == NULL)
    {
        return 1;
    }
    return 0;
}

NODE *criar_arvore_de_huffman(QUEUE *fila)
{
    if (fila->head->proximo != NULL)
    {
        unsigned char *aux = (unsigned char *)malloc(sizeof(unsigned char));
        *aux = '*';
        NODE *novo_no = (NODE *)malloc(sizeof(NODE));
        novo_no->item = aux;
        novo_no->esquerda = desenfileirar(fila);
        novo_no->direita = desenfileirar(fila);
        int freq_direita = novo_no->direita->frequencia;
        int freq_esquerda = novo_no->esquerda->frequencia;
        novo_no->frequencia = (freq_direita + freq_esquerda);
        enfileirar_nos(fila, novo_no);
        criar_arvore_de_huffman(fila);
    }
    else
    {
        return fila->head;
    }
}

void criar_caminho(NODE *raiz, HASH *hash, char *caminho, int contador)
{
    if (folha_da_arvore(raiz))
    {
        caminho[contador] = '\0';
        adicionar_caminho(hash, raiz->item, caminho);
    }
    else
    {
        caminho[contador] = '0';
        criar_caminho(raiz->esquerda, hash, caminho, contador + 1);
        caminho[contador] = '1';
        criar_caminho(raiz->direita, hash, caminho, contador + 1);
    }
}

int tamanho_do_lixo(HASH *hash)
{
    int i, lixo = 0;
    int bit, freq;
    for (i = 0; i < TAMANHO_DA_TABELA; i++)
    {
        if (hash->tabela[i]->freq > 0)
        {
            bit = strlen(hash->tabela[i]->caminho);
            freq = hash->tabela[i]->freq;
            lixo += (bit * freq);
        }
    }
    lixo = 8 - (lixo % 8);
    return lixo;
}

int tamanho_da_arvore(NODE *raiz)
{
    if (folha_da_arvore(raiz))
    {
        if (verificar_no(raiz->item, (char)BARRA) || verificar_no(raiz->item, '*'))
        {
            return 2;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1 + tamanho_da_arvore(raiz->esquerda) + tamanho_da_arvore(raiz->direita);
    }
}

void imprimir_arvore_no_arquivo(NODE *raiz, FILE *arquivo_para_compactar)
{
    if (raiz != NULL)
    {
        if (raiz->esquerda == NULL && raiz->direita == NULL && (verificar_no(raiz->item, '*') || verificar_no(raiz->item, (char)BARRA)))
        {
            fprintf(arquivo_para_compactar, "%c", (char)BARRA);
        }
        fprintf(arquivo_para_compactar, "%c", *(unsigned char *)raiz->item);
        imprimir_arvore_no_arquivo(raiz->esquerda, arquivo_para_compactar);
        imprimir_arvore_no_arquivo(raiz->direita, arquivo_para_compactar);
    }
}