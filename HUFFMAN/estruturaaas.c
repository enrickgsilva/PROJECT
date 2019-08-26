element *createelement()
{
    element *new_element = (element *)malloc(sizeof(element));
    new_element->n = 0;
    return new_element;
}

hash *createhash()
{
    hash *new_hash = (hash *)malloc(sizeof(hash));
    int i;
    for (i = 0; i < 256; i++)
    {
        new_hash->table[i] = NULL;
    }
    return new_hash;
}

void add_hash(hash *hash, void *item, char *bit, int n)
{
    int h = *(unsigned char *)item;
    element *new_element = createelement();
    new_element->n = n;
    int i;
    for (i = 0; i < n; i++)
    {
        new_element->string[i] = bit[i];
    }
    hash->table[h] = new_element;
}

void print_hash(hash *hash)
{
    int i, j;
    for (i = 0; i < 256; i++)
    {
        if (hash->table[i] != NULL)
        {
            printf("%d -> ", i);
            for (j = 0; j < hash->table[i]->n; j++)
            {
                printf("%c", hash->table[i]->string[j]);
            }
            printf(" [%d]\n", hash->table[i]->n);
        }
    }
}
