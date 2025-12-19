#include <stdio.h>

double fast_pow(double x, int n);

int main() {
    double x;
    int n;
    
    scanf("%lf", &x);
    scanf("%d", &n);

    printf("%f", fast_pow(x, n));
}