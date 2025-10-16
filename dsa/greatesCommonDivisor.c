#include <stdlib.h>
#include <stdio.h>

int main() {
    unsigned long long int a, b, n, temp, i;
    scanf("%llu", &n);
    for (i=0; i<n; i++) {
        scanf("%llu %llu", &a, &b);
        if (a < b) {
            temp = a;
            a = b;
            b = temp;
        }

        if (a % b == 0) {
            printf("%llu", b);
            if (i != (n-1))
                printf("\n");
            continue;
        }
        
        while (b != 0) {
            temp = b;
            b = a % b;
            a = temp;
        }

        printf("%llu", a);
        if (i != (n-1))
            printf("\n");
    }
    return 0;
}