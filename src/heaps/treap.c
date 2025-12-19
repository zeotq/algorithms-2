// Декартово дерево: свойства бинарного дерева поиска (BST) и бинарной кучи (Heap)

/**
 * treap.c
 *
 * Реализация и теория: Декартово дерево (Treap)
 *
 * Содержание:
 * 1. Что такое декартово дерево
 * 2. Свойства: BST + Heap
 * 3. Связь операции поиска с бинарным (дихотомическим) поиском
 * 4. Базовые операции: split и merge
 * 5. Вставка и удаление элементов
 * 6. Вычислительные сложности
 * 7. Реализация на C
 * 8. Тестовый пример
 *
 * -----------------------------------------------------------------------------
 *
 * 1) Что такое декартово дерево?
 *    Декартово дерево (treap = tree + heap) — это структура данных,
 *    которая одновременно удовлетворяет:
 *      - свойствам бинарного дерева поиска (BST) по ключу;
 *      - свойствам бинарной кучи (heap) по приоритету.
 *
 *    Обычно:
 *      - ключ (key) — задаётся пользователем;
 *      - приоритет (priority) — случайное число.
 *
 *    Такая комбинация приводит к тому, что дерево в среднем
 *    сбалансировано, без явных операций балансировки (в отличие от AVL / RB).
 *
 * -----------------------------------------------------------------------------
 *
 * 2) Свойства декартова дерева
 *
 *    Пусть у узла есть:
 *      - key      — ключ (для BST)
 *      - priority — приоритет (для heap)
 *
 *    Инварианты:
 *
 *    (1) BST-свойство по key:
 *        - все ключи в левом поддереве < key
 *        - все ключи в правом поддереве > key
 *
 *    (2) Heap-свойство по priority (max-heap):
 *        - priority узла >= priority его детей
 *
 *    Эти два свойства одновременно однозначно определяют форму дерева,
 *    если все ключи и приоритеты уникальны.
 *
 * -----------------------------------------------------------------------------
 *
 * 3) Поиск в декартовом дереве и связь с бинарным поиском
 *
 *    Поиск элемента по ключу выполняется ТОЛЬКО по BST-свойству:
 *
 *      если key < node->key → идём влево
 *      если key > node->key → идём вправо
 *
 *    Это полностью аналогично бинарному поиску:
 *      - на каждом шаге пространство поиска уменьшается вдвое
 *        (в среднем, при сбалансированном дереве)
 *
 *    Средняя сложность поиска: O(log n)
 *    Худшая (вырожденное дерево): O(n)
 *
 * -----------------------------------------------------------------------------
 *
 * 4) Базовые операции: split и merge
 *
 *    Декартово дерево удобно реализуется через две фундаментальные операции:
 *
 *    split(T, key):
 *      - разбивает дерево T на два:
 *          L — все ключи < key
 *          R — все ключи >= key
 *
 *    merge(L, R):
 *      - объединяет два дерева, где:
 *          max(key в L) < min(key в R)
 *
 *    Все остальные операции (insert, delete) выражаются через них.
 *
 * -----------------------------------------------------------------------------
 *
 * 5) Вставка и удаление
 *
 *    Вставка (key, priority):
 *      1. split(root, key) → (L, R)
 *      2. создать новый узел N
 *      3. root = merge(merge(L, N), R)
 *
 *    Удаление key:
 *      1. split(root, key)       → (L, R)
 *      2. split(R, key + eps)    → (M, R2)
 *      3. удалить M
 *      4. root = merge(L, R2)
 *
 * -----------------------------------------------------------------------------
 *
 * 6) Вычислительные сложности
 *
 *    Пусть n — число элементов.
 *
 *    В среднем (за счёт случайных приоритетов):
 *      - Поиск:        O(log n)
 *      - Вставка:      O(log n)
 *      - Удаление:     O(log n)
 *      - Память:       O(n)
 *
 *    В худшем случае:
 *      - Все операции: O(n)
 *
 * -----------------------------------------------------------------------------
 *
 * 7) Реализация на C
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

/* Узел декартова дерева */
typedef struct TreapNode {
    int key;                /* ключ (BST) */
    int priority;           /* приоритет (heap) */
    struct TreapNode *left;
    struct TreapNode *right;
} TreapNode;

