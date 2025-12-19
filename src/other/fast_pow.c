double fast_pow(double x, int n) {
    double result = 1.0;
    long long power = n;

    if (power < 0) {
        x = 1.0 / x;
        power = -power;
    }

    while (power > 0) {
        if (power & 1) {
            result *= x;
        }
        x *= x;
        power >>= 1;
    }
    return result;
}
