#include <stdio.h>
#include <string.h>

void infix_to_postfix(char infix[], char postfix[]);

int main(void) {
    char infix[1024], postfix[2048];
    if (fgets(infix, sizeof(infix), stdin) != NULL) {
        size_t len = strlen(infix);
        if (len > 0 && infix[len-1] == '\n') infix[len-1] = '\0';
        infix_to_postfix(infix, postfix);
        printf("%s\n", postfix);
    }
    return 0;
}
