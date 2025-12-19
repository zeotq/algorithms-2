#include <stdio.h>
#include <string.h>

int to_dec(char* str, int size, int base, long int * dec);

int main(void) {
    char s[256];
    int base;
    if (scanf("%255s %d", s, &base) == 2) {
        long int dec;
        if (to_dec(s, strlen(s), base, &dec) == 0) {
            printf("%ld\n", dec);
        } else {
            printf("Invalid digit for base\n");
        }
    }
    return 0;
}
