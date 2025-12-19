#include "postfix_calc.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>


long int get_value(lstack_t *stck) {
    if (lstack_is_empty(stck)) {
        printf("Stack is empty, idiot error\n");
        exit(EXIT_FAILURE);
    }
    return lstack_pop(stck);
}


long int calc_postfix(char postfix[]) {
    lstack_t *nums = lstack_new();
    int i = 0;
    char token;

    while ((token = postfix[i]) != '\0') {
        if (token == ' ') { i++; continue; }

        if (isalpha((unsigned char)token)) {
            fprintf(stderr, "Alpha not supported.\n");
            exit(EXIT_FAILURE);
        }

        if (isdigit((unsigned char)token)) {  /* parse multi-digit number */
            long int num = 0;
            while (isdigit((unsigned char)postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            lstack_push(nums, num);
            continue;
        }

        /* operator */
        long int a = get_value(nums);
        long int b = get_value(nums);
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
        i++;
    }

    long int result = lstack_top(nums);
    lstack_destroy(nums);
    return result;
}