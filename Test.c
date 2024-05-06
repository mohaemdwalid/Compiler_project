#include <stdio.h>

void printNumbers(int n);

int main() {
    int i = 0;
    char x= 'a';
    int a ,  b ,c;
    a = b - c;
    

    printf("Using for loop:\n");
    for (i = 0; i < 5; i++) {
        printf("%d ", i);
    }

    printf("Using while loop:\n");
    i = 1; 
    while (i <= 5) {
        printf("%d ", i);
        i++;
    }

    return 0;
}

void printNumbers(int n) {
    printf("Numbers from 1 to %d: ", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", i);
    }
    printf("\n");
}
