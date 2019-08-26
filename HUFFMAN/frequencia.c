#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 256

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
    int frequencia;
    char string[50];
}ELEMENT;

typedef struct hash {
    ELEMENT *table[TABLE_SIZE];
}HASH;

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
    new_element->frequencia = 0;
    return new_element;
}

HASH* createHash() {
    HASH *new_hash = (HASH *)malloc(sizeof(HASH));
    int i;
    for(i = 0; i < 256; i++) {
        new_hash->table[i] = NULL;
    }
    return new_hash;
}

HASH* initHash(HASH *h) {
    for(int i = 0; i < TABLE_SIZE; i++) {
        h->table[i] = createElement();
    }
    return h;
}

HASH* frequency(HASH *h, FILE *file) {
    BYTE index;
    while (!feof(file)) {
        index = fgetc(file);
        h->table[(int)index]->frequencia += 1;
    }
    return h;
}

void add_way(HASH *table, void *item, char *bits, int n) {
    int aux = *(unsigned char *)item;
    ELEMENT *new_element = createElement();
    new_element->frequencia = n;
    for(int i = 0; i < n; i++) {
        new_element->string[i] = bits[i];
    }
    table->table[aux] = new_element;
}

void printHash(HASH *hash) {
    int i, j;
    for (i = 0; i < 256; i++) {
        j = hash->table[i]->frequencia;
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

void enqueue_nodes(QUEUE *queue, NODE *head) {
    if(emptyQueue(queue) || head->frequency <= queue->head->frequency) {
        head->next = queue->head;
        queue->head = head;
    } else {
        NODE *current = queue->head;
        while(current->next != NULL && current->next->frequency < head->frequency) {
            current = current->next;
        }
        head->next = queue->head;
        queue->head = head;
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
        if(h->table[i]->frequencia > 0) {
            unsigned char *aux = (unsigned char *)malloc(sizeof(unsigned char));
            *aux = i;
            enqueue(queue, aux, h->table[i]->frequencia);
        }
    }
    return queue;
}

NODE* mergeNodes(QUEUE *queue) {
    if (queue->head->next != NULL) {
        unsigned char *aux = (unsigned char *)malloc(sizeof(unsigned char));
        *aux = '*';
        NODE *new_node = (NODE *)malloc(sizeof(NODE));
        new_node->left = dequeue(queue);
        new_node->right = dequeue(queue);
        new_node->item = aux;
        new_node->frequency = new_node->left->frequency + new_node->right->frequency;
        enqueue_nodes(queue, new_node);
        mergeNodes(queue);
    } else {
        return queue->head;
    }
}

void create_huffman_three(NODE *root, HASH* table, char *way, int count) {
    if(root->left == NULL && root->right == NULL) {
        add_way(table, root->item, way, count);
    } else {
        way[count] = '0';
        create_huffman_three(root->left, table, way, count);
        way[count] = '1';
        create_huffman_three(root->right, table, way, count);
    }
}

int main() {
    char filename[50];
    char huffman_path[TABLE_SIZE];

    printf("Enter to the filename: ");
    scanf("%s", filename);

    NODE *root = NULL;
    HASH *new_hash = createHash();
    QUEUE *new_queue = createQueue();

    FILE *file = fopen(filename, "rb");

    FILE *final_file;
    printf("Saving frequencies...\n");
    printf("Initializing Hash...\n");
    new_hash = initHash(new_hash); //start hash with values of ASCII TABLE    
    printf("Add frequency and characteres on Hash Table...\n");
    new_hash = frequency(new_hash, file); //add characteres and frequencies on hash table
    printf("Creating Priority Queue...\n");
    new_queue = generateFreqeucencyQueue(new_hash, new_queue); //generate frequency priority queue
    printf("Done!\n\n");


    printf("Merge all nodes on tree..\n");
    root = mergeNodes(new_queue);
    printf("Creating a Huffman tree..\n");
    create_huffman_three(root, new_hash, huffman_path, 0);
    printf("Ok!\n\n");
    
    fclose(file);
    return 0;
}
