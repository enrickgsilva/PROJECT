#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_DA_TABELA 256
#define BARRA 92 //caractere de barra invertida da tabela ASCII

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

/*Verifica se os bits estão setados*/
int bit_esta_setado(unsigned char c, int i);

/*Seta os bits*/
unsigned char setar_bit(unsigned char c, int i);

/*Verifica se o nó é um nó pai*/
int verificar_no(void *item1, unsigned char item2);

/*Cria um nó da árvore*/
NODE *criar_no(void *item, int frequencia);

/*Cria um elemento*/
ELEMENT *criar_elemento();

/*Cria uma hash do tipo elemento*/
HASH *criar_hash();

/*Inicializa a Hash com os ELEMENTOS*/
HASH *inicializar_hash(HASH *hash);

/*Pega as frequencias dos caracteres do arquivo e adiciona na Hash*/
HASH *gerar_frequencias(HASH *hash, FILE *arquivo);

/*Adiciona o caminho da nova codificação na tabela Hash*/
void adicionar_caminho(HASH *hash, void *item, char *bits); 

/*Verifica se a fila está vazia*/
short fila_vazia(QUEUE *fila);

/*Cria uma nova fila*/
QUEUE *criar_fila(); 

/*Enfileira os elementos em ordem crescente*/
void enfileirar(QUEUE *fila, void *item, int frequencia);

/*Enfileira os nós em ordem crescente*/
void enfileirar_nos(QUEUE *fila, NODE *head);

/*Desenfileira um elemento da fila*/
NODE *desenfileirar(QUEUE *fila);

/*Controla e envia os itens da tabela Hash para serem enfileirados*/
QUEUE *gerar_fila_de_prioridade(HASH *hash, QUEUE *fila); 

/*Verifica se o nó é uma folha da árvore*/
short folha_da_arvore(NODE *raiz);

/*Cria a árvore de Huffman, somando as frequencias dos filhos e adicionando (*) no nó pai*/
NODE *criar_arvore_de_huffman(QUEUE *fila); 

/*Cria o a nova codificação(caminho) ao percorrer da raiz até a folha*/
void criar_caminho(NODE *raiz, HASH *hash, char *caminho, int contador); 

/*É responsável por retornar o tamanho do lixo*/
int tamanho_do_lixo(HASH *hash); 

/*Função responsável por retornar o tamanho da árvore*/
int tamanho_da_arvore(NODE *raiz); 

/*Função responsável por escrever a árvore em pré-ordem no arquivo*/
void imprimir_arvore_no_arquivo(NODE *raiz, FILE *arquivo_para_compactar);

#endif