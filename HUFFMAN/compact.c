#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 256
#define SLASH 92

typedef unsigned char BYTE;

typedef struct node {
    int frequency;
    void *item;
    struct node *next;
    struct node *right;
    struct node *left;
}NODE;

typedef struct queue {
    NODE *head;
}QUEUE;

typedef struct element {
    int freq;
    char way[50];
}ELEMENT;

typedef struct hash {
    ELEMENT *table[TABLE_SIZE];
}HASH;

unsigned char setBit(unsigned char c, int i) {
    unsigned char mask = 1 << i;
    return mask | c;
}

int verify(void *item1, unsigned char item2) {
    return (*((unsigned char *)item1) == item2);
}

NODE* createNode(void *item, int frequency) {
    NODE *new_node = (NODE *)malloc(sizeof(NODE));
    new_node->item = item;
    new_node->frequency = frequency;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

ELEMENT* createElement() {
    ELEMENT *new_element = (ELEMENT *)malloc(sizeof(ELEMENT));
    new_element->freq = 0;
    return new_element;
}

HASH* createHash() {
    HASH *new_hash = (HASH *)malloc(sizeof(HASH));
    int i;
    for(i = 0; i <= TABLE_SIZE; i++) {
        new_hash->table[i] = NULL;
    }
    return new_hash;
}

HASH* initHash(HASH *hash) {
    for(int i = 0; i < TABLE_SIZE; i++) {
        hash->table[i] = createElement();
    }
    return hash;
}

HASH* frequency(HASH *hash, FILE *file) {
    unsigned char index;
    while (!feof(file)) {
        index = fgetc(file);
        hash->table[(int)index]->freq++;
    }
    return hash;
}

void addWay(HASH *table, void *item, char *bits, int n) {
    int aux = *(unsigned char *)item;
    int i = 0;
    while(bits[i] != '\0') {
        table->table[aux]->way[i] = bits[i];
        i++;
    }
    table->table[aux]->way[i] = '\0';
}

void printHash(HASH *hash) {
    int i, j;
    for (i = 0; i < TABLE_SIZE; i++) {
        j = hash->table[i]->freq;
        if (j > 0)
            printf("%c|%d -> %d \n", (char)i, i, j);
    }
}

short emptyQueue(QUEUE *priority) {
    if(priority->head == NULL) {
        return 1;
    } else {
        return 0;
    }
}

QUEUE* createQueue() {
    QUEUE *new_queue = (QUEUE *) malloc(sizeof(QUEUE));
    new_queue->head = NULL;
    return new_queue;
}

void enqueue(QUEUE *queue, void *item, int frequency) {
    NODE *new_node = createNode(item, frequency);
    if(emptyQueue(queue) || frequency <= queue->head->frequency) {
        new_node->next = queue->head;
        queue->head = new_node;
    } else {
        NODE *current = queue->head; 
        while(current->next != NULL && current->next->frequency < frequency) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

void enqueueNodes(QUEUE *queue, NODE *head) {
    if(emptyQueue(queue) || head->frequency <= queue->head->frequency) {
        head->next = queue->head;
        queue->head = head;
    } else {
        NODE *current = queue->head;
        while(current->next != NULL && current->next->frequency < head->frequency) {
            current = current->next;
        }
        head->next = current->next;
        current->next = head;
    }
}

NODE *dequeue(QUEUE *queue) {
    if(emptyQueue(queue)) {
        printf("Priority Queue Underflow\n");
    } else {
        NODE *new_node = queue->head;
        queue->head = queue->head->next;
        new_node->next = NULL;
        return new_node;
    }
}

QUEUE* generateFreqeucencyQueue(HASH* h, QUEUE* queue){
    for(int i = 0; i < TABLE_SIZE; i++) {
        if(h->table[i]->freq > 0) {
            unsigned char *aux = (unsigned char *)malloc(sizeof(unsigned char));
            *aux = i;
            enqueue(queue, aux, h->table[i]->freq);
        }
    }
    return queue;
}

short isLeaf(NODE *root) {
    if(root->left == NULL && root->right == NULL) {
        return 1;
    }
    return 0;
}

NODE* mergeNodes(QUEUE *queue) {
    if (queue->head->next != NULL) {
        unsigned char *aux = (unsigned char *)malloc(sizeof(unsigned char));
        *aux = '*';
        NODE *new_node = (NODE *)malloc(sizeof(NODE));
        new_node->item = aux;
        new_node->left = dequeue(queue);
        new_node->right = dequeue(queue);
        new_node->frequency = new_node->left->frequency + new_node->right->frequency;
        enqueueNodes(queue, new_node);
        mergeNodes(queue);
    } else {
        return queue->head;
    }
}

void createHuffmanTree(NODE *root, HASH* table, char *way, int count) {
    if(isLeaf(root)) {
        way[count] = '\0';
        addWay(table, root->item, way, count);
    } else {
        way[count] = '0';
        createHuffmanTree(root->left, table, way, count+1);
        way[count] = '1';
        createHuffmanTree(root->right, table, way, count+1);
    }
}

int getTrashSize(HASH *hash) {
    int i, trash = 0;
    int bit, freq;
    for(i = 0; i < TABLE_SIZE; i++) {
        if(hash->table[i]->freq > 0) {
            bit = strlen(hash->table[i]->way);
            freq = hash->table[i]->freq;
            trash += bit * freq;
        }
    }
    trash = 8 - (trash % 8);
    return trash;
}

int getSizeTree(NODE *root) {
    if(isLeaf(root)) {
        if(verify(root->item, (char)SLASH) || verify(root->item, '*')) {
            return 2;
        } else {
            return 1;
        }
    } else {
        return 1 + getSizeTree(root->left) + getSizeTree(root->right);
    }
}

int main() {
    char filename[50];
    char huffmanPath[TABLE_SIZE];
    int count = 0;
    short trash, sizeTree, final;
    unsigned char current_byte, compact = 0;

    NODE *root = NULL;
    HASH *new_hash = createHash();
    QUEUE *new_queue = createQueue();
    FILE *final_file;

    printf("Enter to the filename: ");
    scanf("%s", filename);
    FILE *file = fopen(filename, "rb");
    if(file == NULL) {
        printf("File not found...\n");
        exit(1);
    }
    
    printf("\nSave frequencies on Hash Table...\n");
    printf("Initializing Hash...\n");
    new_hash = initHash(new_hash); //start hash with values of ASCII TABLE   
    printf("Add frequency characteres on Hash Table...\n");
    new_hash = frequency(new_hash, file); //add characteres and frequencies on hash table
    if(new_hash->table[255]->freq > 0)
        new_hash->table[255]->freq--; //remove charactere 255 read on fgetc
    printf("Creating Priority Queue...\n");
    new_queue = generateFreqeucencyQueue(new_hash, new_queue); //generate frequency priority queue
    printf("DONE!\n\n");

    printf("Merge all nodes on tree..\n");
    root = mergeNodes(new_queue);
    printf("Creating a Huffman tree..\n");
    createHuffmanTree(root, new_hash, huffmanPath, 0);
    printf("DONE!\n\n");
    
    printf("Get size trash...\n");
    trash = getTrashSize(new_hash);
    sizeTree = getSizeTree(root);
    printf("TREE SIZE: %d\n", sizeTree);


    fclose(file);
    return 0;
}
