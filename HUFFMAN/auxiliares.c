void printTree(NODE *root) //printar a árvore
{
    if (root == NULL)
    {
        return;
    }
    else
    {
        unsigned char *aux = (unsigned char *)malloc(sizeof(unsigned char));
        aux = root->item;
        printTree(root->left);
        printTree(root->right);
        printf("ITEM: %c - FREQ: %d\n", *aux, root->frequency);
    }
}

void add_way(HASH *table, void *item, char *bits, int n)
{
    int aux = *(unsigned char *)item;
    int i;
    ELEMENT *new_element = createElement();
    new_element->freq = n;
    for (i = 0; i <= n; i++)
    {
        new_element->way[i] = bits[i];
    }
    new_element->way[i] = '\0';
    table->table[aux] = new_element;
}
