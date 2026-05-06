/*
 * Funciones auxiliares
 */

#include <stdlib.h>
#include <stdio.h>
#include <optifib/utils.h>

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

int getInt() {
    int val = 0;
    while (1) {
        printf("\n> ");
        int r = scanf("%d", &val);
        int n = clearBuffer();
        if (r == 1 && n == 0) {
            return val;
        }
        printf("\t|ERROR| Entrada invalida, debe ser un numero entero.\n");
    }
}

double getDouble() {
    double val = 0.0;
    while (1) {
        printf("\n> ");
        int r = scanf("%lf", &val);
        int n = clearBuffer();
        if (r == 1 && n == 0) {
            return val;
        }
        printf("\t|ERROR| Entrada invalida. Por favor, ingresa un numero decimal. ");
    }
}

