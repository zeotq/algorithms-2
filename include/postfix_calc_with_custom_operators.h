#pragma once

#include "postfix_calc.h"
#include <stdarg.h>

typedef long int (*op_func_t)(long int a, long int b);

typedef struct {
    const char *op;     // строка-оператор ("+", "-", "*", "/")
    op_func_t func;     // функция вычисления
} postfix_op_t;

static postfix_op_t *find_operator(char token, int count, va_list ap);
long int calc_postfix_var(char postfix[], int op_count, ...);
