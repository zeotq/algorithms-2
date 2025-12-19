#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define CONCAT_OK        0
#define CONCAT_OVERFLOW  1
#define CONCAT_BAD_FIRST 2
#define CONCAT_ALLOC_ERR 3

int concat(char **result, char *first, ...)
{
    if (!result || !first)
        return CONCAT_BAD_FIRST;

    *result = NULL;

    size_t capacity = 16;
    size_t len = 0;

    char *buf = malloc(capacity);
    if (!buf)
        return CONCAT_ALLOC_ERR;

    va_list ap;
    va_start(ap, first);

    char *s = first;

    while (s) {
        size_t slen = strlen(s);

        /* Проверка переполнения size_t */
        if (len > __SIZE_MAX__ - slen - 1) {
            free(buf);
            va_end(ap);
            return CONCAT_OVERFLOW;
        }

        /* Расширение буфера при необходимости */
        while (len + slen + 1 > capacity) {
            if (capacity > __SIZE_MAX__ / 2) {
                free(buf);
                va_end(ap);
                return CONCAT_OVERFLOW;
            }

            capacity *= 2;
            char *tmp = realloc(buf, capacity);
            if (!tmp) {
                free(buf);
                va_end(ap);
                return CONCAT_ALLOC_ERR;
            }
            buf = tmp;
        }

        memcpy(buf + len, s, slen);
        len += slen;

        s = va_arg(ap, char *);
    }

    va_end(ap);

    buf[len] = '\0';
    *result = buf;

    return CONCAT_OK;
}
