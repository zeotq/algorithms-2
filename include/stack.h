#ifndef STACK_DEF

#include <stdbool.h>
#include <stdlib.h>

#define STACK_DEF(dtype, dname)                                \
                                                               \
typedef struct dname##node_ {                                  \
    dtype data;                                                \
    struct dname##node_ *next;                                 \
} dname##_node_t;                                              \
                                                               \
typedef struct dname##_ {                                      \
    dname##_node_t *top;                                       \
} dname##_t;                                                   \
                                                               \
static dname##_t *dname##_new(void) {                          \
    dname##_t *new_stack = malloc(sizeof(dname##_t));          \
    new_stack->top = NULL;                                     \
    return new_stack;                                          \
}                                                              \
                                                               \
static int dname##_push(dname##_t *stck, dtype value) {        \
    dname##_node_t *new_node = malloc(sizeof(dname##_node_t)); \
    if (new_node == NULL) return -1;                           \
    new_node->data = value;                                    \
    new_node->next = stck->top;                                \
    stck->top = new_node;                                      \
    return 0;                                                  \
}                                                              \
                                                               \
static dtype dname##_pop(dname##_t *stck) {                    \
    dname##_node_t *top = stck->top;                           \
    if (top == NULL) exit(EXIT_FAILURE);                       \
    stck->top = top->next;                                     \
    dtype data = top->data;                                    \
    free(top);                                                 \
    return data;                                               \
}                                                              \
                                                               \
static dtype dname##_top(dname##_t *stck) {                    \
    return stck->top->data;                                    \
}                                                              \
                                                               \
static bool dname##_is_empty(dname##_t *stck) {                \
    return (stck->top == NULL);                                \
}                                                              \
                                                               \
static void dname##_destroy(dname##_t *stck) {                 \
    dname##_node_t *cur = stck->top;                           \
    while (cur != NULL) {                                      \
        dname##_node_t *next = cur->next;                      \
        free(cur);                                             \
        cur = next;                                            \
    }                                                          \
    free(stck);                                                \
}                                                              \

#endif