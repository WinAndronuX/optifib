/*
 * Funciones auxiliares
 */

#include <stdlib.h>
#include <optifib/utils.h>
#include <stdio.h>

void clearConsole() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

bool isInRange(int x, int a, int b) {
    return x >= a && x <= b;
}

int clearBuffer() {
    int c, n = 0;
    while ((c = getchar()) != '\n' && c != EOF) n++;
    return n;
}

int getInt(int min, int max) {
    int val = 0;
    while (1) {
        printf("> ");
        int r = scanf("%d", &val);
        int n = clearBuffer();
        if ((r == 1 && n == 0 ) && (val >= min && val <= max)) {
            return val;
        }
        printf("\t|ERROR| Debe ingresar un numero entre %d y %d.\n", min, max);
    }
}

void pausa() {
    printf("\nOprima [Enter] para continuar...");
    getchar();
}
