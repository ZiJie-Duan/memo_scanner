#include <stdio.h>

int main() {
    printf("I am a sheep v2.0\n");
    int memo = 0;

    while (1) {
        printf("memo: %d\n", memo);
        getchar();
        getchar();
        printf("memo: %d\n", memo);
        printf("change memo: ");
        scanf("%d", &memo);
    }

    return 0;
}