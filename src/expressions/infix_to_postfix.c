#include "infix_to_postfix.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>


static int priority(char op) {
    switch(op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

// Функция для преобразования инфиксного выражения в постфиксное
void infix_to_postfix(char infix[], char postfix[]) {
    stack_t *stack = stack_new();
    int i, j = 0;
    char token;

    for (i = 0; infix[i] != '\0'; i++) {
        token = infix[i];
        if (token == ' ') continue;

        if (isalnum((unsigned char)token)) {
            /* collect multi-char identifier/number */
            postfix[j++] = token;
            while (isalnum((unsigned char)infix[i + 1])) {
                postfix[j++] = infix[++i];
            }
            /* separate tokens with a space */
            postfix[j++] = ' ';
        } else if (token == '(') {
            stack_push(stack, token);
        } else if (token == ')') {
            while (!stack_is_empty(stack) && stack_top(stack) != '(') {
                postfix[j++] = stack_pop(stack);
                postfix[j++] = ' ';
            }
            if (!stack_is_empty(stack) && stack_top(stack) == '(') {
                stack_pop(stack);
            }
        } else { /* operator */
            while (!stack_is_empty(stack) && priority(stack_top(stack)) >= priority(token)) {
                postfix[j++] = stack_pop(stack);
                postfix[j++] = ' ';
            }
            stack_push(stack, token);
        }
    }

    /* flush remaining operators */
    while (!stack_is_empty(stack)) {
        postfix[j++] = stack_pop(stack);
        postfix[j++] = ' ';
    }

    if (j > 0 && postfix[j-1] == ' ') j--; /* trim trailing space */
    postfix[j] = '\0';

    stack_destroy(stack);
}
