/*
 * Funciones auxiliares
 */

#include <stdlib.h>
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
