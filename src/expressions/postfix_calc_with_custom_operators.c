#include "postfix_calc_with_custom_operators.h"
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>

static long int get_value(lstack_t *stck) {
    if (lstack_is_empty(stck)) {
        printf("Stack is empty, idiot error\n");
        exit(EXIT_FAILURE);
    }
    return lstack_pop(stck);
}

static postfix_op_t *find_operator(char token, int count, va_list ap)
{
    for (int i = 0; i < count; i++) {
        postfix_op_t *op = va_arg(ap, postfix_op_t *);
        if (op && op->op[0] == token && op->op[1] == '\0')
            return op;
    }
    return NULL;
}

long int calc_postfix_var(char postfix[], int op_count, ...)
{
    lstack_t *nums = lstack_new();
    int i = 0;
    char token;

    while ((token = postfix[i]) != '\0') {
        if (token == ' ') { i++; continue; }

        if (isalpha((unsigned char)token)) {
            fprintf(stderr, "Alpha not supported.\n");
            exit(EXIT_FAILURE);
        }

        /* число */
        if (isdigit((unsigned char)token)) {
            long int num = 0;
            while (isdigit((unsigned char)postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            lstack_push(nums, num);
            continue;
        }

        /* оператор */
        long int a = get_value(nums);
        long int b = get_value(nums);

        va_list ap;
        va_start(ap, op_count);
        postfix_op_t *op = find_operator(token, op_count, ap);
        va_end(ap);

        if (!op) {
            fprintf(stderr, "Unexpected operator: %c\n", token);
            exit(EXIT_FAILURE);
        }

        long int out = op->func(a, b);
        lstack_push(nums, out);
        i++;
    }

    long int result = lstack_top(nums);
    lstack_destroy(nums);
    return result;
}
