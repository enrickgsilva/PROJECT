#ifndef COMPACTAR_H
#define COMPACTAR_H

#include "estruturas.h"

void createfile(char *nomedoarquivo);

void compactar_arquivo(FILE *arquivo, FILE *arquivo_final, HASH *hash);

void compactar();

#endif