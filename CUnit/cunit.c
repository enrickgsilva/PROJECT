#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include "estruturas.c"

void teste_criar_no() {
    NODE *new = (NODE *) malloc(sizeof(NODE));
    new->frequencia = 10;
    new->item = (unsigned char *) 'x';
    new->proximo = NULL;
    new->esquerda = NULL;
    new->direita = NULL;
    NODE *test = criar_no((unsigned char *) 'x', 10);

    CU_ASSERT(test != NULL);
    CU_ASSERT_EQUAL(new->frequencia, test->frequencia);
    CU_ASSERT_EQUAL(new->item, test->item);
    CU_ASSERT_PTR_EQUAL(new->proximo, test->proximo);
    CU_ASSERT_PTR_EQUAL(new->esquerda, test->esquerda);
    CU_ASSERT_PTR_EQUAL(new->direita, test->direita);
}

void teste_enfileirar() {
    QUEUE *pq = criar_fila();
    int a = 1, b = 2, c = 3; 
    enfileirar(pq, &a, 10);
    enfileirar(pq, &b, 15);
    enfileirar(pq, &c, 5);
    
	NODE *current = pq->head;
	CU_ASSERT_EQUAL(*((int*)current->item), 3);
	current = current->proximo;
	CU_ASSERT_EQUAL(*((int*)current->item), 1);
	current = current->proximo;
	CU_ASSERT_EQUAL(*((int*)current->item), 2);
}

void teste_desenfilerar() {
    QUEUE *pq = criar_fila();
    int a = 1, b = 2, c = 3; 
    enfileirar(pq, &a, 10);
    enfileirar(pq, &b, 15);
    enfileirar(pq, &c, 5);
    
	NODE *current = desenfileirar(pq);
	CU_ASSERT_EQUAL(*((int*)current->item), 3);
}

void teste_bit_esta_setado() {
    unsigned char aux = 157;

    CU_ASSERT(!bit_esta_setado(aux, 5));
    CU_ASSERT(bit_esta_setado(aux, 4));
}

void teste_fila_vazia() {
    QUEUE *queue = NULL;

    CU_ASSERT(queue == NULL);

}

int main() {

    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }
    CU_basic_set_mode(CU_BRM_VERBOSE);

    CU_pSuite pSuite = NULL, pSuite2 = NULL;
    pSuite = CU_add_suite("TEST", 0, 0);

    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (NULL == CU_add_test(pSuite, "test - 1", teste_criar_no)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (NULL == CU_add_test(pSuite, "test - 2", teste_enfileirar)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (NULL == CU_add_test(pSuite, "test - 3", teste_desenfilerar)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (NULL == CU_add_test(pSuite, "test - 4", teste_bit_esta_setado)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (NULL == CU_add_test(pSuite, "test - 5", teste_fila_vazia)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    return CU_get_error();
}
