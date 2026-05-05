#ifndef UTILS_H
#define UTILS_H
/*
 * Prototipos para funciones auxiliares
 */
#include <stdbool.h>

#ifdef _WIN32
#define PATH_DIV "\\"
#else
#define PATH_DIV "/"
#endif

// Funcion para limpiar la consola
void clearConsole();
bool isInRange(int x, int a, int b);

#endif
