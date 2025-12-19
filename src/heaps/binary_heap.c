/**
 * binary_heap.c
 *
 * Реализация и теория: Бинарная куча (Binary Heap) / Приоритетная очередь
 *
 * Содержание (на русском):
 * 1. Краткая теория о бинарной куче (min-heap / max-heap)
 * 2. Представление в виде массива и индексные формулы
 * 3. Операции: вставка (push), извлечение вершины (pop), просмотр вершины (peek)
 * 4. Построение кучи из массива (heapify / Floyd's algorithm)
 * 5. Сложности по времени и памяти
 * 6. Реализация на C с динамическим размером и тест-пример
 *
 * -----------------------------------------------------------------------------
 *
 * 1) Что такое бинарная куча?
 *    Бинарная куча — это частично упорядоченное полное бинарное дерево, используемое
 *    для реализации приоритетной очереди. В min-heap каждый узел <= своих детей;
 *    в max-heap каждый узел >= своих детей. Это гарантирует, что элемент с наивысшим
 *    приоритетом находится в корне.
 *
 * 2) Представление в виде массива
 *    Полное бинарное дерево удобно хранить в массиве: для индекса i (0-based):
 *      parent(i) = (i - 1) / 2
 *      left(i)   = 2*i + 1
 *      right(i)  = 2*i + 2
 *    Такое представление экономит память (нет указателей) и упрощает операции
 *    подъёма (sift-up) и опускания (sift-down).
 *
 * 3) Операции
 *    - push (insert): добавляем элемент в конец массива и выполняем sift-up
 *      (восстановление свойства кучи, сравнивая с родителем). Сложность: O(log n).
 *    - peek: возвращаем элемент в корне без удаления. Сложность: O(1).
 *    - pop (extract): удаляем корень (возвращаем его), заменяем корнем последний
 *      элемент и выполняем sift-down (опускание). Сложность: O(log n).
 *    - build (heapify): строим кучу из неупорядоченного массива за время O(n)
 *      (алгоритм Флойда: выполняем sift-down начиная с последнего неполного узла).
 *
 * 4) Сложности
 *    - Вставка: O(log n)
 *    - Извлечение: O(log n)
 *    - Просмотр корня: O(1)
 *    - Построение кучи из массива: O(n)
 *    - Память: O(n)
 *
 * 5) Реализация
 *    Ниже реализованы:
 *      - структура BinaryHeap (динамический массив, размер, ёмкость, флаг min/max)
 *      - heap_create / heap_destroy
 *      - heap_push / heap_peek / heap_pop
 *      - heap_sift_up / heap_sift_down
 *      - heap_build_from_array
 *
 *    Также добавлен тестовый main под макросом BINARY_HEAP_TEST.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

typedef struct BinaryHeap {
    int *data;        /* массив элементов */
    size_t size;      /* текущий размер (количество элементов) */
    size_t capacity;  /* вместимость массива */
    bool is_min;      /* true для min-heap, false для max-heap */
} BinaryHeap;

/* Вспомогательные индексные макросы (0-based) */
#define PARENT(i) (((i) - 1) / 2)
#define LEFT(i)   ((i) * 2 + 1)
#define RIGHT(i)  ((i) * 2 + 2)

/* Внутренняя: сравнение двух значений в куче с учётом типа (min/max) */
static inline bool heap_prefers(BinaryHeap *h, int a, int b) {
    /* возвращает true, если a имеет более высокий приоритет ("лучше") чем b */
    return h->is_min ? (a < b) : (a > b);
}

/* Обмен элементов */
static inline void heap_swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

/**
 * heap_create
 * -----------
 * Создаёт пустую кучу с заданной начальной ёмкостью и типом (min/max).
 */
BinaryHeap *heap_create(size_t capacity, bool is_min) {
    BinaryHeap *h = (BinaryHeap *)malloc(sizeof(BinaryHeap));
    if (!h) { perror("malloc"); exit(EXIT_FAILURE); }
    if (capacity == 0) capacity = 4; /* базовый минимум */
    h->data = (int *)malloc(sizeof(int) * capacity);
    if (!h->data) { perror("malloc"); exit(EXIT_FAILURE); }
    h->size = 0;
    h->capacity = capacity;
    h->is_min = is_min;
    return h;
}

/**
 * heap_resize
 * -----------
 * Увеличивает ёмкость массива вдвое (или до указанного минимума).
 */
