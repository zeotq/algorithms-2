/**
 * binary_tree.c
 *
 * Реализация и теория: Бинарное дерево поиска (Binary Search Tree, BST)
 *
 * Содержание (на русском):
 * 1. Краткая теория о бинарных деревьях поиска
 * 2. Создание и удаление дерева
 * 3. Операция поиска и её связь с бинарным (дихотомическим) поиском
 * 4. Обходы: префиксный (pre-order), инфиксный (in-order), постфиксный (post-order)
 * 5. Вставка и удаление узла в BST
 * 6. Вычислительные сложности (в среднем и в худшем случае)
 * 7. Реализация на языке C с примерами использования
 *
 * -----------------------------------------------------------------------------
 *
 * 1) Что такое бинарное дерево поиска (BST)?
 *    Бинарное дерево поиска — это бинарное дерево, в котором для любого узла:
 *      - все ключи в левом поддереве строго меньше ключа в узле;
 *      - все ключи в правом поддереве строго больше ключа в узле.
 *    Такое ограничение позволяет эффективно выполнять операции поиска, вставки
 *    и удаления, следуя аналогии с дихотомическим (бинарным) поиском в отсортированном
 *    массиве.
 *
 * 2) Создание и удаление дерева
 *    - Создание узла: выделение памяти и инициализация полей (ключ, указатели на детей).
 *    - Создание дерева обычно сводится к установке указателя корня в NULL (пустое дерево).
 *    - Удаление дерева: рекурсивно освобождать левое и правое поддеревья, затем
 *      освобождать сам узел (постфиксный обход).
 *
 * 3) Поиск и связь с бинарным поиском
 *    - Поиск в BST проводится аналогично двоичному поиску: начиная с корня,
 *      сравниваем искомый ключ с ключом в текущем узле и либо возвращаем найденный
 *      узел, либо переходим в левое (меньше) или правое (больше) поддерево.
 *    - В сбалансированном BST сложность поиска равна O(log n) (аналогично бинарному
 *      поиску в отсортированном массиве). В худшем случае (когда дерево вырождается
 *      в список) сложность — O(n).
 *
 * 4) Обходы деревьев
 *    - Префиксный (pre-order): посещаем узел, затем левое поддерево, затем правое.
 *      Полезен для копирования дерева или получения префиксной формы выражений.
 *    - Инфиксный (in-order): левое, узел, правое. Для BST это дает ключи в
 *      возрастающем порядке (сортированный вывод).
 *    - Постфиксный (post-order): левое, правое, узел. Удобен для освобождения памяти
 *      (удаление узла после детей).
 *
 * 5) Вставка и удаление
 *    - Вставка: спуск от корня; при достижении NULL создаём новый узел в этом месте.
 *      Сложность: O(h), где h — высота дерева; в среднем O(log n), в худшем O(n).
 *    - Удаление имеет три основных случая:
 *        a) Удаляемый узел — лист (нет детей): просто освободить узел.
 *        b) Узел имеет одного ребёнка: заменить узел его ребёнком.
 *        c) Узел имеет двух детей: найти либо преемника (минимум в правом поддереве),
 *           либо предшественника (максимум в левом поддереве), заменить ключ, затем
 *           рекурсивно удалить тот элемент (который теперь дублируется). Сложность — O(h).
 *
 * 6) Сложности (общие):
 *    - Поиск: avg O(log n), worst O(n)
 *    - Вставка: avg O(log n), worst O(n)
 *    - Удаление: avg O(log n), worst O(n)
 *    - Построение дерева вставками: avg O(n log n), worst O(n^2)
 *
 * 7) Реализация в C
 *    Ниже приведены функции:
 *      - создание узла: bst_create_node
 *      - вставка: bst_insert (рекурсивная)
 *      - поиск: bst_search (рекурсивный и итеративный)
 *      - удаление: bst_delete (рекурсивная реализация со случаями)
 *      - обходы: bst_preorder, bst_inorder, bst_postorder
 *      - освобождение всего дерева: bst_free
 *
 *    Дополнительно приведён простой пример (под макросом BINARY_TREE_TEST).
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Узел бинарного дерева поиска */
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

/**
 * bst_create_node
 * ----------------
 * Создаёт новый узел с заданным ключом. Поля детей инициализируются NULL.
 *
 * Сложность: O(1) по времени и памяти (за исключением выделения).
 */
Node *bst_create_node(int key) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

/**
 * bst_insert
 * ----------
 * Вставляет ключ в дерево и возвращает (возможно обновлённый) корень.
 * Рекурсивная реализация: сравниваем ключ с текущим узлом и спускаемся
 * в соответствующее поддерево; если место свободно (NULL), создаём новый узел.
 *
 * Сложность: O(h) времени, где h — высота дерева; в среднем O(log n), в худшем O(n).
 */
Node *bst_insert(Node *root, int key) {
    if (root == NULL) {
        return bst_create_node(key);
    }
    if (key < root->key) {
        root->left = bst_insert(root->left, key);
    } else if (key > root->key) {
        root->right = bst_insert(root->right, key);
    } else {
        /* Если ключ уже есть, для простоты ничего не делаем (можно хранить счётчик)
           Альтернативы: разрешить дубликаты, например, помещая их в правое поддерево. */
    }
    return root;
}

