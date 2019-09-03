#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "compactar.h"
#include "descompactar.h"


FILE *input = NULL;
//---- --- - - -- -
int opening()
{
    input = fopen("entrada", "rb");   
    if(input == NULL)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int closing()
{
    if(fclose(input))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}



// ---------------------

void test_frequency_saving()
{
    HASH *frequencies = gerar_frequencias(NULL, input);
    //input = "aaaaabbbbccccddeef" (a->[5], b->[4], c->[4], d->[2], e->[2], f->[1])
    int a = 'a', b = 'b', c = 'c', d = 'd', e = 'e', f = 'f';
    CU_ASSERT_EQUAL(frequencies->tabela[a]->freq, 5);
    CU_ASSERT_EQUAL(frequencies->tabela[b]->freq, 4);
    CU_ASSERT_EQUAL(frequencies->tabela[c]->freq, 4);
    CU_ASSERT_EQUAL(frequencies->tabela[d]->freq, 2);
    CU_ASSERT_EQUAL(frequencies->tabela[e]->freq, 2);
    CU_ASSERT_EQUAL(frequencies->tabela[f]->freq, 1);  
}
void test_gerar_fila_de_prioridade()
{
    QUEUE *queue1 = criar_fila();
    rewind(input);
    HASH *frequencies = gerar_frequencias(NULL, input);
    gerar_fila_de_prioridade(frequencies, queue1);
    //input = "aaaaabbbbccccddeef" (a->[5], b->[4], c->[4], d->[2], e->[2], f->[1])
    NODE *current = queue1->head->proximo;
    CU_ASSERT_EQUAL(*((int*)current->item), 'f');
    current = current->proximo;
    CU_ASSERT_EQUAL(*((int*)current->item), 'e');
    current = current->proximo;
    CU_ASSERT_EQUAL(*((int*)current->item), 'd');
    current = current->proximo;
    CU_ASSERT_EQUAL(*((int*)current->item), 'c');
    current = current->proximo;
    CU_ASSERT_EQUAL(*((int*)current->item), 'b');
    current = current->proximo;
    CU_ASSERT_EQUAL(*((int*)current->item), 'a');
}
void test_enqueue()
{
	QUEUE *queue1 = criar_fila();
	int a = 1, b = 2, c = 3, d = 4;
	//Priority of 1 is 100
	enfileirar(queue1, &a, 100);
	//Priority of 2 is 120
	enfileirar(queue1, &b, 120);
	//Priority of 3 is 50
	enfileirar(queue1, &c, 50);
	//Priority of 4 is 120
	enfileirar(queue1, &d, 120);


	NODE *current = queue1->head;
	CU_ASSERT_EQUAL(*((int*)current->item), 3);
	current = current->proximo;
	CU_ASSERT_EQUAL(*((int*)current->item), 1);
	current = current->proximo;
	CU_ASSERT_EQUAL(*((int*)current->item), 4);
	current = current->proximo;
	CU_ASSERT_EQUAL(*((int*)current->item), 2);
}

void test_dequeue()
{
	QUEUE *queue1 = criar_fila();
	int a = 1, b = 2, c = 3, d = 4;
	//Priority of 1 is 100
	enfileirar(queue1, &a, 100);
	//Priority of 2 is 120
	enfileirar(queue1, &b, 120);
	//Priority of 3 is 50
	enfileirar(queue1, &c, 50);
	//Priority of 4 is 120
	enfileirar(queue1, &d, 120);

	NODE *current = desenfileirar(queue1);
	CU_ASSERT_EQUAL(*((int*)current->item), 3);
	current = desenfileirar(queue1);
	CU_ASSERT_EQUAL(*((int*)current->item), 1);
	current = desenfileirar(queue1);
	CU_ASSERT_EQUAL(*((int*)current->item), 4);
	current = desenfileirar(queue1);
	CU_ASSERT_EQUAL(*((int*)current->item), 2);

	CU_ASSERT_EQUAL(fila_vazia(queue1), 1);
}

// --------- - - - - - -  TREE

// ************
NODE *create_test_tree()
{
	NODE *raiz = criar_no((unsigned char *)'*', 5);
	raiz->esquerda = criar_no((unsigned char *)'A', 6);
	raiz->direita = criar_no((unsigned char *)'B', 7);

	return raiz;
}

void writing_pre_order(NODE *root, char *string, int* count)
{
    if (root != NULL)
    {
        if (root->esquerda == NULL && root->direita == NULL && (verificar_no(root->item, '*') || verificar_no(root->item, '\\')))
        {
            string[*count] = '\\';
            *count += 1;
        }
        string[*count] = *(unsigned char *)root->item;
        *count += 1;

        writing_pre_order(root->esquerda, string, count);
        writing_pre_order(root->direita, string, count);
    }
}
// ****************
void test_tree()
{
	NODE *root = create_test_tree();

	CU_ASSERT_EQUAL(*((unsigned char*)root->item), '*');
	CU_ASSERT_EQUAL(*((unsigned char*)root->esquerda->item), 'A');
	CU_ASSERT_EQUAL(*((unsigned char*)root->direita->item), 'B');
	CU_ASSERT_EQUAL(tamanho_da_arvore(root), 3);
}

void test_criar_arvore_de_huffman()
{
    QUEUE* queue1 = criar_fila();
    HASH *frequencies = gerar_frequencias(NULL, input);
    gerar_fila_de_prioridade(frequencies, queue1);
    NODE *root = criar_arvore_de_huffman(queue1);

    CU_ASSERT_PTR_NULL(queue1->head->proximo);
    CU_ASSERT_EQUAL(*((unsigned char*)queue1->head->item), '*');
}

void test_tamanho_da_arvore()
{
    rewind(input);
    QUEUE* queue1 = criar_fila();
    HASH *frequencies = gerar_frequencias(NULL, input);
    gerar_fila_de_prioridade(frequencies, queue1);
    NODE *root = criar_arvore_de_huffman(queue1);

    CU_ASSERT_EQUAL(tamanho_da_arvore(root), 13);
}

void test_preorder_tree()
{
    rewind(input);
    QUEUE* queue1 = criar_fila();
    HASH *frequencies = gerar_frequencias(NULL, input);
    gerar_fila_de_prioridade(frequencies, queue1);
    NODE *root = criar_arvore_de_huffman(queue1);

    char p_order[15] = "**cb*a*d**fe";
    char w_p_order[15];
    int *i = (int*)malloc(sizeof(int));
    *i = 0;    
    writing_pre_order(root, w_p_order, i);
    
    CU_ASSERT_EQUAL(*i, 13);


}
//---------------- HASH -----------

void test_huff_code()
{
    rewind(input);
    char compacted_byte[50];
    HASH *hash_table1 = criar_hash();
    QUEUE* queue1 = criar_fila();
    HASH *frequencies = gerar_frequencias(NULL, input);
    gerar_fila_de_prioridade(frequencies, queue1);
    NODE *root = criar_arvore_de_huffman(queue1);
    criar_caminho(root, hash_table1, compacted_byte, 0);
    int a = 'a', b = 'b', c = 'c', d = 'd', e = 'e', f = 'f';

    CU_ASSERT_EQUAL(strlen(hash_table1->tabela[a]->caminho), 2);
    CU_ASSERT_STRING_EQUAL(hash_table1->tabela[a]->caminho, "10");
    CU_ASSERT_EQUAL(strlen(hash_table1->tabela[b]->caminho), 2);
    CU_ASSERT_STRING_EQUAL(hash_table1->tabela[b]->caminho, "01");
    CU_ASSERT_EQUAL(strlen(hash_table1->tabela[c]->caminho), 2);
    CU_ASSERT_STRING_EQUAL(hash_table1->tabela[c]->caminho, "00");
    CU_ASSERT_EQUAL(strlen(hash_table1->tabela[d]->caminho), 3);
    CU_ASSERT_STRING_EQUAL(hash_table1->tabela[d]->caminho, "110");
    CU_ASSERT_EQUAL(strlen(hash_table1->tabela[e]->caminho), 4);
    CU_ASSERT_STRING_EQUAL(hash_table1->tabela[e]->caminho, "1111");
    CU_ASSERT_EQUAL(strlen(hash_table1->tabela[f]->caminho), 5);
    CU_ASSERT_STRING_EQUAL(hash_table1->tabela[f]->caminho, "11101");
}

int main() {


   // Init the CUnit
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

    // FIRST SUITE
        CU_pSuite ps_queue = NULL;

        ps_queue = CU_add_suite("test_queue", opening, closing);

        if(ps_queue == NULL)
        {
          CU_cleanup_registry();
          return CU_get_error();
        }

        //Adding test to the suite
        if (NULL == CU_add_test(ps_queue, "test_enqueue", test_enqueue)) {
          CU_cleanup_registry();
          return CU_get_error();
        }

        if (NULL == CU_add_test(ps_queue, "test_dequeue", test_dequeue)) {
          CU_cleanup_registry();
          return CU_get_error();
        }
        if(NULL == CU_add_test(ps_queue, "test_frequency_saving", test_frequency_saving))
        {
            CU_cleanup_registry();
            return CU_get_error();
        }
        if(NULL == CU_add_test(ps_queue, "test_gerar_fila_de_prioridade", test_gerar_fila_de_prioridade))
        {
            CU_cleanup_registry();
            return CU_get_error();
        }

    // SECOND SUITE
        CU_pSuite ps_tree = NULL;

        ps_tree = CU_add_suite("test_tree", opening, closing);

        if(ps_tree == NULL)
        {
          CU_cleanup_registry();
          return CU_get_error();
        }

        //Adding test to the suite
        if (NULL == CU_add_test(ps_tree, "test_criar_arvore_de_huffman", test_criar_arvore_de_huffman))
        {
          CU_cleanup_registry();
          return CU_get_error();
        }
        if (NULL == CU_add_test(ps_tree, "test_tree", test_tree))
        {
          CU_cleanup_registry();
          return CU_get_error();
        }
        if (NULL == CU_add_test(ps_tree, "test_tamanho_da_arvore", test_tamanho_da_arvore))
        {
          CU_cleanup_registry();
          return CU_get_error();
        }
        if (NULL == CU_add_test(ps_tree, "test_preorder_tree", test_preorder_tree))
        {
          CU_cleanup_registry();
          return CU_get_error();
        }
    // THIRD SUITE
        CU_pSuite ps_hash = NULL;

        ps_hash = CU_add_suite("test_hash_table", opening, closing);

        if(ps_hash == NULL)
        {
          CU_cleanup_registry();
          return CU_get_error();
        }

        //Adding test to the suite
        
        if (NULL == CU_add_test(ps_hash, "test_huff_code", test_huff_code))
        {
          CU_cleanup_registry();
          return CU_get_error();
        }

   CU_basic_set_mode(CU_BRM_VERBOSE);
   

   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
