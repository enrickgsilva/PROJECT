#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 256

typedef unsigned char BYTE;

typedef struct NODE
{
    int frequency;
    void *item;
    struct NODE *next;
    struct NODE *right;
    struct NODE *left;
} node;

typedef struct QUEUE
{
    node *head;
} queue;

typedef struct ELEMENT
{
    int frequencia;
    char string[50];
} element;

typedef struct HASH
{
    element *table[TABLE_SIZE];
} hash;

node *createNode(void *item, int frequency)
{
    node *new_node = (node *)malloc(sizeof(node));
    new_node->item = item;
    new_node->frequency = frequency;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

element *createElement()
{
    element *new_element = (element *)malloc(sizeof(element));
    new_element->frequencia = 0;
    return new_element;
}

hash *createHash()
{
    hash *new_hash = (hash *)malloc(sizeof(hash));
    int i;
    for (i = 0; i < 256; i++)
    {
        new_hash->table[i] = NULL;
    }
    return new_hash;
}

hash *initHash(hash *h)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        h->table[i] = createElement();
    }
    return h;
}

hash *frequency(hash *h, FILE *file)
{
    BYTE index;
    while (!feof(file))
    {
        index = fgetc(file);
        h->table[(int)index]->frequencia += 1;
    }
    return h;
}

void printHash(hash *hash)
{
    int i, j;
    for (i = 0; i < 256; i++)
    {
        j = hash->table[i]->frequencia;
        if (j > 0)
            printf("%c|%d -> %d \n", (char)i, i, j);
    }
}

short emptyQueue(queue *priority)
{
    if (priority->head == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

queue *createQueue()
{
    queue *new_queue = (queue *)malloc(sizeof(queue));
    new_queue->head = NULL;
    return new_queue;
}

void enqueue(queue *q, void *item, int frequency)
{
    node *new_node = createNode(item, frequency);
    if (emptyQueue(q) || frequency <= q->head->frequency)
    {
        new_node->next = q->head;
        q->head = new_node;
    }
    else
    {
        node *current = q->head;
        while (current != NULL && current->next->frequency < frequency)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

node *dequeue(queue *queue)
{
    if (emptyQueue(queue))
    {
        printf("Priority Queue Underflow\n");
    }
    else
    {
        node *new_node = queue->head;
        queue->head = queue->head->next;
        new_node->next = NULL;
        return new_node;
    }
}

queue *generateFreqeucencyQueue(hash *h, queue *q)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (h->table[i]->frequencia > 0)
        {
            unsigned char *aux = (unsigned char *)malloc(sizeof(unsigned char));
            *aux = i;
            enqueue(q, aux, h->table[i]->frequencia);
        }
    }
    return q;
}

int main()
{
    char filename[50];
    printf("Enter to the filename: ");
    scanf("%s", filename);

    hash *new_hash = createHash();
    queue *new_queue = createQueue();

    FILE *file = fopen(filename, "rb");

    printf("Saving frequencies...\n");
    new_hash = initHash(new_hash);
    new_hash = frequency(new_hash, file);
    printHash(new_hash);

    //new_queue = generateFreqeucencyQueue(new_hash, new_queue);

    queue *nova = createQueue();
    nova = new_queue;

    /*
    while(!emptyQueue(nova)) {
        unsigned char *c = new_queue->head->item;
        printf("ITEM: %c | FREQUENCY: %d\n", *c, new_queue->head->frequency);
        nova->head = nova->head->next;
    }
*/

    QUEUE *nova = createQueue();
    nova = new_queue;

    while (!emptyQueue(nova))
    {
        unsigned char *c = new_queue->head->item;
        printf("ITEM: %c | FREQUENCY: %d\n", *c, new_queue->head->frequency);
        nova->head = nova->head->next;
    }

    fclose(file);
    return 0;
}