/**
 * bst_search_recursive
 * --------------------
 * Рекурсивный поиск ключа в дереве. Возвращает указатель на узел или NULL.
 *
 * Сложность: O(h) времени.
 */
Node *bst_search_recursive(Node *root, int key) {
    if (root == NULL) return NULL;
    if (root->key == key) return root;
    if (key < root->key) return bst_search_recursive(root->left, key);
    return bst_search_recursive(root->right, key);
}

/**
 * bst_search_iterative
 * --------------------
 * Итеративный вариант поиска, эквивалентен рекурсивному, но без расхода
 * на стек вызовов (внутренний стек поддерживается программистом в виде цикла).
 */
Node *bst_search_iterative(Node *root, int key) {
    Node *cur = root;
    while (cur) {
        if (cur->key == key) return cur;
        if (key < cur->key) cur = cur->left;
        else cur = cur->right;
    }
    return NULL;
}

/**
 * bst_min
 * -------
 * Возвращает узел с минимальным ключом в данном (непустом) дереве —
 * просто идёт влево до NULL.
 */
Node *bst_min(Node *root) {
    assert(root != NULL);
    Node *cur = root;
    while (cur->left) cur = cur->left;
    return cur;
}

/**
 * bst_delete
 * ----------
 * Удаление узла с заданным ключом из BST. Возвращает (возможно обновлённый)
 * указатель на корень поддерева.
 *
 * Случаи удаления:
 *  - узел — лист: просто освобождаем
 *  - узел с одним ребёнком: заменяем собой ребёнка
 *  - узел с двумя детьми: находим преемника (минимум в правом поддереве),
 *    копируем его ключ в текущий узел и рекурсивно удаляем преемника
 *    (который гарантированно имеет не более одного ребёнка — правого).
 *
 * Сложность: O(h)
 */
Node *bst_delete(Node *root, int key) {
    if (root == NULL) return NULL;
    if (key < root->key) {
        root->left = bst_delete(root->left, key);
    } else if (key > root->key) {
        root->right = bst_delete(root->right, key);
    } else {
        /* Найден узел для удаления */
        if (root->left == NULL && root->right == NULL) {
            /* Лист */
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            Node *r = root->right;
            free(root);
            return r;
        } else if (root->right == NULL) {
            Node *l = root->left;
            free(root);
            return l;
        } else {
            /* Два ребёнка: заменяем ключом преемника (минимум правого поддерева) */
            Node *succ = bst_min(root->right);
            root->key = succ->key;
            root->right = bst_delete(root->right, succ->key);
        }
    }
    return root;
}

/**
 * bst_free
 * --------
 * Рекурсивно освобождает всё дерево (постфиксный обход): сначала дети, затем
 * сам узел. Используется для корректного удаления всех выделенных ресурсов.
 */
void bst_free(Node *root) {
    if (root == NULL) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}

/**
 * Обходы дерева (рекурсивно):
 * - bst_preorder:   node, left, right
 * - bst_inorder:    left, node, right  (для BST возвращает отсортированные ключи)
 * - bst_postorder:  left, right, node
 *
 * Сложность каждого обхода: O(n) — посещаем каждый узел ровно один раз.
 */
void bst_preorder(Node *root, void (*visit)(int)) {
    if (!root) return;
    visit(root->key);
    bst_preorder(root->left, visit);
    bst_preorder(root->right, visit);
}

void bst_inorder(Node *root, void (*visit)(int)) {
    if (!root) return;
    bst_inorder(root->left, visit);
    visit(root->key);
    bst_inorder(root->right, visit);
}

void bst_postorder(Node *root, void (*visit)(int)) {
    if (!root) return;
    bst_postorder(root->left, visit);
    bst_postorder(root->right, visit);
    visit(root->key);
}

/* Вспомогательная функция для вывода ключа */
static void print_key(int k) {
    printf("%d ", k);
}

/* Небольшой тестовый пример — включается, если определить макрос BINARY_TREE_TEST */
#ifdef BINARY_TREE_TEST
int main(void) {
    Node *root = NULL;
    int vals[] = { 50, 30, 70, 20, 40, 60, 80 };
    for (size_t i = 0; i < sizeof(vals)/sizeof(vals[0]); ++i) {
        root = bst_insert(root, vals[i]);
    }

    printf("In-order (sorted): ");
    bst_inorder(root, print_key);
    printf("\n");

    printf("Pre-order: ");
    bst_preorder(root, print_key);
    printf("\n");

    printf("Post-order: ");
    bst_postorder(root, print_key);
    printf("\n");

    int key = 60;
    Node *f = bst_search_iterative(root, key);
    printf("Search %d: %s\n", key, f ? "found" : "not found");

    root = bst_delete(root, 70); /* удаляем узел с двумя детьми */
    printf("After deleting 70, in-order: ");
    bst_inorder(root, print_key);
    printf("\n");

    bst_free(root);
    return 0;
}
#endif /* BINARY_TREE_TEST */
