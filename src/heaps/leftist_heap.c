/**
 * leftist_heap.c
 *
 * Реализация и теория: Левосторонняя приоритетная очередь (Leftist Heap)
 *
 * Содержание:
 * 1. Что такое приоритетная очередь
 * 2. Идея левосторонней кучи
 * 3. Null Path Length (NPL) и левостороннее свойство
 * 4. Базовая операция merge
 * 5. Операции: insert, find-min, delete-min
 * 6. Слияние с разрушением и без разрушения
 * 7. Вычислительные сложности
 * 8. Реализация на C
 * 9. Тестовый пример
 *
 * -----------------------------------------------------------------------------
 *
 * 1) Приоритетная очередь
 *
 *    Приоритетная очередь — абстрактная структура данных, поддерживающая:
 *      - вставку элемента
 *      - поиск элемента с наивысшим приоритетом (min / max)
 *      - удаление элемента с наивысшим приоритетом
 *
 *    В отличие от обычной очереди, порядок определяется приоритетом,
 *    а не временем добавления.
 *
 * -----------------------------------------------------------------------------
 *
 * 2) Левосторонняя приоритетная очередь (Leftist Heap)
 *
 *    Левосторонняя куча — это бинарное дерево, удовлетворяющее:
 *
 *      (1) Heap-свойству (обычно min-heap):
 *          ключ узла <= ключей его детей
 *
 *      (2) Левостороннему свойству:
 *          NPL(left) >= NPL(right)
 *
 *    Главная идея:
 *      - правые пути в дереве всегда короткие
 *      - это делает операцию merge эффективной
 *
 * -----------------------------------------------------------------------------
 *
 * 3) Null Path Length (NPL)
 *
 *    NPL(x) — длина кратчайшего пути от x до узла,
 *    имеющего хотя бы одного пустого потомка.
 *
 *    Формально:
 *      NPL(NULL) = -1
 *      NPL(x) = 1 + min(NPL(x->left), NPL(x->right))
 *
 *    В левосторонней куче:
 *      NPL(left) >= NPL(right)
 *
 * -----------------------------------------------------------------------------
 *
 * 4) Базовая операция merge
 *
 *    merge(h1, h2):
 *      - выбираем корень с меньшим ключом
 *      - рекурсивно сливаем его правое поддерево
 *      - проверяем левостороннее свойство
 *      - при необходимости меняем детей местами
 *
 *    Все остальные операции выражаются через merge.
 *
 * -----------------------------------------------------------------------------
 *
 * 5) Основные операции
 *
 *    insert(x):
 *      - создать одноэлементную кучу
 *      - merge(heap, new_node)
 *
 *    find-min:
 *      - корень кучи
 *
 *    delete-min:
 *      - удалить корень
 *      - merge(left_subtree, right_subtree)
 *
 * -----------------------------------------------------------------------------
 *
 * 6) Слияние с разрушением и без разрушения
 *
 *    - Разрушающее:
 *        использует существующие узлы
 *        O(log n)
 *
 *    - Неразрушающее:
 *        создаёт копии узлов
 *        O(n + m)
 *
 * -----------------------------------------------------------------------------
 *
 * 7) Вычислительные сложности
 *
 *    Пусть n — число элементов.
 *
 *    Время (амортизированно):
 *      - merge:        O(log n)
 *      - insert:       O(log n)
 *      - find-min:     O(1)
 *      - delete-min:   O(log n)
 *
 *    Память:
 *      - O(n)
 *
 * -----------------------------------------------------------------------------
 *
 * 8) Реализация на C
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Узел левосторонней кучи */
typedef struct LeftistNode {
    int key;
    int npl;                    /* null path length */
    struct LeftistNode *left;
    struct LeftistNode *right;
} LeftistNode;

/* Создание нового узла */
static LeftistNode *leftist_new_node(int key) {
    LeftistNode *n = malloc(sizeof(LeftistNode));
    if (!n) { perror("malloc"); exit(EXIT_FAILURE); }
    n->key = key;
    n->npl = 0;
    n->left = n->right = NULL;
    return n;
}

