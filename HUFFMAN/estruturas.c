#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 256

typedef struct ELEMENT {
    int frequencia;
    char string[50];
}element;

typedef struct HASH {
    element *table[TABLE_SIZE];
}hash;

typedef unsigned char BYTE;

element *createelement() {
    element *new_element = (element *)malloc(sizeof(element));
    new_element->frequencia = 0;
    return new_element;
}

hash *createhash() {
    hash *new_hash = (hash *)malloc(sizeof(hash));
    int i;
    for(i = 0; i < 256; i++) {
        new_hash->table[i] = NULL;
    }
    return new_hash;
}

hash* initHash(hash* h) {
    for (int i = 1; i < TABLE_SIZE; i++) {
        h->table[i] = createelement();
    }
    return h;
}

hash* frequencia(hash* h, FILE* file) {
    BYTE index;
    while(!feof(file)) {
        index = fgetc(file);
        h->table[(int)index]->frequencia += 1;
    }
    return h;
}

void print_hash(hash *hash) {
    int i, j;
    for (i = 1; i < 256; i++) {
        j = hash->table[i]->frequencia;
        if (j > 0)
            printf("%c|%d -> %d \n", (char) i, i, j);
    }
}
/*
int main() {
    FILE* file = fopen("teste.txt", "rb");
    hash *new_hash = createhash();
    new_hash = initHash(new_hash);
    new_hash = frequencia(new_hash, file);
    print_hash(new_hash);
}
*/