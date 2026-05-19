#ifndef UTILS_H
#define UTILS_H
/*
 * Prototipos para funciones auxiliares
 */
#include <stdbool.h>

#define PATH_DIV "/"

// Funcion para limpiar la consola
void clearConsole();
bool isInRange(int x, int a, int b);
int clearBuffer();
int getInt();
double getDouble();
#endif