static void heap_resize(BinaryHeap *h, size_t min_capacity) {
    size_t new_cap = h->capacity ? h->capacity * 2 : 4;
    if (new_cap < min_capacity) new_cap = min_capacity;
    int *new_data = (int *)realloc(h->data, sizeof(int) * new_cap);
    if (!new_data) { perror("realloc"); exit(EXIT_FAILURE); }
    h->data = new_data;
    h->capacity = new_cap;
}

/**
 * heap_sift_up
 * ------------
 * Поднимает элемент с индекса idx вверх до корректной позиции.
 */
static void heap_sift_up(BinaryHeap *h, size_t idx) {
    while (idx > 0) {
        size_t p = PARENT(idx);
        if (heap_prefers(h, h->data[idx], h->data[p])) {
            heap_swap(&h->data[idx], &h->data[p]);
            idx = p;
        } else break;
    }
}

/**
 * heap_sift_down
 * --------------
 * Опускает элемент с индекса idx вниз до корректной позиции.
 */
static void heap_sift_down(BinaryHeap *h, size_t idx) {
    size_t n = h->size;
    while (true) {
        size_t l = LEFT(idx);
        size_t r = RIGHT(idx);
        size_t best = idx;
        if (l < n && heap_prefers(h, h->data[l], h->data[best])) best = l;
        if (r < n && heap_prefers(h, h->data[r], h->data[best])) best = r;
        if (best != idx) {
            heap_swap(&h->data[idx], &h->data[best]);
            idx = best;
        } else break;
    }
}

/**
 * heap_push
 * ---------
 * Вставляет значение в кучу, автоматически расширяя массив при необходимости.
 * Сложность амортизированно: O(log n)
 */
void heap_push(BinaryHeap *h, int value) {
    if (h->size + 1 > h->capacity) heap_resize(h, h->size + 1);
    h->data[h->size++] = value;
    heap_sift_up(h, h->size - 1);
}

/**
 * heap_peek
 * ---------
 * Возвращает значение в корне кучи (min или max) без удаления.
 * Сложность: O(1)
 */
int heap_peek(BinaryHeap *h) {
    assert(h->size > 0);
    return h->data[0];
}

/**
 * heap_pop
 * --------
 * Извлекает и возвращает корневой элемент (приоритетный). Выполняет sift-down.
 * Сложность: O(log n)
 */
int heap_pop(BinaryHeap *h) {
    assert(h->size > 0);
    int root = h->data[0];
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        heap_sift_down(h, 0);
    }
    return root;
}

/**
 * heap_destroy
 * ------------
 * Освобождает память, занятую кучей.
 */
void heap_destroy(BinaryHeap *h) {
    if (!h) return;
    free(h->data);
    free(h);
}

/**
 * heap_build_from_array
 * ---------------------
 * Строит кучу за O(n) из заданного массива (алгоритм Флойда).
 */
BinaryHeap *heap_build_from_array(const int *arr, size_t n, bool is_min) {
    BinaryHeap *h = (BinaryHeap *)malloc(sizeof(BinaryHeap));
    if (!h) { perror("malloc"); exit(EXIT_FAILURE); }
    h->data = (int *)malloc(sizeof(int) * (n ? n : 1));
    if (!h->data) { perror("malloc"); exit(EXIT_FAILURE); }
    memcpy(h->data, arr, sizeof(int) * n);
    h->size = n;
    h->capacity = n ? n : 1;
    h->is_min = is_min;
    /* выполнять sift-down начиная с последнего родителя */
    if (n > 1) {
        for (ssize_t i = (ssize_t)PARENT(n - 1); i >= 0; --i) {
            heap_sift_down(h, (size_t)i);
        }
    }
    return h;
}

/* 
 * Дополнительные операции для приоритетных очередей (слияние)
 * -----------------------------------------------------------
 * В задании требуется также реализовать операции слияния приоритетных очередей.
 * Существует несколько подходов:
 *  - Неразрушающее слияние: создаём новую кучу, копируем элементы из обеих
 *    очередей и выполняем heapify (алгоритм Флойда). Сложность: O(n + m).
 *  - Разрушающее слияние (с уничтожением исходных очередей): можно тоже
 *    склеить массивы и выполнить heapify; после этого исходные очереди
 *    уничтожаются. Сложность: O(n + m). Альтернативно, можно последовательно
 *    вставлять элементы одной кучи в другую (heap_push) — это O(m log(n + m)) или
 *    при вставке в существующую кучу O(m log n).
 *
 * Замечания по совместимости:
 *  - Объединять имеет смысл только кучи одного типа (оба min-heap или оба max-heap).
 *  - Мы реализуем обе версии: heap_merge (неразрушающая) и
 *    heap_merge_destroy (разрушающая — освобождает исходные структуры).
 */

