#include "balance_brackets.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

bool is_balanced(const char *expression) {
    stack_t *s = stack_new();
    for (int i = 0; expression[i] != '\0'; i++) {
        char current = expression[i];
        
        if (current == '(' || current == '[' || current == '{') {
            stack_push(s, current);
        }
        else if (current == ')' || current == ']' || current == '}') {
            if (stack_is_empty(s)) {
                stack_destroy(s);
                return false;
            }
            
            char top_char = stack_pop(s);
            
            // Проверка соответствия скобок
            if ((current == ')' && top_char != '(') ||
                (current == ']' && top_char != '[') ||
                (current == '}' && top_char != '{')) {
                stack_destroy(s);
                return false;
            }
        }
    }
    
    bool result = stack_is_empty(s);
    stack_destroy(s);
    return result;
}
