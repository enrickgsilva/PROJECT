#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 256

typedef struct node
{
    int frequency;
    void *item;
    struct node *next;
    struct node *right;
    struct node *left;
} NODE;

typedef struct queue {
    NODE *head;
} QUEUE;

QUEUE* createqueue() {
    QUEUE *new_queue = (QUEUE *)malloc(sizeof(QUEUE));
    new_queue->head = NULL;
    return new_queue;
}

NODE* createnode(void *item) {
    NODE *new_node = (NODE *) malloc(sizeof(NODE));
    new_node->frequency = 0;
    new_node->item = item;
    new_node->next = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void enqueue(QUEUE *queue, void *item) {
    NODE *new_node = createnode(item);
    new_node->next = queue->head;
    queue->head = new_node;
}

NODE* dequeue(QUEUE *queue) {
    if(emptyQueue(queue)) {
        printf("Priority Queue Underflow\n");
    } else {
        NODE *new_node = queue->head;
        queue->head = queue->head->next;
        new_node->next = NULL;
        return new_node;
    }
}

int main() {
    int i = 121;
    char b = (char) i;
    QUEUE *new_queue = createqueue();
    enqueue(new_queue, &b);
    char *c = new_queue->head->item;
    printf("%c\n", *c);
    return 0;
}