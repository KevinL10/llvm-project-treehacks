#include <stdio.h>

int out[50];
int n = 10;

int _start() {
    out[0] = 0;
    out[1] = 1;

    for(int i = 2; i <= n; i++) {
        out[i] = out[i - 1] + out[i - 2];
    }

    return out[n];
}