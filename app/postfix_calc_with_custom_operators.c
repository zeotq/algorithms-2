#include "postfix_calc_with_custom_operators.h"
#include <stdio.h>
#include <string.h>

long int op_add(long int a, long int b) { return b + a; }
long int op_sub(long int a, long int b) { return b - a; }
long int op_mul(long int a, long int b) { return b * a; }
long int op_div(long int a, long int b) { return b / a; }

int main(void) {
    char infix[1024], postfix[2048];
    if (fgets(infix, sizeof(infix), stdin) != NULL) {
        size_t len = strlen(infix);
        if (len > 0 && infix[len-1] == '\n') infix[len-1] = '\0';
        infix_to_postfix(infix, postfix);
        printf("%s\n", postfix);

        /* create operator descriptors and pass their addresses */
        postfix_op_t op_plus  = { "+", op_add };
        postfix_op_t op_minus = { "-", op_sub };
        postfix_op_t op_mul_s = { "*", op_mul };
        postfix_op_t op_div_s = { "/", op_div };

        long int res = calc_postfix_var(postfix, 4, &op_plus, &op_minus, &op_mul_s, &op_div_s);
        printf("%ld\n", res);
    }
    return 0;
}
