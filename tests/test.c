int fatorial(int n) {
    if (n <= 1) return 1;
    return n * fatorial(n - 1);
}

int main() {
    int i;
    for (i = 1; i <= 10; i++) {
        printf("%d! = %d\n", i, fatorial(i));
    }
    return 0;
}
