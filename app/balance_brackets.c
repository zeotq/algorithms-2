#include <stdio.h>
#include <string.h>

int is_balanced(const char *expression);

int main(void) {
    char buffer[1024];

    while (fgets(buffer, sizeof buffer, stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
        if (buffer[0] == '\0') continue;
        printf("'%s' - %s\n", buffer, is_balanced(buffer) ? "Сбалансировано" : "Не сбалансировано");
    }

    return 0;
}
