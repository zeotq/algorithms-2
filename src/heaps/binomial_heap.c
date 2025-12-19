/**
 * binomial_heap.c
 *
 * Теория и реализация: Биномиальная приоритетная очередь
 *
 * -----------------------------------------------------------------------------
 * 1. Биномиальное дерево
 * 2. Ранг биномиального дерева и число элементов
 * 3. Биномиальная куча
 * 4. Основные операции
 * 5. Вычислительные сложности
 * 6. Реализация на C
 *
 * -----------------------------------------------------------------------------
 *
 * 1) Биномиальное дерево
 *
 *    Биномиальное дерево B_k определяется рекурсивно:
 *
 *      B_0 — один узел
 *      B_k — результат слияния двух деревьев B_{k-1}
 *
 *    Корень одного дерева становится левым ребёнком корня другого.
 *
 * -----------------------------------------------------------------------------
 *
 * 2) Ранг биномиального дерева
 *
 *    Ранг дерева B_k равен k.
 *
 *    Свойства:
 *      - высота дерева = k
 *      - число узлов = 2^k
 *      - у корня k детей
 *
 * -----------------------------------------------------------------------------
 *
 * 3) Биномиальная куча
 *
 *    Биномиальная куча — это набор биномиальных деревьев,
 *    в котором:
 *      - не более одного дерева каждого ранга
 *      - выполнено heap-свойство (min-heap)
 *
 *    Структура хранения:
 *      - корни деревьев связаны в список
 *
 * -----------------------------------------------------------------------------
 *
 * 4) Операции
 *
 *    insert:
 *      - создать B_0
 *      - слить с кучей
 *
 *    find-min:
 *      - минимум среди корней
 *
 *    delete-min:
 *      - удалить дерево с минимальным корнем
 *      - слить его детей с кучей
 *
 *    merge:
 *      - аналог сложения бинарных чисел
 *
 * -----------------------------------------------------------------------------
 *
 * 5) Сложности
 *
 *    insert:       O(log n)
 *    find-min:     O(log n)
 *    delete-min:   O(log n)
 *    merge:        O(log n)
 *
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

/* Узел биномиального дерева */
typedef struct BinomialNode {
    int key;
    int degree;
    struct BinomialNode *parent;
    struct BinomialNode *child;
    struct BinomialNode *sibling;
} BinomialNode;

/* Биномиальная куча */
typedef struct {
    BinomialNode *head;
} BinomialHeap;

/* Создание узла */
static BinomialNode *binomial_new_node(int key) {
    BinomialNode *n = malloc(sizeof(BinomialNode));
    if (!n) { perror("malloc"); exit(EXIT_FAILURE); }
    n->key = key;
    n->degree = 0;
    n->parent = n->child = n->sibling = NULL;
    return n;
}

/* Создание пустой кучи */
BinomialHeap *binomial_create(void) {
    BinomialHeap *h = malloc(sizeof(BinomialHeap));
    h->head = NULL;
    return h;
}

/* Связывание двух деревьев одинакового ранга */
static BinomialNode *binomial_link(BinomialNode *a, BinomialNode *b) {
    if (b->key < a->key) {
        BinomialNode *t = a; a = b; b = t;
    }
    b->parent = a;
    b->sibling = a->child;
    a->child = b;
    a->degree++;
    return a;
}

/* Слияние списков корней */
static BinomialNode *merge_roots(BinomialNode *a, BinomialNode *b) {
    if (!a) return b;
    if (!b) return a;
    BinomialNode *head, **pos = &head;
    while (a && b) {
        if (a->degree <= b->degree) {
            *pos = a; a = a->sibling;
        } else {
            *pos = b; b = b->sibling;
        }
        pos = &(*pos)->sibling;
    }
    *pos = a ? a : b;
    return head;
}

/* Основное слияние куч */
BinomialHeap *binomial_merge(BinomialHeap *h1, BinomialHeap *h2) {
    BinomialHeap *h = binomial_create();
    h->head = merge_roots(h1->head, h2->head);

    if (!h->head) return h;

    BinomialNode *prev = NULL;
    BinomialNode *curr = h->head;
    BinomialNode *next = curr->sibling;

    while (next) {
        if (curr->degree != next->degree ||
            (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else {
            curr = binomial_link(curr, next);
            if (prev)
                prev->sibling = curr;
            else
                h->head = curr;
        }
        next = curr->sibling;
    }
    return h;
}

/* Вставка */
void binomial_insert(BinomialHeap **heap, int key) {
    BinomialHeap *tmp = binomial_create();
    tmp->head = binomial_new_node(key);
    *heap = binomial_merge(*heap, tmp);
}

/* Поиск минимума */
int binomial_find_min(BinomialHeap *h) {
    assert(h->head);
    int min = INT_MAX;
    for (BinomialNode *n = h->head; n; n = n->sibling)
        if (n->key < min) min = n->key;
    return min;
}

/* Удаление минимума */
void binomial_delete_min(BinomialHeap **heap) {
    BinomialNode *prev = NULL, *min_prev = NULL;
    BinomialNode *min_node = (*heap)->head;

    for (BinomialNode *n = (*heap)->head; n->sibling; n = n->sibling) {
        if (n->sibling->key < min_node->key) {
            min_node = n->sibling;
            min_prev = n;
        }
    }

    if (min_prev)
        min_prev->sibling = min_node->sibling;
    else
        (*heap)->head = min_node->sibling;

    /* Реверсируем детей */
    BinomialNode *child = min_node->child;
    BinomialNode *rev = NULL;
    while (child) {
        BinomialNode *next = child->sibling;
        child->sibling = rev;
        child->parent = NULL;
        rev = child;
        child = next;
    }

    BinomialHeap *tmp = binomial_create();
    tmp->head = rev;
    *heap = binomial_merge(*heap, tmp);
    free(min_node);
}