/* Получение NPL */
static int npl(LeftistNode *n) {
    return n ? n->npl : -1;
}

/**
 * merge (разрушающее)
 * -------------------
 * Сливает две левосторонние кучи
 */
LeftistNode *leftist_merge(LeftistNode *a, LeftistNode *b) {
    if (!a) return b;
    if (!b) return a;

    /* min-heap */
    if (b->key < a->key) {
        LeftistNode *tmp = a;
        a = b;
        b = tmp;
    }

    a->right = leftist_merge(a->right, b);

    /* восстановление левостороннего свойства */
    if (npl(a->left) < npl(a->right)) {
        LeftistNode *tmp = a->left;
        a->left = a->right;
        a->right = tmp;
    }

    a->npl = 1 + npl(a->right);
    return a;
}

/**
 * insert
 * ------
 * Вставка элемента
 */
LeftistNode *leftist_insert(LeftistNode *heap, int key) {
    LeftistNode *n = leftist_new_node(key);
    return leftist_merge(heap, n);
}

/**
 * find-min
 * --------
 * Поиск минимума
 */
int leftist_find_min(LeftistNode *heap) {
    assert(heap);
    return heap->key;
}

/**
 * delete-min
 * ----------
 * Удаление минимума
 */
LeftistNode *leftist_delete_min(LeftistNode *heap) {
    assert(heap);
    LeftistNode *res = leftist_merge(heap->left, heap->right);
    free(heap);
    return res;
}

/**
 * destroy
 * -------
 * Удаление всей кучи
 */
void leftist_destroy(LeftistNode *heap) {
    if (!heap) return;
    leftist_destroy(heap->left);
    leftist_destroy(heap->right);
    free(heap);
}

/* ------------------------------------------------------------
 * Неразрушающее слияние (копирование)
 * ------------------------------------------------------------ */

/* Копирование дерева */
LeftistNode *leftist_clone(LeftistNode *h) {
    if (!h) return NULL;
    LeftistNode *n = leftist_new_node(h->key);
    n->npl = h->npl;
    n->left = leftist_clone(h->left);
    n->right = leftist_clone(h->right);
    return n;
}

/**
 * merge без разрушения
 */
LeftistNode *leftist_merge_persistent(LeftistNode *a, LeftistNode *b) {
    LeftistNode *ca = leftist_clone(a);
    LeftistNode *cb = leftist_clone(b);
    return leftist_merge(ca, cb);
}

/* Печать (inorder, не сортировка!) */
void leftist_print(LeftistNode *h) {
    if (!h) return;
    leftist_print(h->left);
    printf("%d ", h->key);
    leftist_print(h->right);
}

#ifdef LEFTIST_HEAP_TEST
int main(void) {
    LeftistNode *h1 = NULL;
    int vals[] = {5, 3, 9, 1, 7, 2};
    size_t n = sizeof(vals)/sizeof(vals[0]);

    for (size_t i = 0; i < n; ++i)
        h1 = leftist_insert(h1, vals[i]);

    printf("Min: %d\n", leftist_find_min(h1));

    printf("Delete-min sequence: ");
    while (h1) {
        printf("%d ", leftist_find_min(h1));
        h1 = leftist_delete_min(h1);
    }
    printf("\n");

    /* Демонстрация merge */
    LeftistNode *a = NULL, *b = NULL;
    a = leftist_insert(a, 1);
    a = leftist_insert(a, 4);
    b = leftist_insert(b, 2);
    b = leftist_insert(b, 3);

    LeftistNode *m = leftist_merge(a, b);
    printf("Merged delete-min: ");
    while (m) {
        printf("%d ", leftist_find_min(m));
        m = leftist_delete_min(m);
    }
    printf("\n");

    return 0;
}
#endif /* LEFTIST_HEAP_TEST */
