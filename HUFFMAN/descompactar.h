#ifndef DESCOMPACTAR_H
#define DESCOMPACTAR_H

#include "estruturas.h"

void descompactar();

int descompressao(FILE *compactado, char *nome);

NODE *montar_arvore(FILE *compactado);

NODE *criar_no_arvore(unsigned char caractere);

void printar_byte(FILE *compactado, FILE *descompactado, NODE *raiz, short tamanho_arvore, short tamanho_lixo, int tamanho_arquivo);

#endif