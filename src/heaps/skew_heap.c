/**
 * skew_heap.c
 *
 * Реализация и теория: Косая приоритетная очередь (Skew Heap)
 *
 * Содержание:
 * 1. Приоритетные очереди и место косой кучи
 * 2. Идея косой приоритетной очереди
 * 3. Отличие от левосторонней кучи
 * 4. Базовая операция merge
 * 5. Операции: insert, find-min, delete-min
 * 6. Слияние с разрушением и без разрушения
 * 7. Вычислительные сложности
 * 8. Реализация на языке C
 * 9. Тестовый пример
 *
 * -----------------------------------------------------------------------------
 *
 * 1) Приоритетная очередь
 *
 *    Приоритетная очередь поддерживает:
 *      - вставку элемента
 *      - поиск элемента с наивысшим приоритетом (min / max)
 *      - удаление элемента с наивысшим приоритетом
 *
 *    Косая очередь — это реализация приоритетной очереди на базе
 *    бинарного дерева, оптимизированная под операцию слияния.
 *
 * -----------------------------------------------------------------------------
 *
 * 2) Косая приоритетная очередь (Skew Heap)
 *
 *    Косая куча — это бинарное дерево, удовлетворяющее ТОЛЬКО
 *    heap-свойству:
 *
 *      (min-heap)
 *        key(node) <= key(children)
 *
 *    В отличие от левосторонней кучи:
 *      - НЕ хранит NPL
 *      - НЕ поддерживает явного балансного инварианта
 *
 *    Балансировка достигается неявно за счёт
 *    регулярных обменов поддеревьев.
 *
 * -----------------------------------------------------------------------------
 *
 * 3) Отличие от левосторонней кучи
 *
 *    Левосторонняя куча:
 *      - хранит NPL
 *      - строгое левостороннее свойство
 *      - гарантированное O(log n) для merge
 *
 *    Косая куча:
 *      - нет NPL
 *      - после каждого merge происходит swap детей
 *      - амортизированная сложность O(log n)
 *      - код проще и компактнее
 *
 * -----------------------------------------------------------------------------
 *
 * 4) Базовая операция merge
 *
 *    merge(h1, h2):
 *      1. выбираем корень с меньшим ключом
 *      2. рекурсивно сливаем правое поддерево
 *      3. меняем местами левого и правого ребёнка
 *
 *    ВАЖНО:
 *      swap выполняется ВСЕГДА, без проверки условий.
 *
 *    Все остальные операции выражаются через merge.
 *
 * -----------------------------------------------------------------------------
 *
 * 5) Основные операции
 *
 *    insert(x):
 *      - создать одноузловую кучу
 *      - merge(heap, node)
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
 * 6) Слияние
 *
 *    Разрушающее:
 *      - использует существующие узлы
 *      - амортизированно O(log n)
 *
 *    Неразрушающее:
 *      - копирование деревьев
 *      - O(n + m)
 *
 * -----------------------------------------------------------------------------
 *
 * 7) Вычислительные сложности
 *
 *    Пусть n — количество элементов.
 *
 *    Амортизированно:
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
 * 8) Реализация на языке C
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Узел косой приоритетной очереди */
typedef struct SkewNode {
    int key;
    struct SkewNode *left;
    struct SkewNode *right;
} SkewNode;

/* Создание нового узла */
static SkewNode *skew_new_node(int key) {
    SkewNode *n = (SkewNode *)malloc(sizeof(SkewNode));
    if (!n) { perror("malloc"); exit(EXIT_FAILURE); }
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

/**
 * skew_merge (разрушающее)
 * -----------------------
 * Слияние двух косых куч (min-heap)
 */
SkewNode *skew_merge(SkewNode *a, SkewNode *b) {
    if (!a) return b;
    if (!b) return a;

    /* гарантируем, что a — корень с меньшим ключом */
    if (b->key < a->key) {
        SkewNode *tmp = a;
        a = b;
        b = tmp;
    }

    /* рекурсивное слияние правого поддерева */
    a->right = skew_merge(a->right, b);

    /* обязательный обмен детей */
    SkewNode *tmp = a->left;
    a->left = a->right;
    a->right = tmp;

    return a;
}

/**
 * insert
 * ------
 * Вставка элемента
 */
SkewNode *skew_insert(SkewNode *heap, int key) {
    SkewNode *n = skew_new_node(key);
    return skew_merge(heap, n);
}

/**
 * find-min
 * --------
 * Поиск минимума
 */
int skew_find_min(SkewNode *heap) {
    assert(heap);
    return heap->key;
}

/**
 * delete-min
 * ----------
 * Удаление минимума
 */
SkewNode *skew_delete_min(SkewNode *heap) {
    assert(heap);
    SkewNode *res = skew_merge(heap->left, heap->right);
    free(heap);
    return res;
}

/**
 * destroy
 * -------
 * Уничтожение всей кучи
 */
void skew_destroy(SkewNode *heap) {
    if (!heap) return;
    skew_destroy(heap->left);
    skew_destroy(heap->right);
    free(heap);
}

/* ------------------------------------------------------------
 * Неразрушающее слияние
 * ------------------------------------------------------------ */

/* Глубокое копирование */
SkewNode *skew_clone(SkewNode *h) {
    if (!h) return NULL;
    SkewNode *n = skew_new_node(h->key);
    n->left = skew_clone(h->left);
    n->right = skew_clone(h->right);
    return n;
}

/**
 * merge без разрушения
 */
SkewNode *skew_merge_persistent(SkewNode *a, SkewNode *b) {
    SkewNode *ca = skew_clone(a);
    SkewNode *cb = skew_clone(b);
    return skew_merge(ca, cb);
}

/* Печать (inorder, не сортировка!) */
void skew_print(SkewNode *h) {
    if (!h) return;
    skew_print(h->left);
    printf("%d ", h->key);
    skew_print(h->right);
}

#ifdef SKEW_HEAP_TEST
int main(void) {
    SkewNode *h = NULL;
    int vals[] = {5, 3, 9, 1, 7, 2};
    size_t n = sizeof(vals)/sizeof(vals[0]);

    for (size_t i = 0; i < n; ++i)
        h = skew_insert(h, vals[i]);

    printf("Min: %d\n", skew_find_min(h));

    printf("Delete-min sequence: ");
    while (h) {
        printf("%d ", skew_find_min(h));
        h = skew_delete_min(h);
    }
    printf("\n");

    /* Демонстрация merge */
    SkewNode *a = NULL, *b = NULL;
    a = skew_insert(a, 1);
    a = skew_insert(a, 4);
    b = skew_insert(b, 2);
    b = skew_insert(b, 3);

    SkewNode *m = skew_merge(a, b);
    printf("Merged delete-min: ");
    while (m) {
        printf("%d ", skew_find_min(m));
        m = skew_delete_min(m);
    }
    printf("\n");

    return 0;
}
#endif /* SKEW_HEAP_TEST */
