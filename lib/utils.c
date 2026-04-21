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
