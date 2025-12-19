#include "infix_calc.h"
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

static long int get_value(lstack_t *stck) {
    if (lstack_is_empty(stck)) {
        printf("Stack is empty, idiot error\n");
        exit(EXIT_FAILURE);
    }
    return lstack_pop(stck);
}

static void apply_ap(stack_t *ops, lstack_t *nums) {
    long int a = get_value(nums);
    long int b = get_value(nums);
    char token = stack_pop(ops);
    long int out;
    switch (token) {
        case '+': out = a + b; break;
        case '-': out = b - a; break;
        case '*': out = a * b; break;
        case '/': out = b / a; break;
        default:
            fprintf(stderr, "Unexpected operator: %c\n", token);
            exit(EXIT_FAILURE);
    }
    lstack_push(nums, out);
}

// Функция для прямого вычисления инфиксного выражения
long int infix_calc(char infix[]) {
    stack_t *stack = stack_new();
    lstack_t *nums = lstack_new();
    int i, j = 0;
    char token;

    for (i = 0; infix[i] != '\0'; i++) {
        token = infix[i];
        if (token == ' ') continue;

        if (isdigit((unsigned char)token)) {  /* parse multi-digit number */
            long int num = 0;
            while (isdigit((unsigned char)infix[i])) {
                num = num * 10 + (infix[i] - '0');
                i++;
            }
            lstack_push(nums, num);
            i--;
            continue;
        } else if (token == '(') {
            stack_push(stack, token);
        } else if (token == ')') {
            while (!stack_is_empty(stack) && stack_top(stack) != '(') {
                apply_ap(stack, nums);
            }
            if (!stack_is_empty(stack) && stack_top(stack) == '(') {
                stack_pop(stack);
            }
        } else { /* operator */
            while (!stack_is_empty(stack) && priority(stack_top(stack)) >= priority(token)) {
                apply_ap(stack, nums);
            }
            stack_push(stack, token);
        }
    }

    /* flush remaining operators */
    while (!stack_is_empty(stack)) {
        apply_ap(stack, nums);
    }

    long int result = lstack_top(nums);
    lstack_destroy(nums);
    stack_destroy(stack);
    return result;
}
