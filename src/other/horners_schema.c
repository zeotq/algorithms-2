int find_index(char symbol, char* alphabet) {
    for (int i = 0; alphabet[i] != '\0'; i++) {
        if (alphabet[i] == symbol) {
            return i;
        }
    }
}

int to_dec(char* str, int size, int base, long int * dec) {
    char* alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    double answer = 0.0;
    int k = size-1;
    for (int i = 0; str[i] != '\0'; i++, k--) {
        int index = find_index(str[i], alphabet);
        if (index >= base){
            return -1;
        }
        answer *= base;
        answer += index;
    }
    *dec = (long int) answer;
    return 0;
}

/*
int number = 100;
int i = 0;
char answer[BUFSIZ];
char* alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
while (number > 0){
    char current = alphabet[(number % base)];
    number /= base;
    answer[i++] = current
}
*/
