#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_DA_TABELA 256
#define BARRA 92

typedef unsigned char BYTE;

typedef struct node
{
    int frequencia;
    void *item;
    struct node *proximo;
    struct node *direita;
    struct node *esquerda;
} NODE;

typedef struct queue
{
    NODE *head;
} QUEUE;

typedef struct element
{
    long long int freq;
    char caminho[50];
} ELEMENT;

typedef struct hash
{
    ELEMENT *tabela[TAMANHO_DA_TABELA];
} HASH;

int bit_esta_setado(unsigned char c, int i);

unsigned char setar_bit(unsigned char c, int i);

int verificar_no(void *item1, unsigned char item2);

NODE *criar_no(void *item, int frequencia);

ELEMENT *criar_elemento();

HASH *criar_hash();

HASH *inicializar_hash(HASH *hash);

HASH *gerar_frequencias(HASH *hash, FILE *arquivo);

void adicionar_caminho(HASH *table, void *item, char *bits); 

void imprimir_hash(HASH *hash); 

short fila_vazia(QUEUE *fila);

QUEUE *criar_fila(); 

void enfileirar(QUEUE *fila, void *item, int frequencia);

void enfileirar_nos(QUEUE *fila, NODE *head);

NODE *desenfileirar(QUEUE *fila);

QUEUE *gerar_fila_de_prioridade(HASH *hash, QUEUE *fila); 

short folha_da_arvore(NODE *raiz);

NODE *criar_arvore_de_huffman(QUEUE *fila); 

void criar_caminho(NODE *raiz, HASH *hash, char *caminho, int contador); 

int tamanho_do_lixo(HASH *hash); 

int tamanho_da_arvore(NODE *raiz); 

void imprimir_caminho(HASH *hash); 

void imprimir_arvore(NODE *raiz); 

void imprimir_arvore_no_arquivo(NODE *raiz, FILE *arquivo_para_compactar);

#endif