/* Создание нового узла */
static TreapNode *treap_new_node(int key) {
    TreapNode *n = (TreapNode *)malloc(sizeof(TreapNode));
    if (!n) { perror("malloc"); exit(EXIT_FAILURE); }
    n->key = key;
    n->priority = rand();
    n->left = n->right = NULL;
    return n;
}

/**
 * split
 * -----
 * Разбивает дерево t по ключу key:
 *   - l: все ключи < key
 *   - r: все ключи >= key
 */
void treap_split(TreapNode *t, int key, TreapNode **l, TreapNode **r) {
    // 1. Если дерево пустое — обе части тоже пустые
    if (!t) {
        *l = *r = NULL;
    }
    // 2. Если наш ключ разделения МЕНЬШЕ ИЛИ РАВЕН текущему узлу
    else if (key <= t->key) {
        // "Режем" ЛЕВОЕ поддерево тем же ключом
        treap_split(t->left, key, l, &t->left);
        // ПРАВОЙ частью становится текущий узел
        *r = t;
    }
    // 3. Если наш ключ разделения БОЛЬШЕ текущего узла
    else {
        // "Режем" ПРАВОЕ поддерево тем же ключом
        treap_split(t->right, key, &t->right, r);
        // ЛЕВОЙ частью становится текущий узел
        *l = t;
    }
}

/**
 * merge
 * -----
 * Объединяет два дерева l и r, где
 *   max(key в l) < min(key в r)
 */
TreapNode *treap_merge(TreapNode *l, TreapNode *r) {
    // 1. Если одно дерево пустое — возвращаем другое
    if (!l || !r) return l ? l : r;
    
    // 2. Смотрим, у кого приоритет ВЫШЕ (кто "важнее")
    if (l->priority > r->priority) {
        // Левый узел "важнее" — он будет корнем
        // Склеиваем его правое поддерево с правым деревом r
        l->right = treap_merge(l->right, r);
        return l;  // Возвращаем левый узел как новый корень
    } else {
        // Правый узел "важнее" — он будет корнем
        // Склеиваем левое дерево l с левым поддеревом правого узла
        r->left = treap_merge(l, r->left);
        return r;  // Возвращаем правый узел как новый корень
    }
}

/**
 * treap_insert
 * ------------
 * Вставка ключа в декартово дерево
 */
TreapNode *treap_insert(TreapNode *root, int key) {
    TreapNode *l, *r;
    treap_split(root, key, &l, &r);
    TreapNode *n = treap_new_node(key);
    return treap_merge(treap_merge(l, n), r);
}

/**
 * treap_remove
 * ------------
 * Удаление ключа из декартова дерева
 */
TreapNode *treap_remove(TreapNode *root, int key) {
    if (!root) return NULL;
    if (root->key == key) {
        TreapNode *res = treap_merge(root->left, root->right);
        free(root);
        return res;
    }
    if (key < root->key)
        root->left = treap_remove(root->left, key);
    else
        root->right = treap_remove(root->right, key);
    return root;
}

/**
 * treap_search
 * ------------
 * Поиск ключа (аналог бинарного поиска)
 */
bool treap_search(TreapNode *root, int key) {
    while (root) {
        if (key == root->key) return true;
        root = (key < root->key) ? root->left : root->right;
    }
    return false;
}

/* Удаление всего дерева */
void treap_destroy(TreapNode *root) {
    if (!root) return;
    treap_destroy(root->left);
    treap_destroy(root->right);
    free(root);
}

/* Симметричный обход (для проверки BST-свойства) */
void treap_inorder(TreapNode *root) {
    if (!root) return;
    treap_inorder(root->left);
    printf("%d ", root->key);
    treap_inorder(root->right);
}

#ifdef TREAP_TEST
int main(void) {
    srand((unsigned)time(NULL));

    TreapNode *root = NULL;
    int vals[] = {5, 2, 8, 1, 4, 7, 9};
    size_t n = sizeof(vals)/sizeof(vals[0]);

    for (size_t i = 0; i < n; ++i)
        root = treap_insert(root, vals[i]);

    printf("Inorder traversal (sorted): ");
    treap_inorder(root);
    printf("\n");

    printf("Search 4: %s\n", treap_search(root, 4) ? "found" : "not found");
    printf("Search 10: %s\n", treap_search(root, 10) ? "found" : "not found");

    root = treap_remove(root, 5);
    printf("After removing 5: ");
    treap_inorder(root);
    printf("\n");

    treap_destroy(root);
    return 0;
}
#endif /* TREAP_TEST */