/**
 * heap_merge
 * ----------
 * Неразрушающее слияние двух куч: возвращает новую кучу, в которой содержатся
 * все элементы из a и b. Исходные кучи не изменяются.
 *
 * Сложности:
 *  - Время: O(n + m) (копирование + heapify)
 *  - Память: O(n + m)
 */
BinaryHeap *heap_merge(const BinaryHeap *a, const BinaryHeap *b) {
    if (!a && !b) return NULL;
    if (!a) return heap_build_from_array(b->data, b->size, b->is_min);
    if (!b) return heap_build_from_array(a->data, a->size, a->is_min);
    assert(a->is_min == b->is_min && "Heaps must be of same type to merge");
    size_t total = a->size + b->size;
    int *buf = (int *)malloc(sizeof(int) * (total ? total : 1));
    if (!buf) { perror("malloc"); exit(EXIT_FAILURE); }
    memcpy(buf, a->data, sizeof(int) * a->size);
    memcpy(buf + a->size, b->data, sizeof(int) * b->size);
    BinaryHeap *res = heap_build_from_array(buf, total, a->is_min);
    free(buf);
    return res;
}

/**
 * heap_merge_destroy
 * ------------------
 * Разрушающее слияние: объединяет две кучи и освобождает память под исходные.
 * Возвращает новую кучу (или переиспользует одну из входных структур в более
 * продвинутой реализации). Здесь простая и безопасная реализация: копируем
 * элементы во временный буфер, строим новую кучу, затем уничтожаем исходные.
 *
 * Сложности:
 *  - Время: O(n + m)
 *  - Память: O(n + m) временно
 */
BinaryHeap *heap_merge_destroy(BinaryHeap *a, BinaryHeap *b) {
    if (!a && !b) return NULL;
    if (!a) { BinaryHeap *r = heap_build_from_array(b->data, b->size, b->is_min); heap_destroy(b); return r; }
    if (!b) { BinaryHeap *r = heap_build_from_array(a->data, a->size, a->is_min); heap_destroy(a); return r; }
    assert(a->is_min == b->is_min && "Heaps must be of same type to merge");
    size_t total = a->size + b->size;
    int *buf = (int *)malloc(sizeof(int) * (total ? total : 1));
    if (!buf) { perror("malloc"); exit(EXIT_FAILURE); }
    memcpy(buf, a->data, sizeof(int) * a->size);
    memcpy(buf + a->size, b->data, sizeof(int) * b->size);
    BinaryHeap *res = heap_build_from_array(buf, total, a->is_min);
    free(buf);
    /* освободим исходные структуры */
    heap_destroy(a);
    heap_destroy(b);
    return res;
}

/* Вспомогательная функция для печати (последовательности извлечения) */
static void print_pop_sequence(BinaryHeap *h) {
    BinaryHeap copy = *h; /* shallow copy (массив не дублируем) */
    /* Для печати создадим отдельную копию данных, чтобы не разрушать исходную */
    copy.data = (int *)malloc(sizeof(int) * h->size);
    memcpy(copy.data, h->data, sizeof(int) * h->size);
    /* размер и capacity принадлежат копии */
    copy.capacity = h->size;
    while (copy.size > 0) {
        printf("%d ", heap_pop(&copy));
    }
    printf("\n");
    free(copy.data);
}

#ifdef BINARY_HEAP_TEST
int main(void) {
    int vals[] = { 5, 3, 8, 1, 9, 2, 7 };
    size_t n = sizeof(vals)/sizeof(vals[0]);

    printf("--- Max-heap test ---\n");
    BinaryHeap *maxh = heap_build_from_array(vals, n, false);
    printf("Pop sequence (should be descending): ");
    print_pop_sequence(maxh);
    heap_destroy(maxh);

    printf("--- Min-heap test ---\n");
    BinaryHeap *minh = heap_build_from_array(vals, n, true);
    printf("Pop sequence (should be ascending): ");
    print_pop_sequence(minh);
    heap_destroy(minh);

    /* Демонстрация push/pop */
    BinaryHeap *h = heap_create(0, false); /* max-heap */
    for (size_t i = 0; i < n; ++i) heap_push(h, vals[i]);
    printf("Push/pop sequence (max-heap): ");
    while (h->size > 0) printf("%d ", heap_pop(h));
    printf("\n");
    heap_destroy(h);

    return 0;
}
#endif /* BINARY_HEAP_TEST */
