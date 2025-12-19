#include <stdio.h>
#include <string.h>

long int infix_calc(char infix[]);

int main(void) {
    char infix[1024];
    if (fgets(infix, sizeof(infix), stdin) != NULL) {
        size_t len = strlen(infix);
        if (len > 0 && infix[len-1] == '\n') infix[len-1] = '\0';
        long int res = infix_calc(infix);
        printf("%ld\n", res);
    }
    return 0;
}
